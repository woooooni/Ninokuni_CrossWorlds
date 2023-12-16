#include "stdafx.h"
#include "StelliaNode_TailRoll.h"

CStelliaNode_TailRoll::CStelliaNode_TailRoll()
{
}

HRESULT CStelliaNode_TailRoll::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_TailRoll::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill01"));
}

CBTNode::NODE_STATE CStelliaNode_TailRoll::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "tailroll" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_TailRoll* CStelliaNode_TailRoll::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_TailRoll* pInstance = new CStelliaNode_TailRoll();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_TailRoll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_TailRoll::Free()
{
	__super::Free();
}
