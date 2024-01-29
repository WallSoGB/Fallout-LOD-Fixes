#include "nvse/PluginAPI.h"
#include "SafeWrite.h"
#include "GameData.hpp"
#include <stack>
#include <chrono>

NVSEInterface* g_nvseInterface{};
IDebugLog	   gLog("logs\\LOD Fixes.log");

#define LOGGING 0

#if LOGGING
#define DEBUG_MSG(...) _MESSAGE(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "LOD Fixes";
	info->version = 120;
	return true;
}

static bool bUseSpecular = false;

NiUpdateData NiUpdateData::kDefaultUpdateData = NiUpdateData();

std::vector<NiPointer<NiAVObject>> kAnimatedLODObjects;

void __fastcall BGSDistantObjectBlock::Prepare(BGSDistantObjectBlock* apThis) {
    bool bUseNormalLOD = !apThis->bPrepared || apThis->spSegmentedTriShape && apThis->spNode0C && apThis->spSegmentedTriShape != apThis->spNode0C;
    bool bUseStinger = !apThis->bPrepared || apThis->spNode10 && apThis->spNode14 && apThis->spNode10 != apThis->spNode14;

    NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
    if (bUseNormalLOD) {
        if (apThis->spSegmentedTriShape) {
            UInt32 uiChildCount = apThis->spMultiboundNode->GetChildCount();

            for (UInt32 i = 0; i < uiChildCount; i++) {
                NiAVObject* pChild = apThis->spMultiboundNode->GetAt(i);
                if (pChild->IsSegmentedTriShape()) {
                    BSSegmentedTriShape* pShape = static_cast<BSSegmentedTriShape*>(pChild);
                    pShape->RemoveProperty(0);

                    NiAlphaProperty* pAlphaProp = NiAlphaProperty::CreateObject();


                    pAlphaProp->m_usFlags.Clear();
                    pAlphaProp->SetAlphaTesting(true);
                    pAlphaProp->SetAlphaBlending(false);
                    pAlphaProp->SetTestRef(128u);
                    pAlphaProp->SetTestMode(NiAlphaProperty::TEST_GREATER);
                    pShape->AddProperty(pAlphaProp);

                    BSShaderProperty* pShaderProp = static_cast<BSShaderProperty*>(pShape->GetProperty(3));
                    pShaderProp->SetFlag(BSShaderProperty::BSSP_LOD_BUILDING, true);
                    pShaderProp->SetFlag(BSShaderProperty::BSSP_LOD_LANDSCAPE, false);
                    pShaderProp->SetFlag(BSShaderProperty::BSSP_SPECULAR, bUseSpecular); // vanilla is false
                    BSShaderProperty* p30ShaderProp = pShaderProp->PickShader(pShape, 0, 1);
                    if (p30ShaderProp) {
                        pShape->RemoveProperty(3);
                        pShaderProp = p30ShaderProp;
                        pShape->AddProperty(p30ShaderProp);
                    }
                    BSShader* pShader = BSShaderManager::GetShader(pShaderProp->uiShaderIndex);
                    pShape->m_pShader = pShader;
                    pShape->UpdatePropertiesUpward();
                    pShape->Update(NiUpdateData::kDefaultUpdateData);
                    pShape->SetFixedBound(true);

                    BGSTerrainNode::UpdateBlockVisibility(apThis->pTerrainNode, 0, 0);

                    NiGeometryData* pModelData = pShape->GetModelData();
                    pModelData->SetKeepFlags(NiGeometryData::KEEP_NONE);
                    pModelData->SetConsistency(NiGeometryData::MUTABLE);
                    pModelData->SetCompressFlags(NiGeometryData::COMPRESS_NORM);

                    pRenderer->PrecacheGeometry(pShape, 0, 0, pShader->GetShaderDeclaration(pShape, pShaderProp));
                    pRenderer->PerformPrecache();
                }
                else if (pChild->IsNiNode()) {
                    NiNode* pNode = static_cast<NiNode*>(pChild);
                    pNode->SetAppCulled(false);
                    CdeclCall(0x4B5D10, pNode); // Remove debug markers
                    pNode->CreateWorldBoundIfMissing();
                    pNode->UpdatePropertiesUpward();
                    pNode->Update(NiUpdateData::kDefaultUpdateData);
                    CdeclCall(0xB57E30, pNode, false, false); // Assign shaders
                }
            }
            bool bHasControllers = NiNode::HasControllers(apThis->spMultiboundNode);
            if (bHasControllers) {
                CdeclCall(0xA6D2D0, apThis->spMultiboundNode); // Start animations
                kAnimatedLODObjects.push_back(apThis->spMultiboundNode);
            }

            apThis->spMultiboundNode->UpdateWorldBound();
            apThis->bPrepared = true;
        }
    }
    if (bUseStinger) {
        if (apThis->spNode10) {
            ThisStdCall(0x6F6A90, apThis, apThis->spNode10);
            pRenderer->PerformPrecache();
            apThis->bPrepared = true;
        }
    }
}

