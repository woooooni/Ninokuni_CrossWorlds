#include "stdafx.h"
#include "PumpkinCandleNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CPumpkinCandleNode_Chase::CPumpkinCandleNode_Chase()
{
}

HRESULT CPumpkinCandleNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleRun"));
}

CBTNode::NODE_STATE CPumpkinCandleNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 2.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Chase* CPumpkinCandleNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CPumpkinCandleNode_Chase* pInstance = new CPumpkinCandleNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Chase::Free()
{
	__super::Free();
}
