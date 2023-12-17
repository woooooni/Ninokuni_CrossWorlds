#include "stdafx.h"
#include "Baobam_WaterNode_Attack1.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_WaterNode_Attack1::CBaobam_WaterNode_Attack1()
{
}

HRESULT CBaobam_WaterNode_Attack1::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Attack1::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Attack01"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CBaobam_WaterNode_Attack1::Tick(const _float& fTimeDelta)
{
	//if (m_bIsSucces)
	//	return NODE_STATE::NODE_SUCCESS;

	// (현재 액션이 취하는 애니메이션, 동작이 끝나고 기다릴 시간, timeDelta)
	return __super::UnLoop_BehaviorTick(TEXT("SKM_Baobam_Water.ao|BaoBam_Attack01"), 2.f, fTimeDelta);

	//if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&  
	//	!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Baobam_Water.ao|BaoBam_Attack01"))
	//{
	//	m_bWait = true;
	//	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_BattleStand"));
	//}
	//
	//if (m_bWait)
	//{
	//	if (Behavior_Wait(4.f, fTimeDelta))
	//	{
	//		m_bIsSucces = true;
	//		m_bWait = false;
	//
	//		if (!dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
	//		{
	//			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
	//			return NODE_STATE::NODE_FAIL;
	//		}
	//	}
	//}
	//
	// return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Attack1* CBaobam_WaterNode_Attack1::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Attack1* pInstance = new CBaobam_WaterNode_Attack1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Attack1::Free()
{
	__super::Free();
}
