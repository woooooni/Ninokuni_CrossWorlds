#include "stdafx.h"
#include "PumpkinCandleNode_Dead.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CPumpkinCandleNode_Dead::CPumpkinCandleNode_Dead()
{
}

HRESULT CPumpkinCandleNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Death"));
}

CBTNode::NODE_STATE CPumpkinCandleNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		// 여기서 객체 지우는 코드.
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Dead* CPumpkinCandleNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CPumpkinCandleNode_Dead* pInstance = new CPumpkinCandleNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Dead::Free()
{
	__super::Free();
}
