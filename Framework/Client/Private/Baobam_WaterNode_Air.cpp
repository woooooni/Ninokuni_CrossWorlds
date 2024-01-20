#include "stdafx.h"
#include "Baobam_WaterNode_Air.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CBaobam_WaterNode_Air::CBaobam_WaterNode_Air()
{
}

HRESULT CBaobam_WaterNode_Air::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fAirTime = 1.5f;

	return S_OK;
}

void CBaobam_WaterNode_Air::Start()
{
}

CBTNode::NODE_STATE CBaobam_WaterNode_Air::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR))
		return NODE_STATE::NODE_FAIL;

	// if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	if (!m_bIsStand && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimationFrame() == 62)
		m_tBTMonsterDesc.pOwnerModel->Set_Stop_Animation(true);

	if (m_tBTMonsterDesc.pOwnerModel->Is_Stop())
	{
		if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground())
		{
			m_fTime += fTimeDelta;
			if (m_fTime >= m_fAirTime)
			{
				if (!m_bIsStand)
				{
					m_tBTMonsterDesc.pOwnerModel->Set_Stop_Animation(false);
					m_bIsStand = true;
				}
			}
		}
	}

	if (m_bIsStand && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Baobam_Water.ao|BaoBam_KnockDown"))
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


	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Air* CBaobam_WaterNode_Air::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CBaobam_WaterNode_Air* pInstance = new CBaobam_WaterNode_Air();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Air::Free()
{
	__super::Free();
}
