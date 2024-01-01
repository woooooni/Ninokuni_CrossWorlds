#include "stdafx.h"
#include "Clown_WizardNode_Dead.h"

#include "BehaviorTree.h"


#include "Model.h"

CClown_WizardNode_Dead::CClown_WizardNode_Dead()
{
}

HRESULT CClown_WizardNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Dead::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Death"));
}

CBTNode::NODE_STATE CClown_WizardNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CClown_WizardNode_Dead* CClown_WizardNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClown_WizardNode_Dead* pInstance = new CClown_WizardNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Dead::Free()
{
	__super::Free();
}
