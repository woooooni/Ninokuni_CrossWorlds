#include "stdafx.h"
#include "MainQuestNode_FinalBattle02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_FinalBattle02::CMainQuestNode_FinalBattle02()
{
}

HRESULT CMainQuestNode_FinalBattle02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[¸ÞÀÎ]");
	m_strQuestName = TEXT("ÃÖÁ¾Àå");
	m_strQuestContent = TEXT("¾Ç¸ùÀÇ ¸¶³à¿Í ´ëÈ­ÇÏÀÚ");

	return S_OK;
}

void CMainQuestNode_FinalBattle02::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_DreamMazeWitch_Npc"), nullptr, &m_pWitch)))
	{
		MSG_BOX("Fail AddGameObj : Quest DreamMazeWitch_Npc");
	}
	else
	{
		m_pWitch->Get_Component_Transform()->Set_State(CTransform::STATE_POSITION, Vec4(142.2f, 1.f, 107.2f, 1.f));
		m_pWitch->Get_Component_Transform()->FixRotation(0.f, -162.f, 0.f);
		
		Vec4 vSpotPos = { 130.6, -0.5f, 89.7f, 1.f };

		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	}

}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
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
				Safe_Release(m_pQuestDestSpot);
				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle02::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle02* CMainQuestNode_FinalBattle02::Create()
{
	CMainQuestNode_FinalBattle02* pInstance = new CMainQuestNode_FinalBattle02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle02::Free()
{
	__super::Free();
}
