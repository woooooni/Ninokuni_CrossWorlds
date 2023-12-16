#include "stdafx.h"
#include "StelliaNode_Scratch.h"

CStelliaNode_Scratch::CStelliaNode_Scratch()
{
}

HRESULT CStelliaNode_Scratch::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Scratch::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Attack01"));
}

CBTNode::NODE_STATE CStelliaNode_Scratch::Tick(const _float& fTimeDelta)
{
	if(m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "atk1" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Scratch* CStelliaNode_Scratch::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Scratch* pInstance = new CStelliaNode_Scratch();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Scratch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Scratch::Free()
{
	__super::Free();
}
