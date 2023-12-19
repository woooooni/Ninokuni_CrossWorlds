#include "stdafx.h"
#include "Shadow_ThiefNode_Dead.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Dead::CShadow_ThiefNode_Dead()
{
}

HRESULT CShadow_ThiefNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Death"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Dead* CShadow_ThiefNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Dead* pInstance = new CShadow_ThiefNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Dead::Free()
{
	__super::Free();
}
