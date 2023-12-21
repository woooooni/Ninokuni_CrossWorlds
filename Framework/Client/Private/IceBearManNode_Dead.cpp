#include "stdafx.h"
#include "IceBearManNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"

CIceBearManNode_Dead::CIceBearManNode_Dead()
{
}

HRESULT CIceBearManNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CIceBearManNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Death"));
}

CBTNode::NODE_STATE CIceBearManNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CIceBearManNode_Dead* CIceBearManNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CIceBearManNode_Dead* pInstance = new CIceBearManNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Dead::Free()
{
	__super::Free();
}
