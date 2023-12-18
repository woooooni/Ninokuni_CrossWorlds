#include "stdafx.h"
#include "Clown_WizardNode_Dead.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClown_WizardNode_Dead::CClown_WizardNode_Dead()
{
}

HRESULT CClown_WizardNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Death"));
}

CBTNode::NODE_STATE CClown_WizardNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		// 여기서 객체 지우는 코드.
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CClown_WizardNode_Dead* CClown_WizardNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
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
