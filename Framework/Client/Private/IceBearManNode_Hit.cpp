#include "stdafx.h"
#include "IceBearManNode_Hit.h"

#include "BehaviorTree.h"


#include "Model.h"

CIceBearManNode_Hit::CIceBearManNode_Hit()
{
}

HRESULT CIceBearManNode_Hit::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CIceBearManNode_Hit::Start()
{
}

CBTNode::NODE_STATE CIceBearManNode_Hit::Tick(const _float& fTimeDelta)
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
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Damage"));
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

CIceBearManNode_Hit* CIceBearManNode_Hit::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CIceBearManNode_Hit* pInstance = new CIceBearManNode_Hit();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Hit::Free()
{
	__super::Free();
}
