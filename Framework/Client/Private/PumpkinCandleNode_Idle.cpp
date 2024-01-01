#include "stdafx.h"
#include "PumpkinCandleNode_Idle.h"

#include "BehaviorTree.h"


#include "Model.h"

CPumpkinCandleNode_Idle::CPumpkinCandleNode_Idle()
{
}

HRESULT CPumpkinCandleNode_Idle::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CPumpkinCandleNode_Idle::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CPumpkinCandleNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Fix() && m_tBTMonsterDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleStand"));
	}

	if (m_bWait)
	{
		if (Behavior_Wait(0.1f, fTimeDelta))
		{
			m_bWait = false;
			m_iCurAnimationIndex += 1;

			if (m_iCurAnimationIndex == m_vecAnimationName.size())
				m_iCurAnimationIndex = 0;

			return NODE_STATE::NODE_SUCCESS;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Idle* CPumpkinCandleNode_Idle::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	CPumpkinCandleNode_Idle* pInstance = new CPumpkinCandleNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Idle::Free()
{
	__super::Free();
}
