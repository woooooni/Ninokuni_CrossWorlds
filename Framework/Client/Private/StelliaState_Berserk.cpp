#include "stdafx.h"
#include "StelliaState_Berserk.h"

#include "Stellia.h"
#include "Moon.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Vfx.h"
#include "Vfx_Stellia_Skill_Roar.h"

CStelliaState_Berserk::CStelliaState_Berserk(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Berserk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Berserk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));

	/* 연출 위해 포지션 중앙으로 세팅 */
	m_pTransformCom->Set_Position(m_pStellia->Get_OriginPos());

	// Effect Create
	CVfx* pVfxEffect = nullptr;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pStellia, &pVfxEffect);
	if (nullptr != pVfxEffect)
		pVfxEffect->Set_OwnerStateIndex((_int)CStellia::STELLIA_BERSERK);
}

void CStelliaState_Berserk::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_Berserk::Exit_State()
{
	// TODO Change to Moon
	CMoon* pMoon = static_cast<CMoon*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_SKYBOX, TEXT("Common_Moon")));
	if (nullptr == pMoon)
		return;

	pMoon->Set_RedColorStart(true);
}

CStelliaState_Berserk* CStelliaState_Berserk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Berserk* pInstance = new CStelliaState_Berserk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Berserk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Berserk::Free()
{
	__super::Free();
}

