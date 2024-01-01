#include "stdafx.h"
#include "Shadow_ThiefNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

CShadow_ThiefNode_Dead::CShadow_ThiefNode_Dead()
{
}

HRESULT CShadow_ThiefNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.5f;

	return S_OK;
}

void CShadow_ThiefNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Knock_Start"));
		m_tBTNodeDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			m_tBTNodeDesc.pTargetTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Death"));
	}
}

CBTNode::NODE_STATE CShadow_ThiefNode_Dead::Tick(const _float& fTimeDelta)
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

CShadow_ThiefNode_Dead* CShadow_ThiefNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Dead* pInstance = new CShadow_ThiefNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Dead::Free()
{
	__super::Free();
}
