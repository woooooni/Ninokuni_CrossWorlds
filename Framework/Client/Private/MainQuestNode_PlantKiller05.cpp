#include "stdafx.h"
#include "MainQuestNode_PlantKiller05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "GameNpc.h"
#include "Ruby.h"
#include "RubyCarriage.h"

CMainQuestNode_PlantKiller05::CMainQuestNode_PlantKiller05()
{
}

HRESULT CMainQuestNode_PlantKiller05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("플랜트 킬러 채집");
	m_strQuestContent = TEXT("루비와 함께 플랜트 킬러를 채집하러 가자");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("마차 호위");
	m_strNextQuestContent = TEXT("루비와 마차를 지키며 목적지로 이동하자");

	// 추후 중간중간 쿠우 대사 집어넣기 = 호송 퀘스트 만드는 클래스에서 하면 될 듯?
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

void CMainQuestNode_PlantKiller05::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::GetInstance()->QUESTEVENT_ESCORT);
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
	if (nullptr == m_pRuby)
		return;

	CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(m_pRuby->Get_RidingObject());
	if (nullptr == pRubyCarriage)
		return;

	pRubyCarriage->Set_TakeTheCarriage(true);
	//m_pRuby->Get_Component_StateMachine()->Change_State(CGameNpc::NPC_STATE::NPC_UNIQUENPC_SEAT);
}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pRuby != nullptr && m_pRuby->Get_Component_StateMachine()->Get_CurrState() == CGameNpc::NPC_IDLE && m_pQuestDestSpot == nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pRuby);

		// 임시로 monster에 
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	}


	if (m_pQuestDestSpot != nullptr)
	{
		m_pQuestDestSpot->Tick(fTimeDelta);
		m_pQuestDestSpot->LateTick(fTimeDelta);

		if (m_pQuestDestSpot->Get_IsCol())
		{
			CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

			m_bIsClear = true;
			m_pQuestDestSpot->Set_ReadyDelete(true);
			Safe_Release(m_pQuestDestSpot);
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_PlantKiller05::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_PlantKiller05* CMainQuestNode_PlantKiller05::Create()
{
	CMainQuestNode_PlantKiller05* pInstance = new CMainQuestNode_PlantKiller05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller05::Free()
{
	__super::Free();
}
