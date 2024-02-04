#include "stdafx.h"
#include "GlanixState_Rage2Stamp.h"

#include "Glanix.h"

#include "Animation.h"
#include "GameInstance.h"

#include "Camera_Follow.h"
#include "Camera_Manager.h"

#include "Character_Manager.h"
#include "Character.h"
#include "Player.h"

#include "UI_Manager.h"

#include "Effect_Manager.h"

CGlanixState_Rage2Stamp::CGlanixState_Rage2Stamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Stamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Rage2Stamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalStamp"));
	m_bDownEffectCreate = false;
}

void CGlanixState_Rage2Stamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() <= 13)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), 40.f, fTimeDelta);

	else if (m_pModelCom->Get_CurrAnimationFrame() == 15)
	{
		/* 남아있는 화염정령 제거 */
		for (auto iter : GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_PROP))
		{
			if (iter->Get_PrototypeTag() == TEXT("Prorotype_GameObject_Glanix_Phoenix"))
			{
				iter->Set_Dead(true);
			}
		}

		if (false == m_bDownEffectCreate)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_RageStamp"), m_pTransformCom->Get_WorldMatrix(), m_pGlanix);
			m_bDownEffectCreate = true;
		}
	}

	else if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Rage2Stamp::Exit_State()
{
	if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN))
	{
		CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN)->Set_Speed_Weight(1.f);
		CUI_Manager::GetInstance()->OnOff_IceVignette(false);
	}

	if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER))
	{
		CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER)->Set_Speed_Weight(1.f);
		CUI_Manager::GetInstance()->OnOff_IceVignette(false);
	}

	if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER))
	{
		CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER)->Set_Speed_Weight(1.f);
		CUI_Manager::GetInstance()->OnOff_IceVignette(false);
	}
}

CGlanixState_Rage2Stamp* CGlanixState_Rage2Stamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Stamp* pInstance = new CGlanixState_Rage2Stamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Stamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Stamp::Free()
{
	__super::Free();
}

