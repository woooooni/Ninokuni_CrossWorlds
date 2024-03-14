#include "stdafx.h"
#include "Criminal_MonsterNode_Air.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CCriminal_MonsterNode_Air::CCriminal_MonsterNode_Air()
{
}

HRESULT CCriminal_MonsterNode_Air::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fAirTime = 3.f;
	m_fForcedStandUpTime = 5.f;

	return S_OK;
}

void CCriminal_MonsterNode_Air::Start()
{
}

CBTNode::NODE_STATE CCriminal_MonsterNode_Air::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR))
		return NODE_STATE::NODE_FAIL;

	m_fCheckTime += fTimeDelta;

	if (m_fCheckTime >= 0.5f)
	{
		Vec4 vOwnerPos = m_tBTMonsterDesc.pOwnerTransform->Get_Position();

		if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground() ||
			vOwnerPos.y < -5.1f)
		{
			if (!m_bIsDown)
			{
				m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Down"));
				m_bIsDown = true;
			}

			m_fTime += fTimeDelta;
			if (m_fTime >= m_fAirTime)
			{
				if (!m_bIsStand)
				{
					m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|StandUp"));
					m_bIsStand = true;
				}

				if (m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_BlackCircleAgent.ao|StandUp"))
				{
					if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
					{
						dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR, false);
						dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);

						if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
							dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
						else
							dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

						m_fTime = m_fAirTime - m_fTime;
						m_bIsStand = false;
						m_bIsDown = false;
						m_fCheckTime = 0.f;

						return NODE_STATE::NODE_SUCCESS;
					}
				}
			}
		}
	}

	// 안 일어나는 놈들이 있어서 강제 기상
	//m_fAccForcedStandUpTime += fTimeDelta;
	//if (m_fAccForcedStandUpTime >= m_fForcedStandUpTime)
	//{
	//	m_fAccForcedStandUpTime = m_fForcedStandUpTime - m_fAccForcedStandUpTime;
	//
	//	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOW, false);
	//	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);
	//
	//	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
	//		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
	//	else
	//		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
	//
	//	m_fTime = m_fAirTime - m_fTime;
	//	m_bIsStand = false;
	//
	//	return NODE_STATE::NODE_SUCCESS;
	//}


	return NODE_STATE::NODE_RUNNING;
}

CCriminal_MonsterNode_Air* CCriminal_MonsterNode_Air::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CCriminal_MonsterNode_Air* pInstance = new CCriminal_MonsterNode_Air();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterNode_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterNode_Air::Free()
{
	__super::Free();
}
