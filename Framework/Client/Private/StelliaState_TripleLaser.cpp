#include "stdafx.h"
#include "StelliaState_TripleLaser.h"

#include "Stellia.h"
#include "Effect_Manager.h"

CStelliaState_TripleLaser::CStelliaState_TripleLaser(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_TripleLaser::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_TripleLaser::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill03_New"));

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_TripleLaser"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_TripleLaser::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}
}

void CStelliaState_TripleLaser::Exit_State()
{
}

CStelliaState_TripleLaser* CStelliaState_TripleLaser::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_TripleLaser* pInstance = new CStelliaState_TripleLaser(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_TripleLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_TripleLaser::Free()
{
	__super::Free();
}

