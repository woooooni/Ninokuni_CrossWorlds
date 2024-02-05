#include "stdafx.h"
#include "NpcState_Idle.h"

#include "GameNpc.h"

#include "Animation.h"

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

