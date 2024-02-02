#include "stdafx.h"
#include "StelliaState_SpawnRoar.h"

#include "Stellia.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

CStelliaState_SpawnRoar::CStelliaState_SpawnRoar(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpawnRoar::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpawnRoar::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));
}

void CStelliaState_SpawnRoar::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimationFrame() == 85 && !m_bIsFadeOut)
	{
		if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 3.f);
		m_bIsFadeOut = true;
	}

	if (m_bIsFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);
	}
}

void CStelliaState_SpawnRoar::Exit_State()
{
}

CStelliaState_SpawnRoar* CStelliaState_SpawnRoar::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpawnRoar* pInstance = new CStelliaState_SpawnRoar(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpawnRoar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpawnRoar::Free()
{
	__super::Free();
}

