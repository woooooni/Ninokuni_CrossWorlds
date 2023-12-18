#include "stdafx.h"
#include "Clown_WizardNode_Idle.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClown_WizardNode_Idle::CClown_WizardNode_Idle()
{
}

HRESULT CClown_WizardNode_Idle::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CClown_WizardNode_Idle::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CClown_WizardNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_BattleStand"));
	}

	if (m_bWait)
	{
		if (Behavior_Wait(2.f, fTimeDelta))
		{
			m_bWait = false;
			m_iCurAnimationIndex += 1;

			if (m_iCurAnimationIndex == m_vecAnimationName.size())
				m_iCurAnimationIndex = 0;

			return NODE_STATE::NODE_SUCCESS;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CClown_WizardNode_Idle* CClown_WizardNode_Idle::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	CClown_WizardNode_Idle* pInstance = new CClown_WizardNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Idle::Free()
{
	__super::Free();
}
