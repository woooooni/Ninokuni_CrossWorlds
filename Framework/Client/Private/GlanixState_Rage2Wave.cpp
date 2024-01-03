#include "stdafx.h"
#include "GlanixState_Rage2Wave.h"

#include "GameInstance.h"

#include "Glanix.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Glanix_ShockWave.h"

CGlanixState_Rage2Wave::CGlanixState_Rage2Wave(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Wave::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iMaxCount = 3;

	return S_OK;
}

void CGlanixState_Rage2Wave::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill06"));
}

void CGlanixState_Rage2Wave::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 여기서 슬로우 처리.
	//m_pGlanix->Get_SlowStack();

	if (m_pModelCom->Get_CurrAnimationFrame() == 50)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		//_float4 vOwnerPos = {};
		//XMStoreFloat4(&vOwnerPos, m_pGlanix->Get_OriginPos());
		//_vector vSpritPos = { vOwnerPos.x, vOwnerPos.y, vOwnerPos.z, 1.f };
		GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_ShockWave"), &m_pGlanix->Get_WavePoint());

	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if (m_iWaveCount >= m_iMaxCount)
		{
			m_iWaveCount = 0;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2RISING);
		}
		else
		{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2IDLE, &m_iWaveCount);
			m_iWaveCount += 1;
		}
	}
}

void CGlanixState_Rage2Wave::Exit_State()
{
}

CGlanixState_Rage2Wave* CGlanixState_Rage2Wave::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Wave* pInstance = new CGlanixState_Rage2Wave(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Wave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Wave::Free()
{
	__super::Free();
}