void __fastcall BGSTerrainNode::UpdateBlockVisibility(BGSTerrainNode* apThis, void*, bool) {
    TES* pTES = TES::GetSingleton();
    TESWorldSpace* pWorldspace = TES::GetWorldSpace();
    if (apThis->pObjectBlock) {
        BSSegmentedTriShape* pSegmentedTriShape = apThis->pObjectBlock->spSegmentedTriShape;
        BSMultiBoundNode* pMultiBoundNode = apThis->pObjectBlock->GetMultiBound(false);
        if (pSegmentedTriShape) {
            if (apThis->IsPlayerInRange()) {
                for (SInt32 x = 0; x < apThis->uiLODLevel; ++x) {
                    for (SInt32 y = 0; y < apThis->uiLODLevel; ++y) {
                        SInt32 uiLevel = y + x * apThis->uiLODLevel;
                        

                        TESObjectCELL* pCell = pWorldspace->GetCellAtCoord(x + apThis->GetCellX(), y + apThis->GetCellY());
                        bool bIsLoaded = TES::IsCellLoaded(pCell, true);

                        for (UInt32 uiObject = 0; uiObject < pMultiBoundNode->GetChildCount(); uiObject++) {
                            NiAVObject* pChild = pMultiBoundNode->GetAt(uiObject);
                            if (pChild && pChild->IsSegmentedTriShape()) {
                                pSegmentedTriShape = static_cast<BSSegmentedTriShape*>(pChild);
                                if (uiLevel < pSegmentedTriShape->GetNumSegments()) {
                                    if (!pSegmentedTriShape->IsSegmentEmpty(uiLevel) && bIsLoaded && pCell->bCanHideLOD)
                                        pSegmentedTriShape->DisableSegment(uiLevel);
                                    else
                                        pSegmentedTriShape->EnableSegment(uiLevel);
                                }
                            }
                        }
                    }
                }
                for (UInt32 uiObject = 0; uiObject < pMultiBoundNode->GetChildCount(); uiObject++) {
                    NiAVObject* pChild = pMultiBoundNode->GetAt(uiObject);
                    if (!pChild)
                        continue;

                    if (pChild->IsSegmentedTriShape()) {
                        pSegmentedTriShape = static_cast<BSSegmentedTriShape*>(pChild);
                        pSegmentedTriShape->UpdateDrawData();
                    }
                    else if (!pChild->GetIgnoreFade()) {
                        NiPoint3 kPos = pChild->m_kWorld.m_Translate;
                        TESObjectCELL* pCell = pWorldspace->GetCellAtCoord(int(kPos.x) >> 12, int(kPos.y) >> 12);
                        if (pCell) {
                            bool bIsLoaded = TES::IsCellLoaded(pCell, true);
                            pChild->SetAppCulled(bIsLoaded);
                            pChild->Update(NiUpdateData::kDefaultUpdateData);
                        }
                    }
                }
            }
            else {
                for (UInt32 uiObject = 0; uiObject < pMultiBoundNode->GetChildCount(); uiObject++) {
                    NiAVObject* pChild = pMultiBoundNode->GetAt(uiObject);
                    if (pChild->IsSegmentedTriShape())
                        static_cast<BSSegmentedTriShape*>(pChild)->EnableAllSegments();
                    else
                        pChild->SetAppCulled(false);
                }
            }
        }
    }
    if (apThis->pObjectBlock && apThis->pObjectBlock->GetMultiBound(true)) {
        if (apThis->IsPlayerInRange()) {
            ThisStdCall(0x6F5970, apThis->pObjectBlock, apThis->pObjectBlock->GetMultiBound(true), apThis->uiLODLevel, apThis->GetCellX(), apThis->GetCellY());
        }
        else {
            apThis->pObjectBlock->ShowRecurse(apThis->pObjectBlock->GetMultiBound(true));
        }
    }
}

