#include "stdafx.h"
#include "Baobam_DarkNode_Dead.h"

#include "BehaviorTree.h"

#include "Model.h"

CBaobam_DarkNode_Dead::CBaobam_DarkNode_Dead()
{
}

HRESULT CBaobam_DarkNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Dead::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Death"));
}

CBTNode::NODE_STATE CBaobam_DarkNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_DarkNode_Dead* CBaobam_DarkNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CBaobam_DarkNode_Dead* pInstance = new CBaobam_DarkNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Dead::Free()
{
	__super::Free();
}
