#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\State_Open.h"
#include "Player.h"
#include "Game_Manager.h"
#include "WitchWood.h"

#include "Quest_Manager.h"

CState_Open::CState_Open(CStateMachine* pMachine)
	: CState(pMachine)
{

}

HRESULT CState_Open::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Open::Enter_State(void* pArg)
{
	if(CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_FINALBATTLE)
		m_pModelCom->Set_Animation(m_AnimIndices[1]);
	else
		m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Open::Tick_State(_float fTimeDelta)
{
	CWitchWood* pWood = static_cast<CWitchWood*>(m_pOwner);

	_bool bClose = pWood->IsClose();
	if (true == bClose)
		m_pStateMachineCom->Change_State(CWitchWood::WOOD_STATE::CLOSE);
}

void CState_Open::Exit_State()
{
	m_fTime = 0.0f;
}

CState_Open* CState_Open::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Open* pInstance = new CState_Open(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Open");
		Safe_Release<CState_Open*>(pInstance);
	}

	return pInstance;
}

void CState_Open::Free()
{
	__super::Free();
}
