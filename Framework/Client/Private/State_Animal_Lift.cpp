#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Lift.h"
#include "Animation.h"
#include "Character_SwordMan.h"
#include "Player.h"
#include "Game_Manager.h"

CState_Animal_Lift::CState_Animal_Lift(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Lift::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Lift::Enter_State(void* pArg)
{
	m_fNextTime = 0.0f;
	m_iCurrAnimIndex = m_AnimIndices[0];
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	m_pRigidBodyCom->Set_Use_Gravity(false);
	m_pRigidBodyCom->Set_Velocity({ 0.f, 0.f, 0.f });
	m_pRigidBodyCom->Set_Ground(true);

	SelectSound();
}

void CState_Animal_Lift::Tick_State(_float fTimeDelta)
{
	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();
	if (false == bLift)
	{
		SelectLiftFinishSound();
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
		m_pRigidBodyCom->Set_Use_Gravity(true);
		m_pRigidBodyCom->Set_Ground(false);
		return;
	}

	m_fNextTime += fTimeDelta;

	
	if (m_fNextTime >= m_fChangeMotionTime)
	{
		m_fNextTime = 0.0f;
		m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 2)];

		m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	}

	
}

void CState_Animal_Lift::Exit_State()
{
	m_bFirstLift = true;
}

void CState_Animal_Lift::SelectSound()
{
	wstring strobjtag = m_pOwner->Get_ObjectTag();
	Vec4 vCamPos = GI->Get_CamPosition();
	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vDistVector = vPos - vCamPos;
	_float fDist = vDistVector.Length();

	if (TEXT("Animal_Cat") == strobjtag)
	{
		// Test Cat
		GI->Play_Sound(TEXT("Cat_V_LiftedStart_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);

	}
	else if (TEXT("Animal_Dochi") == strobjtag)
	{
		GI->Play_Sound(TEXT("Dochi_V_Liftted_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_DuckGoo") == strobjtag)
	{
		GI->Play_Sound(TEXT("Duckgoo_V_Liftted_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Fox") == strobjtag)
	{
		GI->Play_Sound(TEXT("FennecFox_V_LiftedStart_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Rabbit") == strobjtag)
	{
		GI->Play_Sound(TEXT("Rabbit_V_LiftedStart_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_PolarBear") == strobjtag)
	{
		GI->Play_Sound(TEXT("PolarBear_V_LiftedStart_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Ermine") == strobjtag)
	{

	}
	else if (TEXT("Animal_WelshCorgi") == strobjtag)
	{

	}
}

void CState_Animal_Lift::SelectLiftFinishSound()
{
	wstring strobjtag = m_pOwner->Get_ObjectTag();
	Vec4 vCamPos = GI->Get_CamPosition();
	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vDistVector = vPos - vCamPos;
	_float fDist = vDistVector.Length();

	if (TEXT("Animal_Cat") == strobjtag)
	{
		// Test Cat
		GI->Play_Sound(TEXT("Cat_V_LiftedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);

	}
	else if (TEXT("Animal_Dochi") == strobjtag)
	{
		GI->Play_Sound(TEXT("Dochi_V_LifttedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_DuckGoo") == strobjtag)
	{
		GI->Play_Sound(TEXT("Duckgoo_V_LifttedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Fox") == strobjtag)
	{
		GI->Play_Sound(TEXT("FennecFox_V_LiftedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Rabbit") == strobjtag)
	{
		GI->Play_Sound(TEXT("Rabbit_V_LiftedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_PolarBear") == strobjtag)
	{
		GI->Play_Sound(TEXT("PolarBear_V_LiftedFinish_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Ermine") == strobjtag)
	{

	}
	else if (TEXT("Animal_WelshCorgi") == strobjtag)
	{

	}
}

CState_Animal_Lift* CState_Animal_Lift::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Lift* pInstance = new CState_Animal_Lift(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Lift");
		Safe_Release<CState_Animal_Lift*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Lift::Free()
{
	__super::Free();
}
