#include "stdafx.h"
#include "StelliaState_Rage1Loop_SpinTail.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

CStelliaState_Rage1Loop_SpinTail::CStelliaState_Rage1Loop_SpinTail(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_SpinTail::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1Loop_SpinTail::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill01"));

	// Effect Create
	CTransform* pTransformCom = m_pStellia->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage01SpinTail"), pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage1Loop_SpinTail::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pDecal == nullptr)
	{
		CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.f, 0.f, 0.f), Vec3(20.f, 5.f, 20.f), Vec3(0.f, 0.f, 0.f), m_pStellia, &m_pDecal, false);
		Safe_AddRef(m_pDecal);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if (m_pDecal != nullptr)
		{
			m_pDecal->Set_Dead(true);
			Safe_Release(m_pDecal);
		}

		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_JUMPSTAMP);
	}
}

void CStelliaState_Rage1Loop_SpinTail::Exit_State()
{
}

CStelliaState_Rage1Loop_SpinTail* CStelliaState_Rage1Loop_SpinTail::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_SpinTail* pInstance = new CStelliaState_Rage1Loop_SpinTail(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_SpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_SpinTail::Free()
{
	__super::Free();
}

