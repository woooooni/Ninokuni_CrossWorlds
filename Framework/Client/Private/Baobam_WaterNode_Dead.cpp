#include "stdafx.h"
#include "Baobam_WaterNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

CBaobam_WaterNode_Dead::CBaobam_WaterNode_Dead()
{
}

HRESULT CBaobam_WaterNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.f;

	return S_OK;
}

void CBaobam_WaterNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_KnockDown"));
		m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-m_tBTMonsterDesc.pOwnerTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Death"));
	}
}

CBTNode::NODE_STATE CBaobam_WaterNode_Dead::Tick(const _float& fTimeDelta)
{

	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		if (m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimationFrame() >= 60 &&
			m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Baobam_Water.ao|BaoBam_KnockDown"))
		{
			m_tBTMonsterDesc.pOwnerModel->Set_Stop_Animation(true);
			m_fTime += fTimeDelta;

			if (m_fTime > m_fBlowDeadTime)
			{
				m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
			}
		}
	}

	else
	{
		if(m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
			m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}


	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Dead* CBaobam_WaterNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
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
