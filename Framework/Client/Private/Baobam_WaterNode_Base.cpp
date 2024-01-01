#include "stdafx.h"
#include "Baobam_WaterNode_Base.h"

CBaobam_WaterNode_Base::CBaobam_WaterNode_Base()
{
}

HRESULT CBaobam_WaterNode_Base::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Base::Start()
{
}

CBTNode::NODE_STATE CBaobam_WaterNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

CBTNode::NODE_STATE CBaobam_WaterNode_Base::Atk_BehaviorTick(_float fDestWaitTime, const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween() && !m_bWait)
	{
		m_bWait = true;
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_BattleStand"));
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE, true);
	}

	if (m_bWait)
	{
		if (Behavior_Wait(fDestWaitTime, fTimeDelta))
		{
			m_bIsSucces = true;
			m_bWait = false;

			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND, false);
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE, false);
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

_bool CBaobam_WaterNode_Base::Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta)
{
	m_fSourWaitTime += fTimeDelta;

	if (m_fSourWaitTime > fDestWaitTime)
	{
		m_fSourWaitTime = fDestWaitTime - m_fSourWaitTime;
		return true;
	}

	return false;
}

void CBaobam_WaterNode_Base::Free()
{
}
