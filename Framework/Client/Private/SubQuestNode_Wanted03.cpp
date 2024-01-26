#include "stdafx.h"
#include "SubQuestNode_Wanted03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted03::CSubQuestNode_Wanted03()
{
}

HRESULT CSubQuestNode_Wanted03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("수상한 남자 잡기");
	m_strQuestContent = TEXT("수상해 보이는 남자 찾기");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("수상한 남자 잡기");
	m_strNextQuestContent = TEXT("수상한 남자 추격하기");

	return S_OK;
}

void CSubQuestNode_Wanted03::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_Criminal_Npc"), nullptr, &m_pCriminal)))
	{
		MSG_BOX("Fail AddGameObj : Quest Criminal_Npc");
	}
	else
	{
		Vec4 vSpotPos = { -38.75f, -6.75f, 39.5f, 1.f };
		// m_pCriminal->Get_Component<CTransform>(TEXT("Com_Transform"))->Set_State(CTransform::STATE_POSITION, { -40.f, -6.75f, 34.7f, 1.f });
		// m_pCriminal->Get_Component<CTransform>(TEXT("Com_Transform"))->FixRotation(0.f, -160.f, 0.f);

		// 임시로 monster에 
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	}

}

CBTNode::NODE_STATE CSubQuestNode_Wanted03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* 추후 현상범 추가되면 수정. */
	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);
	
			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

					m_bIsClear = true;
					m_pQuestDestSpot->Set_Dead(true);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted03* CSubQuestNode_Wanted03::Create()
{
	CSubQuestNode_Wanted03* pInstance = new CSubQuestNode_Wanted03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted03::Free()
{
	__super::Free();
}
