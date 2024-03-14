#include "stdafx.h"
#include "StelliaState_Rage1Start.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Vfx.h"
#include "Vfx_Stellia_Skill_Roar.h"
#include "Effect.h"

CStelliaState_Rage1Start::CStelliaState_Rage1Start(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Start::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1Start::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));

	// Effect Create
	CVfx* pVfxEffect = nullptr;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pStellia, &pVfxEffect);
	if (nullptr != pVfxEffect)
		pVfxEffect->Set_OwnerStateIndex((_int)CStellia::STELLIA_RAGE1START);

	// 기존 방어막 삭제
	CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_EFFECT, L"Stellia_Skill_Rage01Shield");
	if (nullptr != pGameObject)
		pGameObject->Set_Dead(true);
}

void CStelliaState_Rage1Start::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_TURN);
	}
}

void CStelliaState_Rage1Start::Exit_State()
{
	// 방어막 생성
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage01Shield"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
	
	m_pStellia->Set_Rage01_UI(true);
}

CStelliaState_Rage1Start* CStelliaState_Rage1Start::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Start* pInstance = new CStelliaState_Rage1Start(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Start::Free()
{
	__super::Free();
}
