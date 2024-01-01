#include "stdafx.h"
#include "IceBearManNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

CIceBearManNode_Dead::CIceBearManNode_Dead()
{
}

HRESULT CIceBearManNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.5f;

	return S_OK;
}

void CIceBearManNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_BlowDead"));
		m_tBTNodeDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-m_tBTNodeDesc.pOwnerTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Death"));
	}
}

CBTNode::NODE_STATE CIceBearManNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween())
	{
		if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
		{
			m_fTime += fTimeDelta;
			if (m_fTime > m_fBlowDeadTime)
			{
				m_tBTNodeDesc.pOwner->Set_Dead(true);
			}
		}
		else
			m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CIceBearManNode_Dead* CIceBearManNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CIceBearManNode_Dead* pInstance = new CIceBearManNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Dead::Free()
{
	__super::Free();
}
