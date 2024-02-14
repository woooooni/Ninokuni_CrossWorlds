#include "stdafx.h"
#include "StelliaState_SpawnIdle.h"

#include "Stellia.h"
#include "UI_Manager.h"
#include "UI_Fade.h"
CStelliaState_SpawnIdle::CStelliaState_SpawnIdle(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpawnIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpawnIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand"));
}

void CStelliaState_SpawnIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* 이름 UI를 띄우지 않으므로 컷신 후반을 자른다. */
	if (!m_bStartFadeOut && !m_pModelCom->Is_Tween() && 0.25f <= m_pModelCom->Get_Progress())
	{
		m_bStartFadeOut = true;
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);
	}

	if (m_bStartFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_SPAWNSTAND);
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);
	}
}

void CStelliaState_SpawnIdle::Exit_State()
{
}

CStelliaState_SpawnIdle* CStelliaState_SpawnIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpawnIdle* pInstance = new CStelliaState_SpawnIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpawnIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpawnIdle::Free()
{
	__super::Free();
}

