#include "stdafx.h"
#include "Shadow_ThiefNode_Hit.h"

#include "BehaviorTree.h"


#include "Model.h"

CShadow_ThiefNode_Hit::CShadow_ThiefNode_Hit()
{
}

HRESULT CShadow_ThiefNode_Hit::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Hit::Start()
{
}

CBTNode::NODE_STATE CShadow_ThiefNode_Hit::Tick(const _float& fTimeDelta)
{
	// 공격 시 체력 감소
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK) &&
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);
		return NODE_STATE::NODE_SUCCESS;
	}

	// 애니메이션 재생
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK) &&
		!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Damage"));
	}

	if (m_tBTMonsterDesc.pOwnerModel->Is_Fix() && m_tBTMonsterDesc.pOwnerModel->Is_Finish())
	{
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);

		return __super::Atk_BehaviorTick(1.f, fTimeDelta);

		// return NODE_STATE::NODE_SUCCESS;
	}

	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Hit* CShadow_ThiefNode_Hit::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CShadow_ThiefNode_Hit* pInstance = new CShadow_ThiefNode_Hit();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Hit::Free()
{
	__super::Free();
}
