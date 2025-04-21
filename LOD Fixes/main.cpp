#include "nvse/PluginAPI.h"
#include "GameData.hpp"
#include "stack"
#include "Allocator/BSMemory.hpp"

BS_ALLOCATORS

NVSEInterface* g_nvseInterface{};
IDebugLog	   gLog("logs\\LOD Fixes.log");

#define LOGGING 0

#if LOGGING
#define DEBUG_MSG(...) _MESSAGE(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info) {
	info->infoVersion   = PluginInfo::kInfoVersion;
	info->name          = "LOD Fixes";
    info->version       = 133;
	return !nvse->isEditor;
}

static bool bUseSpecular = false;
static bool bUseFullShader = false;
static bool bAllowForceUpdates = true;
static UInt32 uiSpecularShine = 65;

std::vector<NiPointer<NiAVObject>> kAnimatedLODObjects;

void BGSDistantObjectBlock::Prepare() {
    bool bUseNormalLOD = !bPrepared || spShape && spPreviousShape && spShape != spPreviousShape;
    bool bUseStinger = !bPrepared || spStingerBlock && spPreviousBlock && spStingerBlock != spPreviousBlock;

    NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
    if (bUseNormalLOD && spShape) [[likely]] {
        UInt32 uiChildCount = spBlock->GetChildCount();

        for (UInt32 i = 0; i < uiChildCount; i++) [[likely]] {
            NiAVObject* pChild = spBlock->GetAt(i);
            if (IS_SEGMENTEDTRISHAPE(pChild)) [[likely]] {
                BSSegmentedTriShape* pShape = static_cast<BSSegmentedTriShape*>(pChild);
                pShape->RemoveProperty(NiProperty::ALPHA);

                NiAlphaProperty* pAlphaProp = NiAlphaProperty::CreateObject();

                pAlphaProp->m_usFlags.Clear();
                pAlphaProp->SetAlphaTesting(true);
                pAlphaProp->SetAlphaBlending(false);
                pAlphaProp->SetTestRef(128u);
                pAlphaProp->SetTestMode(NiAlphaProperty::TEST_GREATER);
                pShape->AddProperty(pAlphaProp);

                if (bUseSpecular) {
                    NiMaterialProperty* pMatProp = static_cast<NiMaterialProperty*>(pShape->GetProperty(NiProperty::MATERIAL));
                    if (!pMatProp) {
                        static NiPointer<NiMaterialProperty> spDefaultMaterial = NiMaterialProperty::CreateObject();
                        spDefaultMaterial->m_fShine = uiSpecularShine;
                        pShape->AddProperty(spDefaultMaterial);
                    }
                }

                BSShaderProperty* pShaderProp = static_cast<BSShaderProperty*>(pShape->GetProperty(NiProperty::SHADE));
                pShaderProp->SetFlag(BSShaderProperty::BSSP_LOD_BUILDING, true);
                pShaderProp->SetFlag(BSShaderProperty::BSSP_LOD_LANDSCAPE, false);
                pShaderProp->SetFlag(BSShaderProperty::BSSP_SPECULAR, bUseSpecular); // vanilla is false
                pShaderProp->SetFlag(BSShaderProperty::BSSP_STINGER_PROP, bUseFullShader); // Obsidian-made flag for their unused LOD system. Disables distance checks for specular
                BSShaderProperty* p30ShaderProp = pShaderProp->PickShader(pShape, 0, 1);
                if (p30ShaderProp) {
                    pShape->RemoveProperty(NiProperty::SHADE);
                    pShaderProp = p30ShaderProp;
                    pShape->AddProperty(p30ShaderProp);
                }
                BSShader* pShader = BSShaderManager::GetShader(pShaderProp->uiShaderIndex);
                pShape->m_pShader = pShader;
                pShape->UpdatePropertiesUpward();
                NiUpdateData kData;
                pShape->Update(kData);
                pShape->SetFixedBound(true);

                pTerrainNode->UpdateBlockVisibility(false);

                NiGeometryData* pModelData = pShape->GetModelData();
                pModelData->SetKeepFlags(NiGeometryData::KEEP_NONE);
                pModelData->SetConsistency(NiGeometryData::STATIC);

                pRenderer->PrecacheGeometry(pShape, 0, 0, pShader->GetShaderDeclaration(pShape, pShaderProp));
                pRenderer->PerformPrecache();
            }
            else if (IS_NODE(pChild)) [[unlikely]] {
                NiNode* pNode = static_cast<NiNode*>(pChild);
                pNode->SetAppCulled(false);
                CdeclCall(0x4B5D10, pNode); // Remove debug markers
                pNode->CreateWorldBoundIfMissing();
                pNode->UpdatePropertiesUpward();
                NiUpdateData kData;
                pNode->Update(kData);
                BSShaderManager::AssignShaders(pNode, false, false);
            }
        }

        if (NiNode::HasControllers(spBlock)) [[unlikely]] {
            CdeclCall(0xA6D2D0, spBlock); // Start animations
            DEBUG_MSG("Adding animated object %x, ref count %i + 1", spBlock, spBlock->m_uiRefCount);
            kAnimatedLODObjects.push_back(spBlock.m_pObject);
        }

        spBlock->UpdateWorldBound();
        bPrepared = true;
    }

    if (bUseStinger && spStingerBlock) [[unlikely]] {
        PrepareStinger(spStingerBlock);
        pRenderer->PerformPrecache();
        bPrepared = true;
    }
}

