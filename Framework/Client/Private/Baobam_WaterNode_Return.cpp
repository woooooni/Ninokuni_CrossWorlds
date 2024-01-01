#include "stdafx.h"
#include "Baobam_WaterNode_Return.h"

#include "BehaviorTree.h"


#include "Model.h"

CBaobam_WaterNode_Return::CBaobam_WaterNode_Return()
{
}

HRESULT CBaobam_WaterNode_Return::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Return::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_BattleRun"));
}

CBTNode::NODE_STATE CBaobam_WaterNode_Return::Tick(const _float& fTimeDelta)
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_OriginPos());
	m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 1.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Return* CBaobam_WaterNode_Return::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CBaobam_WaterNode_Return* pInstance = new CBaobam_WaterNode_Return();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Return::Free()
{
	__super::Free();
}
