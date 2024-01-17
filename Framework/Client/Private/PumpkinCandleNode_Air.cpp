#include "stdafx.h"
#include "PumpkinCandleNode_Air.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CPumpkinCandleNode_Air::CPumpkinCandleNode_Air()
{
}

HRESULT CPumpkinCandleNode_Air::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Air::Start()
{
}

CBTNode::NODE_STATE CPumpkinCandleNode_Air::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR))
		return NODE_STATE::NODE_FAIL;

	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_AIR, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);

		if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
		else
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Air* CPumpkinCandleNode_Air::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CPumpkinCandleNode_Air* pInstance = new CPumpkinCandleNode_Air();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Air::Free()
{
	__super::Free();
}