void BGSTerrainNode::UpdateBlockVisibility(bool) {
    TES* pTES = TES::GetSingleton();
    TESWorldSpace* pWorldspace = TES::GetWorldSpace();
    if (!pObjectBlock)
        return;

    BSSegmentedTriShape* pSegmentedShape = pObjectBlock->spShape;
    if (pSegmentedShape) [[likely]] {
        BSMultiBoundNode* pBlock = pObjectBlock->GetBlock(false);
        if (IsPlayerInRange()) {
            for (SInt32 x = 0; x < uiLODLevel; ++x) {
                for (SInt32 y = 0; y < uiLODLevel; ++y) {
                    UInt32 uiLevel = y + x * uiLODLevel;

                    TESObjectCELL* pCell = pWorldspace->GetCellAtCoord(x + GetCellX(), y + GetCellY());
                    bool bIsLoaded = TES::IsCellLoaded(pCell, true);

                    for (UInt32 uiObject = 0; uiObject < pBlock->GetArrayCount(); uiObject++) {
                        NiAVObject* pChild = pBlock->GetAt(uiObject);
						if (!pChild || !IS_SEGMENTEDTRISHAPE(pChild)) [[unlikely]]
                            continue;

                        // Intentional overwrite - spShape is a child of the multibound node
                        // We are handling multiple segmented trishapes instead of just one
                        pSegmentedShape = static_cast<BSSegmentedTriShape*>(pChild);
                        if (uiLevel >= pSegmentedShape->GetNumSegments())
                            continue;

                        if (bIsLoaded && !pSegmentedShape->IsSegmentEmpty(uiLevel) && pCell->bCanHideLOD)
                            pSegmentedShape->DisableSegment(uiLevel);
                        else
                            pSegmentedShape->EnableSegment(uiLevel);
                    }
                }
            }

            // Update visibility of all children regardless of LOD level
            for (UInt32 uiObject = 0; uiObject < pBlock->GetArrayCount(); uiObject++) {
                NiAVObject* pChild = pBlock->GetAt(uiObject);
                if (!pChild)
                    continue;

                if (IS_SEGMENTEDTRISHAPE(pChild)) [[likely]] {
                    pSegmentedShape = static_cast<BSSegmentedTriShape*>(pChild);
                    pSegmentedShape->UpdateDrawData();
                }
                else if (!pChild->GetIgnoreFade()) {
                    NiPoint3 kPos = pChild->m_kWorld.m_Translate;
                    TESObjectCELL* pCell = pWorldspace->GetCellAtCoord(int(kPos.x) >> 12, int(kPos.y) >> 12);
                    if (!pCell)
                        continue;

                    bool bIsLoaded = TES::IsCellLoaded(pCell, true);
                    pChild->SetAppCulled(bIsLoaded);
                    NiUpdateData kData;
                    pChild->Update(kData);
                }
            }
        }
        else {
            for (UInt32 uiObject = 0; uiObject < pBlock->GetArrayCount(); uiObject++) {
                NiAVObject* pChild = pBlock->GetAt(uiObject);
                if (!pChild) [[unlikely]]
                    continue;

                if (IS_SEGMENTEDTRISHAPE(pChild)) [[likely]]
                    static_cast<BSSegmentedTriShape*>(pChild)->EnableAllSegments();
                else
                    pChild->SetAppCulled(false);
            }
        }
    }

    BSMultiBoundNode* pStingerBound = pObjectBlock->GetBlock(true);
    if (!pStingerBound) [[likely]]
        return;

    if (IsPlayerInRange())
        pObjectBlock->ToggleVisibilityRecurse(pStingerBound, uiLODLevel, GetCellX(), GetCellY());
    else
        pObjectBlock->ShowRecurse(pStingerBound);
}

