#include "stdafx.h"
#include "IceBearManNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CIceBearManNode_Chase::CIceBearManNode_Chase()
{
}

HRESULT CIceBearManNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CIceBearManNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_BattleRun"));
}

CBTNode::NODE_STATE CIceBearManNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 6.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CIceBearManNode_Chase* CIceBearManNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CIceBearManNode_Chase* pInstance = new CIceBearManNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Chase::Free()
{
	__super::Free();
}
