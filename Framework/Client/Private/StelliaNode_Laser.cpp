#include "stdafx.h"
#include "StelliaNode_Laser.h"

CStelliaNode_Laser::CStelliaNode_Laser()
{
}

HRESULT CStelliaNode_Laser::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Laser::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill05"));
}

CBTNode::NODE_STATE CStelliaNode_Laser::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "laser" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Laser* CStelliaNode_Laser::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Laser* pInstance = new CStelliaNode_Laser();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Laser::Free()
{
	__super::Free();
}