static void SetWaterMultiBoundHeight(NiGeometry* apWaterMesh) {
    if (!apWaterMesh)
        return;

    DEBUG_MSG("[ SetWaterMultiBoundHeight ] Setting AABB height");
    NiGeometryData* pGeometryData = apWaterMesh->GetModelData();
    if (!pGeometryData || !pGeometryData->GetVertices()) {
        return;
    }

    NiNode* pParent = apWaterMesh->GetParent();
    if (!pParent || !IS_MULTIBOUNDNODE(pParent)) {
        DEBUG_MSG("[ SetWaterMultiBoundHeight ] Water mesh parent is null or not a multi bound node!");
        return;
    }

    BSMultiBoundNode* pMultiBoundNode = static_cast<BSMultiBoundNode*>(pParent);
    BSMultiBound* pMultiBound = pMultiBoundNode->GetMultiBound();
    if (!pMultiBound) {
        DEBUG_MSG("[ SetWaterMultiBoundHeight ] MultiBound is null!");
        return;
    }

    BSMultiBoundAABB* pMultiBoundAABB = static_cast<BSMultiBoundAABB*>(pMultiBound->GetShape());
    if (!pMultiBoundAABB || pMultiBoundAABB->GetType() != BSMultiBoundShape::BSMB_SHAPE_AABB) {
        DEBUG_MSG("[ SetWaterMultiBoundHeight ] MultiBound AABB is null or not an AABB!");
        return;
    }

    pMultiBoundAABB->Center.z = pGeometryData->GetVertices()[0].z;
    pMultiBoundAABB->HalfExtents.z = 1.f;

    apWaterMesh->SetFixedBound(true);
    pMultiBoundNode->SetFixedBound(true);
}

void BGSTerrainChunk::AttachWaterLOD(bool abForce) {
    ThisStdCall(0x6FB170, this, abForce);
    SetWaterMultiBoundHeight(spWaterMesh);
    SetWaterMultiBoundHeight(spWaterReflectMesh);
}

void BGSTerrainChunk::InitializeShaderProperty() {
    ThisStdCall(0x6FB7E0, this);
    // Fix specular flag being applied if normal map has an alpha channel
    // LOD terrain has no specular capable shader, so game uses the default ADTS shader.
    // This breaks everything, because ADTS shaders are not compatible with LOD terrain.
    BSShaderProperty* pShaderProp = static_cast<BSShaderProperty*>(spLandMesh->GetProperty(NiProperty::SHADE));
    if (pShaderProp)
        pShaderProp->SetFlag(BSShaderProperty::BSSP_SPECULAR, false);
}

void TESObjectCELL::AddReference(TESObjectREFR* apRef, bool abOnTop) {
	// Fix for imposter references being... detached from the cell after being... attached to the cell.
    // This leads to them not being updated, as they are gone from cell's list of animated references. Genius.
    // Imposters are always attached to the *current* cell player is in (whose update always happens first), so we can safely ignore calls coming from ::AssignPersistentRefsToCell afterwards
    if (apRef) {
        NiNode* pRoot = apRef->Get3D();
        if (pRoot && pRoot->GetParent() && apRef->IsImposter()) {
            DEBUG_MSG("[ TESObjectCELL::AddReference ] Ignoring imposter reference %08X - Already attached by the player", apRef->uiFormID);
            return;
        }
    }

    ThisStdCall(0x548230, this, apRef, abOnTop);
}

namespace WaterReflectionFix {
    static bool* const bForceHighDetailReflections = (bool*)0x11C7C04;
    static float fOrgLODDrop = 0.f;
    static std::vector<BSSegmentedTriShape*> kShapesToRestore;

    static void ToggleLODRecurse(NiAVObject* apObject, bool abIgnore) {
        if (!apObject)
            return;

        if (IS_NODE(apObject)) {
            NiNode* pNode = static_cast<NiNode*>(apObject);
            for (UInt32 i = 0; i < pNode->GetArrayCount(); i++)
                ToggleLODRecurse(pNode->GetAt(i), abIgnore);
        }
        else if (IS_SEGMENTEDTRISHAPE(apObject)) {
            BSSegmentedTriShape* pSegTriShape = static_cast<BSSegmentedTriShape*>(apObject);
            pSegTriShape->bIgnoreSegments = abIgnore;
            if (abIgnore) {
                if (!pSegTriShape->pSegments[0].bVisible)
                    kShapesToRestore.push_back(pSegTriShape);

                pSegTriShape->pSegments[0].bVisible = true;
            }
        }
    }

    static void ShowLOD() {
        if (*bForceHighDetailReflections)
            return;

        NiNode* pRoot = *(NiNode**)0x11D8690;
        fOrgLODDrop = BSShaderManager::GetLODLandDrop();
        BSShaderManager::SetLODLandDrop(0.f);
        ToggleLODRecurse(pRoot, true);

    }

