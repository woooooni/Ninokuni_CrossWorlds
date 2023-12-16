#include "stdafx.h"
#include "StelliaNode_Boom.h"

CStelliaNode_Boom::CStelliaNode_Boom()
{
}

HRESULT CStelliaNode_Boom::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Boom::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill04_New"));
}

CBTNode::NODE_STATE CStelliaNode_Boom::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "boom" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;
	
	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Boom* CStelliaNode_Boom::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Boom* pInstance = new CStelliaNode_Boom();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Boom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Boom::Free()
{
	__super::Free();
}
