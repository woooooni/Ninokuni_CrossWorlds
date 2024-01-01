#include "stdafx.h"
#include "IceBearManNode_Idle.h"

#include "BehaviorTree.h"


#include "Model.h"

CIceBearManNode_Idle::CIceBearManNode_Idle()
{
}

HRESULT CIceBearManNode_Idle::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecAnimationName = vecAnimationName;

	return S_OK;
}

void CIceBearManNode_Idle::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(m_vecAnimationName[m_iCurAnimationIndex]);
}

CBTNode::NODE_STATE CIceBearManNode_Idle::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Fix() && m_tBTMonsterDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == m_vecAnimationName[m_iCurAnimationIndex])
	{
		m_bWait = true;
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_BattleStand"));
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

CIceBearManNode_Idle* CIceBearManNode_Idle::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName)
{
	CIceBearManNode_Idle* pInstance = new CIceBearManNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecAnimationName)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Idle::Free()
{
	__super::Free();
}
