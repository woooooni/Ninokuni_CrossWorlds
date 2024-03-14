#include "stdafx.h"
#include "MainQuestNode_PlantKiller07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "GameNpc.h"
#include "Ruby.h"
#include <FileUtils.h>
#include <Utils.h>
#include "UI_PopupQuest.h"

CMainQuestNode_PlantKiller07::CMainQuestNode_PlantKiller07()
{
}

HRESULT CMainQuestNode_PlantKiller07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("루비를 지켜라");
	m_strQuestContent = TEXT("몬스터들을 막으며 루비를 지키기");

	// 추후 중간중간 쿠우 대사 집어넣기 = 웨이브 막는 클래스가 따로 있으면 거기에. 없으면 여기에.
	//Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/06.MainQuest_WitchForest/MainQuest_WitchForest05_2.json");
	//
	//for (const auto& talkDesc : Load) {
	//	TALK_DELS sTalkDesc;
	//	sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
	//	sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
	//	m_vecTalkDesc.push_back(sTalkDesc);
	//}

	return S_OK;
}

void CMainQuestNode_PlantKiller07::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	m_pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
	if (nullptr == m_pRuby)
		return;

	m_pRuby->Set_QuestSection(CRuby::SECTION3, true);
	Load_Monster_Data();
}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pRuby != nullptr && m_pRuby->Get_Component_StateMachine()->Get_CurrState() == CGameNpc::NPC_IDLE && m_pQuestDestSpot == nullptr)
	{
		Vec4 vSpotPos = Vec4(-75.815f, 11.195f, 46.207f, 1.0f);
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	}

	// 임시
	// 추후에는 호송 이벤트가 종료되면 넘어가기. 또한 퀘스트 팝업도 첫 몬스터 웨이브 소환 후 바꿔준다.
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_MONSTER);

	if (pGameObjects.size() <= 0)
	{

		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot->Get_IsCol())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

				m_bIsClear = true;
				m_pQuestDestSpot->Set_ReadyDelete(true);
				CRuby* pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
				pRuby->Set_QuestSection(CRuby::SECTION3, false);
				pRuby->Get_Component_Transform()->FixRotation(0.0f, 45.f, 0.0f);

				Safe_Release(m_pQuestDestSpot);
				return NODE_STATE::NODE_FAIL;
			}
		}


		//CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
		//m_bIsClear = true;
		//
		//CRuby* pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
		//

		//pRuby->Set_QuestSection(CRuby::SECTION3, false);
		//pRuby->Get_Component_Transform()->FixRotation(0.0f, 45.f, 0.0f);

		//return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_PlantKiller07::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_PlantKiller07::Load_Monster_Data()
{
	wstring strMapFilePath = TEXT("../Bin/DataFiles/Map/Witch/Witch_Escort_3.map");

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;

			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_WITCHFOREST, i, strPrototypeTag, &Init_Data, &pObj, true)))
			{
				MSG_BOX("Load_Objects_Failed.");
				return;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return;
			}



			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}
}

CMainQuestNode_PlantKiller07* CMainQuestNode_PlantKiller07::Create()
{
	CMainQuestNode_PlantKiller07* pInstance = new CMainQuestNode_PlantKiller07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller07::Free()
{
	__super::Free();
}
