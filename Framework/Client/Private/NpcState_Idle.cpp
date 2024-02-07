#include "stdafx.h"
#include "NpcState_Idle.h"

#include "GameNpc.h"

#include "Animation.h"
#include "Ruby.h"
#include "RubyCarriage.h"
#include "GameInstance.h"

CNpcState_Idle::CNpcState_Idle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iIdleIndex = 1;
	m_fIdleTime = 5.f;

	return S_OK;
}

void CNpcState_Idle::Enter_State(void* pArg)
{
	if (nullptr != pArg)
	{
		m_pModelCom->Set_Animation(m_iCurrAnimIndex, MIN_TWEEN_DURATION);
		return;
	}

	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* 여분의 Idle 애니메이션이 있을 때에만. */
	if (m_AnimIndices.size() > 1)
	{
		if (m_bIsStane)
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= m_fIdleTime)
			{
				m_fTime = m_fIdleTime - m_fTime;
				m_bIsStane = false;
				m_pModelCom->Set_Animation(m_AnimIndices[m_iIdleIndex]);
			}
		}
		else
		{
			if (__super::State_Wait(m_pModelCom->Get_Animation(m_AnimIndices[m_iIdleIndex])->Is_Loop(), m_pModelCom->Get_Animation(m_AnimIndices[m_iIdleIndex])->Get_AnimationName(), 5.f, fTimeDelta))
			{
				m_iIdleIndex += 1;
				if (m_iIdleIndex >= m_AnimIndices.size())
				{
					/* 0은 Stand, 고로 1부터. */
					m_iIdleIndex = 1;
				}

				m_bIsStane = true;
				m_pModelCom->Set_Animation(m_AnimIndices[0]); // Stand로 
			}
		}
	}

	if (m_pOwner->Get_ObjectTag() == TEXT("Ruby") && LEVELID::LEVEL_WITCHFOREST == GI->Get_CurrentLevel()) // 에버모어에서도 사용해서, 마녀의 숲에서만 아래 코드 타도록 함
	{
		CRuby* pRuby = static_cast<CRuby*>(m_pOwner);
		CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(pRuby->Get_RidingObject());
	
		_bool bTake = pRubyCarriage->TakeTheCarriage();
		if (true == bTake)
		{
			if (true == m_pModelCom->Is_Finish() && false == m_pModelCom->Is_Tween())
				m_pStateMachineCom->Change_State(CGameNpc::NPC_STATE::NPC_UNIQUENPC_SEAT);
		}
		else if (true == pRuby->Get_QuestSection(CRuby::SECTION3))
		{
			if (true == m_pModelCom->Is_Finish() && false == m_pModelCom->Is_Tween())
				m_pStateMachineCom->Change_State(CGameNpc::NPC_STATE::NPC_UNIQUENPC_WALK);
		}
	}	
}

void CNpcState_Idle::Exit_State()
{
}

CNpcState_Idle* CNpcState_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_Idle* pInstance = new CNpcState_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_Idle::Free()
{
	__super::Free();
}

