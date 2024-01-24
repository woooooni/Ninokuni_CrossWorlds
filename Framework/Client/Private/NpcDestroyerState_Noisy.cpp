#include "stdafx.h"
#include "NpcDestroyerState_Noisy.h"

#include "GameNpc.h"

#include "GameInstance.h"
#include "Animation.h"

CNpcDestroyerState_Noisy::CNpcDestroyerState_Noisy(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcDestroyerState_Noisy::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_vecSound.push_back(TEXT("Destroyer_V_Atk_Cast_Long_2.mp3"));
	m_vecSound.push_back(TEXT("Destroyer_V_Atk_Cast_Long_1.mp3"));
	m_vecSound.push_back(TEXT("Guardians_Destroyer_V_Atk_Medium_1.mp3"));

	return S_OK;
}

void CNpcDestroyerState_Noisy::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	GI->Play_Sound(m_vecSound[0], CHANNELID::SOUND_BOSS, 1.f, false);
}

void CNpcDestroyerState_Noisy::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_iAnimIndex += 1;
		if (m_iAnimIndex >= m_AnimIndices.size())
		{
			m_iAnimIndex = 0;
		}

		m_pModelCom->Set_Animation(m_AnimIndices[m_iAnimIndex]);
		GI->Play_Sound(m_vecSound[m_iAnimIndex], CHANNELID::SOUND_BOSS, 1.f, false);
	}
}

void CNpcDestroyerState_Noisy::Exit_State()
{
}

CNpcDestroyerState_Noisy* CNpcDestroyerState_Noisy::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDestroyerState_Noisy* pInstance = new CNpcDestroyerState_Noisy(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDestroyerState_Noisy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDestroyerState_Noisy::Free()
{
	__super::Free();
}

