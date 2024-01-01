#include "stdafx.h"
#include "PumpkinCandleNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CPumpkinCandleNode_Chase::CPumpkinCandleNode_Chase()
{
}

HRESULT CPumpkinCandleNode_Chase::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Chase::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleRun"));
}

CBTNode::NODE_STATE CPumpkinCandleNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 2.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Chase* CPumpkinCandleNode_Chase::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
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