    static void HideLOD() {
        if (*bForceHighDetailReflections)
            return;

        NiNode* pRoot = *(NiNode**)0x11D8690;
        ToggleLODRecurse(pRoot, false);
        BSShaderManager::SetLODLandDrop(fOrgLODDrop);

        for (BSSegmentedTriShape* pShape : kShapesToRestore)
            pShape->pSegments[0].bVisible = false;

        kShapesToRestore.clear();
    }
}

static void UpdateLODAnimations() {
	if (kAnimatedLODObjects.empty())
		return;

    std::stack<NiPointer<NiAVObject>> kRemoveStack;
    NiUpdateData kUpdateData = NiUpdateData(*(float*)0x11C3C08, true);
    for (NiAVObject* pObject : kAnimatedLODObjects) {
        if (!pObject) {
			DEBUG_MSG("Object is null, removing from animated list");
            continue;
        }

        if (pObject->m_uiRefCount > 2)
            pObject->UpdateControllers(kUpdateData);
        else {
            DEBUG_MSG("Object %x ref count %i, adding to the removal list", pObject, pObject->m_uiRefCount);
            kRemoveStack.push(pObject);
        }
    }

    while (!kRemoveStack.empty()) {
        NiAVObject* pObject = kRemoveStack.top();
        kRemoveStack.pop();
        DEBUG_MSG("Removing object %x, ref count %i", pObject, pObject->m_uiRefCount);
        kAnimatedLODObjects.erase(std::remove(kAnimatedLODObjects.begin(), kAnimatedLODObjects.end(), pObject), kAnimatedLODObjects.end());
    }
}

static void UpdateLOD() {
	if (!bAllowForceUpdates)
		return;

    auto pWorld = TES::GetWorldSpace();
    if (!pWorld)
        return;

    BGSTerrainManager* pTerrainManager = pWorld->GetTerrainManager();
	if (!pTerrainManager)
		return;

    if (!pTerrainManager->bNeedsImmediateUpdate)
        return;

    DEBUG_MSG("Force Updating LOD");
	PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();
    if (TESMain::GetSingleton()->bIsFlyCam)
        pTerrainManager->Update(&pPlayer->kFlycamPos.kPosition, 0xF);
    else 
        pTerrainManager->Update(pPlayer->GetPos(), 0xF);
}

static void MessageHandler(NVSEMessagingInterface::Message* msg) {
    switch (msg->type) {
    case NVSEMessagingInterface::kMessage_MainGameLoop:
        if (!TESMain::GetSingleton()->bFreezeTime) {
            UpdateLODAnimations();
            UpdateLOD();
        }
        break;
    default:
        break;
    }
}

bool NVSEPlugin_Load(NVSEInterface* nvse) {
	if (!nvse->isEditor) {
        ((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(nvse->GetPluginHandle(), "NVSE", MessageHandler);

        char cINIDir[MAX_PATH];
        GetModuleFileNameA(GetModuleHandle(NULL), cINIDir, MAX_PATH);
        strcpy((char*)(strrchr(cINIDir, '\\') + 1), "Data\\NVSE\\Plugins\\LOD Fixes.ini");
        bUseSpecular        = GetPrivateProfileInt("Main", "bUseSpecular", 0, cINIDir);
        bUseFullShader      = GetPrivateProfileInt("Main", "bUseFullShader", 0, cINIDir);
        uiSpecularShine     = GetPrivateProfileInt("Main", "uiSpecularShine", 65, cINIDir);
		bAllowForceUpdates  = GetPrivateProfileInt("Main", "bAllowForceUpdates", 0, cINIDir);

		ReplaceCallEx(0x6FB0FB, &BGSTerrainChunk::InitializeShaderProperty);
        ReplaceCallEx(0x6F6011, &BGSDistantObjectBlock::Prepare);

		ReplaceCallEx(0x6F696E, &BGSTerrainNode::UpdateBlockVisibility);
		ReplaceCallEx(0x6F6C73, &BGSTerrainNode::UpdateBlockVisibility);
		ReplaceCallEx(0x6FE0F5, &BGSTerrainNode::UpdateBlockVisibility);

        // Fix imposters failing to animate
        // Only one specific call to AddReference is the offender here
        ReplaceCallEx(0x5881D1, &TESObjectCELL::AddReference);

        // Fix object LOD not reflecting in water
        ReplaceCall(0x4EB6C6, WaterReflectionFix::ShowLOD);
        ReplaceCall(0x4EB893, WaterReflectionFix::HideLOD);

        // Fix water LOD multibound height
        for (UInt32 uiAddr : {0x6FA964, 0x6FB0C0 })
            ReplaceCallEx(uiAddr, &BGSTerrainChunk::AttachWaterLOD);
	}

	return true;
}