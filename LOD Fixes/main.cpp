#include "nvse/PluginAPI.h"
#include "SafeWrite.h"
#include "GameData.hpp"

NVSEInterface* g_nvseInterface{};
IDebugLog	   gLog("logs\\LOD Fixes.log");

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "LOD Fixes";
	info->version = 110;
	return true;
}

static bool bUseSpecular = false;

NiUpdateData NiUpdateData::kDefaultUpdateData = NiUpdateData();

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

                    CdeclCall(0xA6D2D0, pNode); // Start animations
                }
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

void MessageHandler(NVSEMessagingInterface::Message* msg) {
    TESWorldSpace* pWorld = nullptr;

    switch (msg->type) {
    case NVSEMessagingInterface::kMessage_MainGameLoop:
        pWorld = TES::GetSingleton()->GetWorldSpace();
        if (pWorld)
            pWorld->GetTerrainManager()->UpdateLODAnimations();
        break;
    }
}

static UInt32 GetRelativeFormID(UInt32 auiFormID) {
    return auiFormID & 0xFFFFFF;
}

static UInt32 GetLODFormID(UInt32 auiFormID) {
    UInt32 uiRelativeFormID = GetRelativeFormID(auiFormID);
    return uiRelativeFormID | (1u << 24);
}

void __fastcall BGSDistantTreeBlock::HideLOD(BGSDistantTreeBlock* apThis, void*, UInt32 aID, bool abAddToUIntArray) {
    BGSDistantTreeBlock::InstanceData* pInstance = nullptr;
    UInt32 uiUsedFormID = aID;
    bool bTreeFound = false;

    bTreeFound = apThis->kInstanceMap.GetAt(uiUsedFormID, pInstance);
    if (!bTreeFound) {
        uiUsedFormID = GetLODFormID(aID);
        if (uiUsedFormID != aID)
            bTreeFound = apThis->kInstanceMap.GetAt(uiUsedFormID, pInstance);
    }

    if (bTreeFound) {
        pInstance->bHidden = true;
        (*apThis->kTreeGroups.GetAt(pInstance->uiTreeGroupIndex))->bShaderPropertyUpToDate = false;
    }
    else if (abAddToUIntArray) {
        ThisStdCall(0x7CB2E0, &apThis->kUIntArray, &uiUsedFormID); // Append
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

        char iniDir[MAX_PATH];
        GetModuleFileNameA(GetModuleHandle(NULL), iniDir, MAX_PATH);
        strcpy((char*)(strrchr(iniDir, '\\') + 1), "Data\\NVSE\\Plugins\\LOD Fixes.ini");
        bUseSpecular = GetPrivateProfileInt("Main", "bUseSpecular", 0, iniDir);
	}

	return true;
}