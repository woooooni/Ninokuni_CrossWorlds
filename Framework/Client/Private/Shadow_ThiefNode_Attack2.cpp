#include "stdafx.h"
#include "Shadow_ThiefNode_Attack2.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Attack2::CShadow_ThiefNode_Attack2()
{
}

HRESULT CShadow_ThiefNode_Attack2::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Attack2::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Attack02"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Attack2::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Attack2* CShadow_ThiefNode_Attack2::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Attack2* pInstance = new CShadow_ThiefNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Attack2::Free()
{
	__super::Free();
}
