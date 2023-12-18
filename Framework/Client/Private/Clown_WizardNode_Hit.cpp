#include "stdafx.h"
#include "Clown_WizardNode_Hit.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClown_WizardNode_Hit::CClown_WizardNode_Hit()
{
}

HRESULT CClown_WizardNode_Hit::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Hit::Start()
{
	// m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Damage"));
}

CBTNode::NODE_STATE CClown_WizardNode_Hit::Tick(const _float& fTimeDelta)
{
	// 공격 시 체력 감소
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT) &&
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);
		return NODE_STATE::NODE_SUCCESS;
	}

	// 애니메이션 재생
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT) &&
		!dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Damage"));
	}

	if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_ClownWizard.ao|ClownWizard_Damage"))
	{
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);

		// 하드코딩
		if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
			m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_BattleStand"));

		return NODE_STATE::NODE_SUCCESS;
	}

	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);

	return NODE_STATE::NODE_RUNNING;
}

CClown_WizardNode_Hit* CClown_WizardNode_Hit::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
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
