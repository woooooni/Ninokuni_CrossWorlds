#include "stdafx.h"
#include "Baobam_DarkNode_Idle.h"

#include "BehaviorTree.h"
#include "Model.h"

CBaobam_DarkNode_Idle::CBaobam_DarkNode_Idle()
{
}

HRESULT CBaobam_DarkNode_Idle::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CBaobam_DarkNode_Idle::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CBaobam_DarkNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Fix() && m_tBTMonsterDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_BattleStand"));
	}

	if (m_bWait)
	{
		if (Behavior_Wait(2.f, fTimeDelta))
		{
			m_bWait = false;
			m_iCurAnimationIndex += 1;

			if (m_iCurAnimationIndex == m_vecAnimationName.size())
				m_iCurAnimationIndex = 0;

			return NODE_STATE::NODE_SUCCESS;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_DarkNode_Idle* CBaobam_DarkNode_Idle::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	CBaobam_DarkNode_Idle* pInstance = new CBaobam_DarkNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Idle::Free()
{
	__super::Free();
}
