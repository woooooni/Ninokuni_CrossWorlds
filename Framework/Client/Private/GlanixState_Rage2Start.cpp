#include "stdafx.h"
#include "GlanixState_Rage2Start.h"

#include "Glanix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

CGlanixState_Rage2Start::CGlanixState_Rage2Start(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Start::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Rage2Start::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BossSkillRage"));
}

void CGlanixState_Rage2Start::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2WAVE);
	}
}

void CGlanixState_Rage2Start::Exit_State()
{
	//_float4 vOwnerPos = {};
	//XMStoreFloat4(&vOwnerPos, m_pGlanix->Get_OriginPos());
	//_vector vSpritPos = { vOwnerPos.x, vOwnerPos.y, vOwnerPos.z, 1.f };
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_Phoenix"));
}

CGlanixState_Rage2Start* CGlanixState_Rage2Start::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Start* pInstance = new CGlanixState_Rage2Start(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Start::Free()
{
	__super::Free();
}
