#include "stdafx.h"
#include "StelliaNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"

CStelliaNode_Dead::CStelliaNode_Dead()
{
}

HRESULT CStelliaNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Death"));
}

CBTNode::NODE_STATE CStelliaNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		// 여기서 객체 지우는 코드.
	}

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Dead* CStelliaNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Dead* pInstance = new CStelliaNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Dead::Free()
{
	__super::Free();
}
