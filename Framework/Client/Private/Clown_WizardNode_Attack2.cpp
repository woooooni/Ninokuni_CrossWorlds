#include "stdafx.h"
#include "Clown_WizardNode_Attack2.h"

#include "BehaviorTree.h"
#include "Model.h"

CClown_WizardNode_Attack2::CClown_WizardNode_Attack2()
{
}

HRESULT CClown_WizardNode_Attack2::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Attack2::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Attack02"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CClown_WizardNode_Attack2::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_ClownWizard.ao|ClownWizard_Attack02"), 1.f, fTimeDelta);
}

CClown_WizardNode_Attack2* CClown_WizardNode_Attack2::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClown_WizardNode_Attack2* pInstance = new CClown_WizardNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Attack2::Free()
{
	__super::Free();
}
