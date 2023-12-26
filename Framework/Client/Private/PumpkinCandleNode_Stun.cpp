#include "stdafx.h"
#include "PumpkinCandleNode_Stun.h"

#include "BehaviorTree.h"

#include "GameInstance.h"
#include "Model.h"

CPumpkinCandleNode_Stun::CPumpkinCandleNode_Stun()
{
}

HRESULT CPumpkinCandleNode_Stun::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CPumpkinCandleNode_Stun::Start()
{
}

CBTNode::NODE_STATE CPumpkinCandleNode_Stun::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return NODE_STATE::NODE_FAIL;

	// 스턴 시 피격
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT))
	{
		GI->Play_Sound(TEXT("../Bin/Resources/Sound/Monster/PumpkinCandle/PumpkinCandle_V_Dmg_1_2.ogg"), CHANNELID::SOUND_VOICE_MONSTER1, 0.4f, true);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_StunTime())
	{
		m_fTime = dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_StunTime() - m_fTime;

		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, false);

		if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
		else
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Stun* CPumpkinCandleNode_Stun::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
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
