#include "stdafx.h"
#include "NpcDMWState_Following.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_Following::CNpcDMWState_Following(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Following::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fAttackCoolTime = 7.f;

	return S_OK;
}

void CNpcDMWState_Following::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_fAccTime = 0.f;
}

void CNpcDMWState_Following::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pWitch->Get_IsBattle())
	{
		if (m_pStellia != nullptr)
		{
			if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_JUMPSTAMP &&
				m_pStellia->Get_Component_Model()->Get_CurrAnimationFrame() > 25)
			{
				m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_VULCAN_READY);
			}
			else
			{
				__super::Following_Stellia(fTimeDelta);

				if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE1START &&
					m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE2START &&
					m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE3START_FADEOUT &&
					m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_RAGE1LOOP_EXPLOSION &&
					m_pStellia->Get_Component_StateMachine()->Get_CurrState() != CStellia::STELLIA_JUMPSTAMP)
				{
					m_fAccTime += fTimeDelta;
					if (m_fAccTime >= m_fAttackCoolTime)
					{
						m_fAccTime = m_fAttackCoolTime - m_fAccTime;

						if (m_iAtkIndex >= m_vecAtkState.size())
							m_iAtkIndex = 0;

						m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);

						//m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_ATTACK);
					}
				}
			}
		}
	}
}

void CNpcDMWState_Following::Exit_State()
{
}

CNpcDMWState_Following* CNpcDMWState_Following::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Following* pInstance = new CNpcDMWState_Following(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Following");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Following::Free()
{
	__super::Free();
}

