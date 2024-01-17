#include "stdafx.h"
#include "PumpkinCandleNode_Stun.h"

#include "BehaviorTree.h"

#include "GameInstance.h"
#include "Model.h"

CPumpkinCandleNode_Stun::CPumpkinCandleNode_Stun()
{
}

HRESULT CPumpkinCandleNode_Stun::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Stun::Start()
{
}

CBTNode::NODE_STATE CPumpkinCandleNode_Stun::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return NODE_STATE::NODE_FAIL;

	m_fTime += fTimeDelta;

	if (m_fTime >= dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_StunTime())
	{
		m_fTime = dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_StunTime() - m_fTime;

		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);

		if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
		else
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Stun* CPumpkinCandleNode_Stun::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CPumpkinCandleNode_Stun* pInstance = new CPumpkinCandleNode_Stun();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Stun::Free()
{
	__super::Free();
}
