#include "stdafx.h"
#include "StelliaState_Laser.h"

#include "Stellia.h"
#include "Effect_Manager.h"

CStelliaState_Laser::CStelliaState_Laser(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Laser::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Laser::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill05_New"));
}

void CStelliaState_Laser::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
	}

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Laser"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Laser::Exit_State()
{
}

CStelliaState_Laser* CStelliaState_Laser::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Laser* pInstance = new CStelliaState_Laser(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Laser::Free()
{
	__super::Free();
}

