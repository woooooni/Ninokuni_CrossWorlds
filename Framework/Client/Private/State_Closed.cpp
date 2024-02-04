#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\State_Closed.h"
#include "Player.h"
#include "Game_Manager.h"
#include "WitchWood.h"

#include "Quest_Manager.h"

CState_Closed::CState_Closed(CStateMachine* pMachine)
	: CState(pMachine)
{

}

HRESULT CState_Closed::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Closed::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[1]);
}

void CState_Closed::Tick_State(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	
	if (m_fTime >= 20.0f)
	{
		if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish() && m_AnimIndices[0] == m_pModelCom->Get_CurrAnimationIndex())
		{
			m_pModelCom->Set_Animation(m_AnimIndices[1]);
			m_fTime = 0.0f;
		}
		//else if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish() && m_AnimIndices[1] == m_pModelCom->Get_CurrAnimationIndex())
		//	m_pStateMachineCom->Change_State(CWitchWood::WOOD_STATE::OPEN);
	}

	CWitchWood* pWood = static_cast<CWitchWood*>(m_pOwner);
	_bool bClose = pWood->IsClose();

	if (true != bClose)
		m_pStateMachineCom->Change_State(CWitchWood::WOOD_STATE::OPEN);
}

void CState_Closed::Exit_State()
{
	m_fTime = 0.0f;
}

CState_Closed* CState_Closed::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Closed* pInstance = new CState_Closed(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Closed");
		Safe_Release<CState_Closed*>(pInstance);
	}

	return pInstance;
}

void CState_Closed::Free()
{
	__super::Free();
}
