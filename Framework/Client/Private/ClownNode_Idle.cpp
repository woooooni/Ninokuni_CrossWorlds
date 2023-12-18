#include "stdafx.h"
#include "ClownNode_Idle.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClownNode_Idle::CClownNode_Idle()
{
}

HRESULT CClownNode_Idle::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CClownNode_Idle::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CClownNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_BattleStand"));
	}

	if (m_bWait)
	{
		if (Behavior_Wait(2.f, fTimeDelta))
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

CClownNode_Idle* CClownNode_Idle::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	CClownNode_Idle* pInstance = new CClownNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CClownNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Idle::Free()
{
	__super::Free();
}
