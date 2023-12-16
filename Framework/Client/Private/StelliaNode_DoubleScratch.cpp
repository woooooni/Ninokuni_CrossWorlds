#include "stdafx.h"
#include "StelliaNode_DoubleScratch.h"

CStelliaNode_DoubleScratch::CStelliaNode_DoubleScratch()
{
}

HRESULT CStelliaNode_DoubleScratch::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_DoubleScratch::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Attack02"));
}

CBTNode::NODE_STATE CStelliaNode_DoubleScratch::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "atk2" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_DoubleScratch* CStelliaNode_DoubleScratch::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_DoubleScratch* pInstance = new CStelliaNode_DoubleScratch();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_DoubleScratch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_DoubleScratch::Free()
{
	__super::Free();
}
