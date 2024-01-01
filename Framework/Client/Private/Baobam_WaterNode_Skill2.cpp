#include "stdafx.h"
#include "Baobam_WaterNode_Skill2.h"

#include "BehaviorTree.h"


#include "Model.h"

CBaobam_WaterNode_Skill2::CBaobam_WaterNode_Skill2()
{
}

HRESULT CBaobam_WaterNode_Skill2::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Skill2::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Skill02"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CBaobam_WaterNode_Skill2::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(1.f, fTimeDelta);
}

CBaobam_WaterNode_Skill2* CBaobam_WaterNode_Skill2::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CBaobam_WaterNode_Skill2* pInstance = new CBaobam_WaterNode_Skill2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Skill2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Skill2::Free()
{
	__super::Free();
}
