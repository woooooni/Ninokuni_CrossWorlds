#include "stdafx.h"
#include "StelliaState_Rage3ClawEndBreak.h"

#include "Stellia.h"

#include "Animation.h"

CStelliaState_Rage3ClawEndBreak::CStelliaState_Rage3ClawEndBreak(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3ClawEndBreak::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3ClawEndBreak::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_End"));
	m_tChargeLerp.Start(m_fSprintSpeed, 0.f, 1.f, LERP_MODE::EASE_IN);
}

void CStelliaState_Rage3ClawEndBreak::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float fCurSpeed = 0.f;

	if (m_tChargeLerp.bActive)
	{
		m_tChargeLerp.Update(fTimeDelta);
		fCurSpeed = m_tChargeLerp.fCurValue;
	}

	if (m_pModelCom->Get_CurrAnimationFrame() <= 22)
	{
		Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();

		cout << "Speed " << fCurSpeed << endl;
		cout << "Dist " << fabs(vCenterToStellia.Length()) << endl;

		if(fabs(vCenterToStellia.Length()) < m_fAroundDist)
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), fCurSpeed, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		_bool bIsRight = true;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_AROUND, &bIsRight);
	}

}

void CStelliaState_Rage3ClawEndBreak::Exit_State()
{
}

CStelliaState_Rage3ClawEndBreak* CStelliaState_Rage3ClawEndBreak::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3ClawEndBreak* pInstance = new CStelliaState_Rage3ClawEndBreak(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3ClawEndBreak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3ClawEndBreak::Free()
{
	__super::Free();
}

