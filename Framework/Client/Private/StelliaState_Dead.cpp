#include "stdafx.h"
#include "StelliaState_Dead.h"

#include "Stellia.h"

#include "Camera_Group.h"

#include "Quest_Manager.h"

CStelliaState_Dead::CStelliaState_Dead(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Death"));

	/* 스텔리아 죽는 컷신 실행 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		pActionCam->Start_Action_Stellia_Dead(m_pStellia);

		CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());
	}
}

void CStelliaState_Dead::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		/* 디졸브 보여주지 않고 바로 마녀 컷신으로 넘어가기 위함 */
		//CQuest_Manager::GetInstance()->Set_IsBossKill(true);
		//m_pStellia->Set_DissolveDuration(0.1f);

		m_pStellia->Reserve_Dead(true);
	}
}

void CStelliaState_Dead::Exit_State()
{
}

CStelliaState_Dead* CStelliaState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Dead* pInstance = new CStelliaState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Dead::Free()
{
	__super::Free();
}
