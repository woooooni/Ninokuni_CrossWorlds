#include "stdafx.h"
#include "IceBearManNode_Blow.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CIceBearManNode_Blow::CIceBearManNode_Blow()
{
}

HRESULT CIceBearManNode_Blow::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowTime = 1.5f;

	return S_OK;
}

void CIceBearManNode_Blow::Start()
{
}

CBTNode::NODE_STATE CIceBearManNode_Blow::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOW))
		return NODE_STATE::NODE_FAIL;

	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground())
		{
			m_fTime += fTimeDelta;
			if (m_fTime >= m_fBlowTime)
			{
				if (!m_bIsStand)
				{
					m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_KnockUp_End"));
					m_bIsStand = true;
				}

				if (m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_IceBearMan_Water.ao|IceBearMan_KnockUp_End"))
				{
					if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
					{
						dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOW, false);
						dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);

						if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
							dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
						else
							dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

						m_fTime = m_fBlowTime - m_fTime;
						m_bIsStand = false;

						return NODE_STATE::NODE_SUCCESS;
					}
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CIceBearManNode_Blow* CIceBearManNode_Blow::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CIceBearManNode_Blow* pInstance = new CIceBearManNode_Blow();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Blow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Blow::Free()
{
	__super::Free();
}
