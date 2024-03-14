#include "stdafx.h"
#include "Clown_WizardNode_Hit.h"

#include "BehaviorTree.h"


#include "Model.h"

CClown_WizardNode_Hit::CClown_WizardNode_Hit()
{
}

HRESULT CClown_WizardNode_Hit::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Hit::Start()
{
	// m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Damage"));
}

CBTNode::NODE_STATE CClown_WizardNode_Hit::Tick(const _float& fTimeDelta)
{
	// 공격 시 체력 감소 (슈퍼 아머)
	//if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK) &&
	//	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	//{
	//	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);
	//	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);
	//	return NODE_STATE::NODE_SUCCESS;
	//}

	// 애니메이션 재생
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK))/* &&
		!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))*/
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Damage"));
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

CClown_WizardNode_Hit* CClown_WizardNode_Hit::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClown_WizardNode_Hit* pInstance = new CClown_WizardNode_Hit();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Hit::Free()
{
	__super::Free();
}
