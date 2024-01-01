#include "stdafx.h"
#include "Baobam_WaterNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

CBaobam_WaterNode_Dead::CBaobam_WaterNode_Dead()
{
}

HRESULT CBaobam_WaterNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.f;

	return S_OK;
}

void CBaobam_WaterNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_KnockDown"));
		m_tBTNodeDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-m_tBTNodeDesc.pOwnerTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Death"));
	}
}

CBTNode::NODE_STATE CBaobam_WaterNode_Dead::Tick(const _float& fTimeDelta)
{
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		if (m_tBTNodeDesc.pOwnerModel->Get_CurrAnimationFrame() >= 60)
		{
			m_tBTNodeDesc.pOwnerModel->Set_Stop_Animation(true);
			m_fTime += fTimeDelta;

			if (m_fTime > m_fBlowDeadTime)
			{
				m_tBTNodeDesc.pOwner->Set_Dead(true);
			}
		}
	}
	else
	{
		if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween())
		{
			m_tBTNodeDesc.pOwner->Set_Dead(true);
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Dead* CBaobam_WaterNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Dead* pInstance = new CBaobam_WaterNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Dead::Free()
{
	__super::Free();
}
