#include "stdafx.h"
#include "StelliaNode_Stamp.h"

CStelliaNode_Stamp::CStelliaNode_Stamp()
{
}

HRESULT CStelliaNode_Stamp::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Stamp::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill02"));
}

CBTNode::NODE_STATE CStelliaNode_Stamp::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "stamp" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Stamp* CStelliaNode_Stamp::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Stamp* pInstance = new CStelliaNode_Stamp();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Stamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Stamp::Free()
{
	__super::Free();
}