static UInt32 GetRelativeFormID(UInt32 auiFormID) {
    return auiFormID & 0xFFFFFF;
}

static UInt32 GetLODFormID(UInt32 auiFormID) {
    UInt32 uiRelativeFormID = GetRelativeFormID(auiFormID);
    return uiRelativeFormID | (1u << 24);
}

void __fastcall BGSDistantTreeBlock::HideLOD(BGSDistantTreeBlock* apThis, void*, UInt32 aID, bool abRegisterFormID) {
    BGSDistantTreeBlock::InstanceData* pInstance = nullptr;
    UInt32 uiUsedFormID = aID;
    bool bTreeFound = false;

    DEBUG_MSG("\n===== Hiding trees =====");
    if (apThis->kInstanceMap.m_uiCount == 0) {
        DEBUG_MSG("Instance map is empty!");
    }
    else {
        bTreeFound = apThis->kInstanceMap.GetAt(uiUsedFormID, pInstance);
        if (!bTreeFound) {
            DEBUG_MSG("Tree %08X not found, trying LOD ID", aID);
            uiUsedFormID = GetLODFormID(aID);
            if (uiUsedFormID != aID) {
                DEBUG_MSG("Trying tree %08X", uiUsedFormID);
                bTreeFound = apThis->kInstanceMap.GetAt(uiUsedFormID, pInstance);
            }
        }
    }

    if (bTreeFound) {
        DEBUG_MSG("Hiding LOD for tree %08X", aID);
        pInstance->bHidden = true;
        (*apThis->kTreeGroups.GetAt(pInstance->uiTreeGroupIndex))->bShaderPropertyUpToDate = false;
    }
    else if (abRegisterFormID) {
        DEBUG_MSG("Tree %08X not found - adding FormID to array", aID);
        ThisStdCall(0x7CB2E0, &apThis->kFormIDs, &aID); // Append
    }
    else {
        DEBUG_MSG("Tree %08X not found", aID);
    }
}

#if LOGGING
static void __fastcall BSMap__SetAt(void* apThis, void*, UInt32 auiFormID, BGSDistantTreeBlock::InstanceData* apInstanceData) {
    DEBUG_MSG("\n ===== Registering tree %08X at %f, %f, %f =====", auiFormID, apInstanceData->kPos.x, apInstanceData->kPos.y, apInstanceData->kPos.z);
    ThisStdCall(0x6F9A80, apThis, auiFormID, apInstanceData);
}

static void __fastcall BGSDistantTreeBlockLoadTask__Run(void* apThis) {
    DEBUG_MSG("\n===== Loading trees =====");
    ThisStdCall(0x6F9570, apThis);
}

static BGSDistantTreeBlock* __fastcall BGSDistantTreeBlock__BGSDistantTreeBlock(BGSDistantTreeBlock* apThis, void*, BGSTerrainNode* apNode, int lodLevel, __int16 x, __int16 y) {
    DEBUG_MSG("\n===== Creating BGSDistantTreeBlock =====");
    return ThisStdCall<BGSDistantTreeBlock*>(0x6F7540, apThis, apNode, lodLevel, x, y);
}
#endif

