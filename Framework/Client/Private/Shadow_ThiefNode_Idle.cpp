#include "stdafx.h"
#include "Shadow_ThiefNode_Idle.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Idle::CShadow_ThiefNode_Idle()
{
}

HRESULT CShadow_ThiefNode_Idle::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Idle::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Stand"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Idle::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Idle* CShadow_ThiefNode_Idle::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Idle* pInstance = new CShadow_ThiefNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
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
