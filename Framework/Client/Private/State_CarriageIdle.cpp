#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\State_CarriageIdle.h"
#include "Player.h"
#include "Game_Manager.h"
#include "WitchWood.h"

#include "RubyCarriage.h"

CState_CarriageIdle::CState_CarriageIdle(CStateMachine* pMachine)
	: CState(pMachine)
{

}

HRESULT CState_CarriageIdle::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_CarriageIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	_float fCamDistance = m_pModelCom->Get_Owner()->Get_CamDistance();
	GI->Stop_Sound(CHANNELID::SOUND_VOICE_WITCH_QUEST);
	//GI->Play_Sound(TEXT("Witch_Carriage.mp3"), CHANNELID::SOUND_VOICE_WITCH_QUEST, 1.0f, false, fCamDistance);
}

void CState_CarriageIdle::Tick_State(_float fTimeDelta)
{
	CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(m_pOwner);
	if (nullptr == pRubyCarriage)
		return;

	_bool bTake = pRubyCarriage->TakeTheCarriage();
	if (true == bTake)
	{
		if(true == m_pModelCom->Is_Finish() && false == m_pModelCom->Is_Tween())
			m_pStateMachineCom->Change_State(CRubyCarriage::STATE_TYPE::STATE_MOVE);
	}


}

void CState_CarriageIdle::Exit_State()
{
	m_fTime = 0.0f;
}

CState_CarriageIdle* CState_CarriageIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_CarriageIdle* pInstance = new CState_CarriageIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_CarriageIdle");
		Safe_Release<CState_CarriageIdle*>(pInstance);
	}

	return pInstance;
}

void CState_CarriageIdle::Free()
{
	__super::Free();
}