static void SetWaterMultiBoundHeight(NiGeometry* apWaterMesh) {
    DEBUG_MSG("[ SetWaterMultiBoundHeight ] Setting AABB height");
    if (!apWaterMesh) {
        DEBUG_MSG("[ SetWaterMultiBoundHeight ] Water mesh is null!");
        return;
    }

    NiGeometryData* pGeometryData = apWaterMesh->GetModelData();
    if (!pGeometryData || !pGeometryData->GetVertices()) {
        return;
    }

    NiNode* pParent = apWaterMesh->GetParent();
    if (!pParent || !pParent->IsMultiBoundNode()) {
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

void __fastcall BGSTerrainChunk::AttachWaterLODEx(BGSTerrainChunk* apThis, void*, bool abForce) {
    ThisStdCall(0x6FB170, apThis, abForce);
    SetWaterMultiBoundHeight(apThis->spWaterMesh);
    SetWaterMultiBoundHeight(apThis->spWaterReflectMesh);
}

static void UpdateLODAnimations() {
    std::stack<NiAVObject*> kRemoveStack;
    NiUpdateData kUpdateData = NiUpdateData(*(float*)0x11C3C08, true);
    for (NiAVObject* pObject : kAnimatedLODObjects) {
        if (pObject) {
            if (pObject->m_uiRefCount > 2)
                pObject->UpdateControllers(kUpdateData);
            else
                kRemoveStack.push(pObject);
        }
    }

    while (!kRemoveStack.empty()) {
        NiAVObject* pObject = kRemoveStack.top();
        DEBUG_MSG("Removing object %x, ref count %i", pObject, pObject->m_uiRefCount);
        kRemoveStack.pop();
        kAnimatedLODObjects.erase(std::remove(kAnimatedLODObjects.begin(), kAnimatedLODObjects.end(), pObject), kAnimatedLODObjects.end());
        pObject->DeleteThis();
    }
}

static void MessageHandler(NVSEMessagingInterface::Message* msg) {
    switch (msg->type) {
    case NVSEMessagingInterface::kMessage_MainGameLoop:
        UpdateLODAnimations();
        break;
    default:
        break;
    }
}

bool NVSEPlugin_Load(NVSEInterface* nvse) {
	
	if (!nvse->isEditor) {
        ((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(nvse->GetPluginHandle(), "NVSE", MessageHandler);

		ReplaceCall(0x6F6011, BGSDistantObjectBlock::Prepare);
		ReplaceCall(0x6FE0F5, BGSTerrainNode::UpdateBlockVisibility);

        // Fix tree LOD not working in TTW
        SafeWriteBuf(0x485B60, (void*)"\x8B\x41\x0C\xC3", 4);
        for (UInt32 uiAddr : {0x6F9342, 0x6FCCE2, 0x6FCFDF })
            ReplaceCall(uiAddr, BGSDistantTreeBlock::HideLOD);

       for (UInt32 uiAddr : {0x6FA964, 0x6FB0C0 })
            ReplaceCall(uiAddr, BGSTerrainChunk::AttachWaterLODEx);

        char iniDir[MAX_PATH];
        GetModuleFileNameA(GetModuleHandle(NULL), iniDir, MAX_PATH);
        strcpy((char*)(strrchr(iniDir, '\\') + 1), "Data\\NVSE\\Plugins\\LOD Fixes.ini");
        bUseSpecular = GetPrivateProfileInt("Main", "bUseSpecular", 0, iniDir);

        // Debug
#if LOGGING
        ReplaceCall(0x6F8621, BSMap__SetAt);
        ReplaceCall(0x6FE406, BGSDistantTreeBlock__BGSDistantTreeBlock);
        SafeWrite32(0x106DED4, UInt32(BGSDistantTreeBlockLoadTask__Run));
#endif

	}

	return true;
}