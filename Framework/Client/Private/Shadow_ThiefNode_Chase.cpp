#include "stdafx.h"
#include "Shadow_ThiefNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CShadow_ThiefNode_Chase::CShadow_ThiefNode_Chase()
{
}

HRESULT CShadow_ThiefNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Run"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 6.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Chase* CShadow_ThiefNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Chase* pInstance = new CShadow_ThiefNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Chase::Free()
{
	__super::Free();
}
