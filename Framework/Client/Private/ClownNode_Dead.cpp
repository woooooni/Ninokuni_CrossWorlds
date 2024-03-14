#include "stdafx.h"
#include "ClownNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

CClownNode_Dead::CClownNode_Dead()
{
}

HRESULT CClownNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.5f;

	return S_OK;
}

void CClownNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_KnockUp_Start"));
		m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-m_tBTMonsterDesc.pOwnerTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Death"));
	}
}

CBTNode::NODE_STATE CClownNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
		{
			if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground())
			{
				m_fTime += fTimeDelta;
				if (m_fTime > m_fBlowDeadTime)
				{
					m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
				}
			}
		}
		else
			m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CClownNode_Dead* CClownNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClownNode_Dead* pInstance = new CClownNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Dead::Free()
{
	__super::Free();
}
