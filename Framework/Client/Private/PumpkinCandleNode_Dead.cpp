#include "stdafx.h"
#include "PumpkinCandleNode_Dead.h"

#include "BehaviorTree.h"


#include "Model.h"

CPumpkinCandleNode_Dead::CPumpkinCandleNode_Dead()
{
}

HRESULT CPumpkinCandleNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Dead::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Death"));
}

CBTNode::NODE_STATE CPumpkinCandleNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Dead* CPumpkinCandleNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
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
