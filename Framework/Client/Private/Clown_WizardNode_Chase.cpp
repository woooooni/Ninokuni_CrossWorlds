#include "stdafx.h"
#include "Clown_WizardNode_Chase.h"

#include "BehaviorTree.h"


#include "Model.h"

CClown_WizardNode_Chase::CClown_WizardNode_Chase()
{
}

HRESULT CClown_WizardNode_Chase::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Chase::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_BattleRun"));
}

CBTNode::NODE_STATE CClown_WizardNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 3.5f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CClown_WizardNode_Chase* CClown_WizardNode_Chase::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClown_WizardNode_Chase* pInstance = new CClown_WizardNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Chase::Free()
{
	__super::Free();
}
