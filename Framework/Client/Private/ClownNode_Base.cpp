#include "stdafx.h"
#include "ClownNode_Base.h"

CClownNode_Base::CClownNode_Base()
{
}

HRESULT CClownNode_Base::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pBT);

	m_tBTMonsterDesc = *pDesc;

	return S_OK;
}

void CClownNode_Base::Start()
{
}

CBTNode::NODE_STATE CClownNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

CBTNode::NODE_STATE CClownNode_Base::Atk_BehaviorTick(_float fDestWaitTime, const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween() && !m_bWait)
	{
		m_bWait = true;
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_BattleStand"));
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE, true);
	}

	if (m_bWait)
	{
		if (Behavior_Wait(fDestWaitTime, fTimeDelta))
		{
			m_bIsSucces = true;
			m_bWait = false;

			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND, false);
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE, false);
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

_bool CClownNode_Base::Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta)
{
	m_fSourWaitTime += fTimeDelta;

	if (m_fSourWaitTime > fDestWaitTime)
	{
		m_fSourWaitTime = fDestWaitTime - m_fSourWaitTime;
		return true;
	}

	return false;
}

void CClownNode_Base::Free()
{
}
