#include "stdafx.h"
#include "PumpkinCandleNode_Attack1.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CPumpkinCandleNode_Attack1::CPumpkinCandleNode_Attack1()
{
}

HRESULT CPumpkinCandleNode_Attack1::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Attack1::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Attack01"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CPumpkinCandleNode_Attack1::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Attack01"), 1.f, fTimeDelta);
}

CPumpkinCandleNode_Attack1* CPumpkinCandleNode_Attack1::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CPumpkinCandleNode_Attack1* pInstance = new CPumpkinCandleNode_Attack1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Attack1::Free()
{
	__super::Free();
}
