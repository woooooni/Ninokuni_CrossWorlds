#include "stdafx.h"
#include "Shadow_ThiefNode_Air.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CShadow_ThiefNode_Air::CShadow_ThiefNode_Air()
{
}

HRESULT CShadow_ThiefNode_Air::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fAirTime = 1.5f;

	return S_OK;
}

void CShadow_ThiefNode_Air::Start()
{
}

CBTNode::NODE_STATE CShadow_ThiefNode_Air::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR))
		return NODE_STATE::NODE_FAIL;

	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground())
		{
			m_fTime += fTimeDelta;
			if (m_fTime >= m_fAirTime)
			{
				if (!m_bIsStand)
				{
					m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Knock_Finish"));
					m_bIsStand = true;
				}

				if (m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_ShadowThief.ao|ShadowThief_Knock_Finish"))
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

						return NODE_STATE::NODE_SUCCESS;
					}
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Air* CShadow_ThiefNode_Air::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CShadow_ThiefNode_Air* pInstance = new CShadow_ThiefNode_Air();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Air::Free()
{
	__super::Free();
}
