#include "stdafx.h"
#include "QuestNode_Base.h"
#include "..\Public\QuestNode_Base.h"

#include "GameInstance.h"

#include "Camera_Group.h"

CQuestNode_Base::CQuestNode_Base()
{
}

HRESULT CQuestNode_Base::Initialize()
{
	return S_OK;
}

void CQuestNode_Base::Start()
{
}

CBTNode::NODE_STATE CQuestNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

void CQuestNode_Base::LateTick(const _float& fTimeDelta)
{
}

Vec4 CQuestNode_Base::Set_DestSpot(CGameObject* pGameObject)
{
	CTransform* pTransform = pGameObject->Get_Component<CTransform>(TEXT("Com_Transform"));
	Vec4 vSpotPos = Vec4(pTransform->Get_Position()) + (Vec4(pTransform->Get_Look()).Normalized() * 2.5f);

	return vSpotPos;
}

void CQuestNode_Base::Delete_QuestDestSpot()
{
	Safe_Release(m_pQuestDestSpot);
}

const _bool CQuestNode_Base::Is_Finish_LevelEnterCameraAction()
{
	/* 레벨 입장시 진행되는 카메라 액션이 끝났는지 체크한다. (플레이어 상태로 구분하면 안됨) */

	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return false;

	if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key()
		&& CCamera_Action::CAMERA_ACTION_TYPE::DOOR == pActionCam->Get_Camera_ActionType())
		return false;

	if (CCamera_Manager::GetInstance()->Is_Blending_Camera())
		return false;

	/* 이게 기존 플레이어 상태로 체크하던 코드 */
	//if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CurrentState() == CCharacter::STATE::NEUTRAL_DOOR_ENTER)
	//	return NODE_STATE::NODE_RUNNING;

	return true;
}

void CQuestNode_Base::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);

	Safe_Delete_Array(m_szpOwner);
	Safe_Delete_Array(m_szpTalk);
}

