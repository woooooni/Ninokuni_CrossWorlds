#include "stdafx.h"
#include "GlanixState_Rage2Stamp.h"

#include "Glanix.h"

#include "Animation.h"
#include "Camera_Manager.h"
#include "GameInstance.h"

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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pGlanix->Get_OriginPos());
}

void CGlanixState_Rage2Stamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 15)
	{
		/* 남아있는 화염정령 제거 */
		for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_PROP))
		{
			if (iter->Get_PrototypeTag() == TEXT("Prorotype_GameObject_Glanix_Phoenix"))
			{
				iter->Set_Dead(true);
			}
		}

		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
	}
}

void CGlanixState_Rage2Stamp::Exit_State()
{
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

