#include "stdafx.h"
#include "StelliaState_Rage3Start_FadeIn.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

CStelliaState_Rage3Start_FadeIn::CStelliaState_Rage3Start_FadeIn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Start_FadeIn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3Start_FadeIn::Enter_State(void* pArg)
{
	/* 공중에서 떨어지지 않도록 y 포지션을 최대한 지면과 가깝게 설정 */
	Vec3 vPos = m_pStellia->Get_Rage3StartPos();
	vPos.y -= 2.5f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	__super::Set_LockOffStellia();
	m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_OriginPos());
	m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
	
	m_pPlayerTransform->Set_State(CTransform::STATE_POSITION, m_pStellia->Get_OriginPos());
	m_pPlayer->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pPlayerTransform->Get_Position());

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand"));

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);
}

void CStelliaState_Rage3Start_FadeIn::Tick_State(_float fTimeDelta)
{
	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_AROUND);
		}
	}
	else
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_AROUND);
	}

}

void CStelliaState_Rage3Start_FadeIn::Exit_State()
{
}

CStelliaState_Rage3Start_FadeIn* CStelliaState_Rage3Start_FadeIn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Start_FadeIn* pInstance = new CStelliaState_Rage3Start_FadeIn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Start_FadeIn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Start_FadeIn::Free()
{
	__super::Free();
}
