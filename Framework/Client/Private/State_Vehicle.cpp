#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Vehicle.h"
#include "UI_Manager.h"
#include "UI_Dialog_Window.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Vehicle.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

CState_Vehicle::CState_Vehicle(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_Vehicle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pVehicle)
		return E_FAIL;

	return S_OK;
}

void CState_Vehicle::Free()
{
	__super::Free();
}


void CState_Vehicle::Reset_Camera_WideToDeafult()
{
	/* 팔로우 카메라 와이드뷰 상태에서, 만약 플레이어가 스킬 혹은 공격을 사용한다면 디폴트뷰로 빠르게 전환한다. */

	CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
	if (nullptr != pCurCam && CAMERA_TYPE::FOLLOW)
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);
		if (nullptr != pFollowCam)
			pFollowCam->Reset_WideView_To_DefaultView(false, 0.5f);
	}
}

