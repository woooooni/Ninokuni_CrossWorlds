#include "stdafx.h"
#include "Shadow_ThiefNode_Base.h"

CShadow_ThiefNode_Base::CShadow_ThiefNode_Base()
{
}

HRESULT CShadow_ThiefNode_Base::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Base::Start()
{
}

CBTNode::NODE_STATE CShadow_ThiefNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

CBTNode::NODE_STATE CShadow_ThiefNode_Base::UnLoop_BehaviorTick(const wstring& strAnimName, _float fDestWaitTime, const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTNodeDesc.pOwnerModel->Is_Fix() && m_tBTNodeDesc.pOwnerModel->Is_Finish() &&
		!m_bWait && m_tBTNodeDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == strAnimName)
	{
		m_bWait = true;
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Stand"));
	}

	if (m_bWait)
	{
		if (Behavior_Wait(fDestWaitTime, fTimeDelta))
		{
			m_bIsSucces = true;
			m_bWait = false;

			if (!dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			{
				dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);
				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

_bool CShadow_ThiefNode_Base::Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta)
{
	m_fSourWaitTime += fTimeDelta;

	if (m_fSourWaitTime > fDestWaitTime)
	{
		m_fSourWaitTime = fDestWaitTime - m_fSourWaitTime;
		return true;
	}

	return false;
}

void CShadow_ThiefNode_Base::Free()
{
}
