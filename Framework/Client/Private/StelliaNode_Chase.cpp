#include "stdafx.h"
#include "StelliaNode_Chase.h"

CStelliaNode_Chase::CStelliaNode_Chase()
{
}

HRESULT CStelliaNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Run"));
}

CBTNode::NODE_STATE CStelliaNode_Chase::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "run" << endl;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Chase* CStelliaNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Chase* pInstance = new CStelliaNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Chase::Free()
{
	__super::Free();
}
