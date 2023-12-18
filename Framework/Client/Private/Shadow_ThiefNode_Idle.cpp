#include "stdafx.h"
#include "Shadow_ThiefNode_Idle.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Idle::CShadow_ThiefNode_Idle()
{
}

HRESULT CShadow_ThiefNode_Idle::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CShadow_ThiefNode_Idle::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CShadow_ThiefNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Stand"));
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

CShadow_ThiefNode_Idle* CShadow_ThiefNode_Idle::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName)
{
	CShadow_ThiefNode_Idle* pInstance = new CShadow_ThiefNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Idle::Free()
{
	__super::Free();
}
