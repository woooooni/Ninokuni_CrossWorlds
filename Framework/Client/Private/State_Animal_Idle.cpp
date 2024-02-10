#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Idle.h"
#include "Animation.h"
#include "Character_SwordMan.h"
#include "Player.h"
#include "Game_Manager.h"

CState_Animal_Idle::CState_Animal_Idle(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Idle::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
	
	SelectSound();
}

void CState_Animal_Idle::Tick_State(_float fTimeDelta)
{
	CPlayer* pObj = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pObj)
		return;
	
	CTransform* pTransform = pObj->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	Vec4 vPlayerPos = pTransform->Get_Position();
	Vec4 vthisPos = m_pTransformCom->Get_Position();

	Vec3 vDirection = vPlayerPos - vthisPos;
	_float fDistance = vDirection.Length();

	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();

	if (true == bLift)
	{
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_LIFT);
		m_iPrevAnim = CAnimals::STATE::STATE_LIFT;
	}
	else if (fDistance <= 5.0f && false == m_bExit)
	{
		if (m_iPrevAnim != CAnimals::STATE::STATE_LIFT)
		{
			m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_RUN);
			m_iPrevAnim = CAnimals::STATE::STATE_RUN;
		}
		else
		{
			if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
			{
				_int iRandomVal = GI->RandomInt(0, 1);

				switch (iRandomVal)
				{
				case 0:
					m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
					m_iPrevAnim = CAnimals::STATE::STATE_IDLE;
					break;
				case 1:
					m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_WALK);
					m_iPrevAnim = CAnimals::STATE::STATE_WALK;
					break;
				}
			}
		}
	}
	else if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
	{
		_int iRandomVal = GI->RandomInt(0, 1);

		switch (iRandomVal)
		{
		case 0:
			m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
			m_iPrevAnim = CAnimals::STATE::STATE_IDLE;
			break;
		case 1:
			m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_WALK);
			m_iPrevAnim = CAnimals::STATE::STATE_WALK;
			break;
		}
	}

}

void CState_Animal_Idle::Exit_State()
{
	if (true == m_bExit)
	{
		m_bExit = false;
		return;
	}

	m_bExit = true;
}

void CState_Animal_Idle::SelectSound()
{
	wstring strobjtag = m_pOwner->Get_ObjectTag();
	Vec4 vCamPos = GI->Get_CamPosition();
	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vDistVector = vPos - vCamPos;
	_float fDist = vDistVector.Length();

	if (TEXT("Animal_Cat") == strobjtag)
	{
		// Test Cat
		GI->Play_Sound(TEXT("Cat_V_Idle_1_2.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);

	}
	else if (TEXT("Animal_Dochi") == strobjtag)
	{
		GI->Play_Sound(TEXT("Dochi_V_Idle_1_1.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_DuckGoo") == strobjtag)
	{
		GI->Play_Sound(TEXT("Duckgoo_V_Idle_3.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Fox") == strobjtag)
	{
		GI->Play_Sound(TEXT("FennecFox_V_Idle_1_2.mp3"), CHANNELID::SOUND_VOICE_ANIMAL, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Rabbit") == strobjtag)
	{
		GI->Play_Sound(TEXT("Rabbit_V_Idle_1-4.mp3"), CHANNELID::SOUND_VOICE_ANIMAL2, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_PolarBear") == strobjtag)
	{
		GI->Play_Sound(TEXT("PolarBear_V_Idle_1_2_2.mp3"), CHANNELID::SOUND_VOICE_ANIMAL3, 1.0f, true, fDist);
	}
	else if (TEXT("Animal_Ermine") == strobjtag)
	{

	}
	else if (TEXT("Animal_WelshCorgi") == strobjtag)
	{

	}

}

CState_Animal_Idle* CState_Animal_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Idle* pInstance = new CState_Animal_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Idle");
		Safe_Release<CState_Animal_Idle*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Idle::Free()
{
	__super::Free();
}
