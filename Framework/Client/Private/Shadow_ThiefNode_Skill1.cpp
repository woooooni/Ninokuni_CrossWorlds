#include "stdafx.h"
#include "Shadow_ThiefNode_Skill1.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Skill1::CShadow_ThiefNode_Skill1()
{
}

HRESULT CShadow_ThiefNode_Skill1::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Skill1::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Skill01"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Skill1::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Skill1* CShadow_ThiefNode_Skill1::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Skill1* pInstance = new CShadow_ThiefNode_Skill1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Skill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Skill1::Free()
{
	__super::Free();
}
