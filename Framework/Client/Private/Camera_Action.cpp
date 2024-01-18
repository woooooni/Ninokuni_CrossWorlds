#include "stdafx.h"
#include "Camera_Action.h"
#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Kuu.h"

#include "UI_Manager.h"

#include "Utils.h"


CCamera_Action::CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Action::CCamera_Action(const CCamera_Action& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Action::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Action::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Transform (Action Cam이 Lobby Level에서 가장 먼저 시작된다는 전제) */
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, m_tActionLobbyDesc.vCamPosition);

		Vec4 vLookAt = m_tActionLobbyDesc.vCamLookAtStart * m_tActionLobbyDesc.fLookMag;
		m_pTransformCom->LookAt(vLookAt.OneW());
	}

	return S_OK;
}

void CCamera_Action::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	/* Check Blending */
	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}

	/* Action Tick */
	if (m_bAction)
	{
		switch (m_eCurActionType)
		{
		case CCamera_Action::LOBBY:
			Tick_Lobby(fTimeDelta);
			break;
		case CCamera_Action::DOOR:
			Tick_Door(fTimeDelta);
			break;
		case CCamera_Action::TALK:
			Tick_Talk(fTimeDelta);
			break;
		default:
			break;
		}
	}

	/* Test */
	Test(fTimeDelta);
}

void CCamera_Action::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

Vec4 CCamera_Action::Get_LookAt()
{
	switch (m_eCurActionType)
	{
	case CAMERA_ACTION_TYPE::LOBBY:
	{
	
	}
	break;
	case CAMERA_ACTION_TYPE::DOOR:
	{
		CTransform* pTargetTransform = m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform");

		const Vec4 vCamLookAt = pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW()
			+ (Vec4)pTargetTransform->Get_Position();

		return vCamLookAt;
	}
	break;
	case CAMERA_ACTION_TYPE::TALK:
	{
		return m_tActionTalkDesc.vPrevLookAt.OneW();
	}
	break;
	default:
		break;
	}
	return Vec4();
}

HRESULT CCamera_Action::Start_Action_Lobby()
{
	m_eCurActionType = CAMERA_ACTION_TYPE::LOBBY;

	m_bAction = true;

	m_tProjDesc.tLerpFov.fCurValue = Cam_Fov_Action_Lobby;

	m_tActionLobbyDesc.vLerpCamLookAt.Start(
		m_tActionLobbyDesc.vCamLookAtStart, 
		m_tActionLobbyDesc.vCamLookAtFinish, 
		m_tActionLobbyDesc.fLerpTime, 
		LERP_MODE::SMOOTHER_STEP);

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Talk(CGameObject* pNpc1, CGameObject* pNpc2)
{
	/* 정보 세팅 */
	{
		CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		if (nullptr == pPlayer)
			return E_FAIL;

		pPlayer->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);

		if (!m_tActionTalkDesc.bInit)
		{
			m_tActionTalkDesc.pKuu = CGame_Manager::GetInstance()->Get_Kuu();
			if (nullptr == m_tActionTalkDesc.pKuu)
				return E_FAIL;

			m_tActionTalkDesc.pTransform_Kuu = m_tActionTalkDesc.pKuu->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransform_Kuu)
				return E_FAIL;


			m_tActionTalkDesc.pTransform_Player = pPlayer->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransform_Player)
				return E_FAIL;

			m_tActionTalkDesc.bInit = true;
		}
		
		if (nullptr != pNpc1)
		{
			m_tActionTalkDesc.pTransformNpc1 = pNpc1->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransformNpc1)
				return E_FAIL;
		}

		if (nullptr != pNpc2)
		{
			m_tActionTalkDesc.pTransformNpc2 = pNpc2->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransformNpc2)
				return E_FAIL;
		}
	}

	/* 쿠우 + 플레이어*/
	if (nullptr == pNpc1)
	{
		Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::KUU_AND_PLAYER);
		return S_OK;
	}
	else if (nullptr != pNpc1) /* 쿠우 + 플레이어 + Npc */
		Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::ALL);

	//Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::NPC1); /* 구현한 다음에 살린다. */

	return S_OK;
}

HRESULT CCamera_Action::Change_Action_Talk_Object(const ACTION_TALK_DESC::VIEW_TYPE& eType)
{
	if (nullptr == m_tActionTalkDesc.pTransform_Kuu || nullptr == m_tActionTalkDesc.pTransform_Player)
		return E_FAIL;

	Set_Talk_Transform(eType);

	return S_OK;
}

HRESULT CCamera_Action::Finish_Action_Talk()
{
	m_tActionTalkDesc.Clear();

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Door()
{
	CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (nullptr == pPlayer)
		return E_FAIL;

	/* 팔로우 카메라 와이드 뷰 세팅 풀어주기 */
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pFollowCam)
		pFollowCam->Reset_WideView_To_DefaultView();

	m_eCurActionType = CAMERA_ACTION_TYPE::DOOR;

	m_bAction = true;

	m_pTargetObj = m_pLookAtObj = pPlayer;

	m_tProjDesc.tLerpFov.fCurValue = Cam_Fov_Default;

	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	/* Cam Positon */
	m_tActionDoorDesc.tLerpDistance.Start(3.5f, 4.25f, m_tActionDoorDesc.fDistanceLerpTime, LERP_MODE::EASE_OUT);
	m_tActionDoorDesc.vTargetOffset.z = m_tActionDoorDesc.tLerpDistance.fCurValue;

	m_tTargetOffset.vCurVec = m_tActionDoorDesc.vTargetOffset;

	const Vec4 vCamTargetPosition = pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec).ZeroW()
								+ (Vec4)pTargetTransform->Get_Position();

	/* Cam LookAt */
	m_tLookAtOffset.vCurVec = m_tActionDoorDesc.vLookAtOffset;

	const Vec4 vCamLookAt = pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW()
							+ (Vec4)pTargetTransform->Get_Position();

	/* Set */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamTargetPosition);
	m_pTransformCom->LookAt(vCamLookAt);

	/* Progress */
	m_tActionDoorDesc.eProgress = ACTION_DOOR_DESC::PROGRESS::DELAY;

	/* Ui Off */
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

	/* 플레이어 모든 인풋을 막는다. */
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

	return S_OK;
}

void CCamera_Action::Tick_Lobby(_float fTimeDelta)
{
	if (!m_tActionLobbyDesc.vLerpCamLookAt.bActive)
		m_bAction = false;

	if (m_tActionLobbyDesc.vLerpCamLookAt.bActive)
		m_tActionLobbyDesc.vLerpCamLookAt.Update_Lerp(fTimeDelta);
	
	Vec4 vLookAt = m_tActionLobbyDesc.vLerpCamLookAt.vCurVec * m_tActionLobbyDesc.fLookMag;

	m_pTransformCom->LookAt(vLookAt.OneW());

	Vec4 vPos = m_tActionLobbyDesc.vCamPosition + Vec4(m_tTargetOffset.vCurVec);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos.OneW());
}

void CCamera_Action::Tick_Door(_float fTimeDelta)
{
	/* Progress */
	{
		if (m_tActionDoorDesc.tLerpRotateSpeed.bActive)
			m_tActionDoorDesc.tLerpRotateSpeed.Update(fTimeDelta);

		switch (m_tActionDoorDesc.eProgress)
		{
		case tagActionDoorDesc::PROGRESS::DELAY:
		{
			/* Check Progress */
			m_tActionDoorDesc.fAcc += fTimeDelta;
			
			if (m_tActionDoorDesc.fDelayTime <= m_tActionDoorDesc.fAcc)
			{
				m_tActionDoorDesc.fAcc = 0.f;

				m_tActionDoorDesc.tLerpRotateSpeed.Start(
					0.f,
					m_tActionDoorDesc.fMaxRotateSpeed,
					m_tActionDoorDesc.fBlendingTime,
					LERP_MODE::EASE_IN);

				m_tActionDoorDesc.eProgress = tagActionDoorDesc::PROGRESS::INTRO;
			}
		}
		break;
		case tagActionDoorDesc::PROGRESS::INTRO:
		{
			/* Check Progress */
			if (!m_tActionDoorDesc.tLerpRotateSpeed.bActive)
				m_tActionDoorDesc.eProgress = tagActionDoorDesc::PROGRESS::FIX;
		}
		break;
		case tagActionDoorDesc::PROGRESS::FIX:
		{
			/* Check Progress */
			m_tActionDoorDesc.fAcc += fTimeDelta;
			if (m_tActionDoorDesc.fFixedTime <= m_tActionDoorDesc.fAcc)
			{
				m_tActionDoorDesc.eProgress = tagActionDoorDesc::PROGRESS::OUTTRO;

				m_tActionDoorDesc.tLerpRotateSpeed.Start(
					m_tActionDoorDesc.fMaxRotateSpeed,
					0.f,
					m_tActionDoorDesc.fBlendingTime,
					LERP_MODE::EASE_OUT);
			}
		}
		break;
		case tagActionDoorDesc::PROGRESS::OUTTRO:
		{
			/* Check Progress */
			if (!m_tActionDoorDesc.tLerpRotateSpeed.bActive)
			{
				m_tActionDoorDesc.Clear();

				m_bAction = false;

				/* 다시 팔로우 카메라로 전환 */
				CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
				if (nullptr != pFollowCam)
				{
					pFollowCam->Set_Default_Position();

					CCamera_Manager::GetInstance()->Change_Camera(CAMERA_TYPE::FOLLOW);

					return;
				}
			}
		}
		break;
		default:
			break;
		}
	}

	/* Transform */
	{
		CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

		if (m_tActionDoorDesc.tLerpDistance.bActive)
		{
			m_tActionDoorDesc.tLerpDistance.Update(fTimeDelta);

			/* Cam Positon */

			m_tActionDoorDesc.vTargetOffset.z = m_tActionDoorDesc.tLerpDistance.fCurValue;

			m_tTargetOffset.vCurVec = m_tActionDoorDesc.vTargetOffset;

			const Vec4 vCamTargetPosition = pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec).ZeroW()
				+ (Vec4)pTargetTransform->Get_Position();

			m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamTargetPosition);
		}

		/* Rotation */
		m_pTransformCom->RevolutionRotation(pTargetTransform->Get_Position(), Vec3::Up, m_tActionDoorDesc.tLerpRotateSpeed.fCurValue * fTimeDelta);

		/* Height */
		Vec4 vPostion = m_pTransformCom->Get_Position();
		vPostion.y += m_tActionDoorDesc.tLerpRotateSpeed.fCurValue * fTimeDelta * 1.25f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPostion);

		/* Look At */
		const Vec4 vCamLookAt = pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW()
								+ (Vec4)pTargetTransform->Get_Position();

		/* Set */
		m_pTransformCom->LookAt(vCamLookAt);
	}
}

void CCamera_Action::Tick_Talk(_float fTimeDelta)
{
	
}

void CCamera_Action::Tick_Blending(const _float fDeltaTime)
{
}

void CCamera_Action::Test(_float fTimeDelta)
{
	if (KEY_TAP(KEY::HOME))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}
}

void CCamera_Action::Set_Talk_Transform(const ACTION_TALK_DESC::VIEW_TYPE& eType)
{
	Vec4 vCamPosition, vCamLookAt;

	switch (eType)
	{
	case CCamera_Action::tagActionTalkDesc::KUU:
	{
		
	}
		break;
	case CCamera_Action::tagActionTalkDesc::KUU_AND_PLAYER:
	{
		/* CamPosition */
		Vec4 vTargetOffSet = { 1.5f, 1.f, 3.f, 1.f };
		vCamPosition = m_tActionTalkDesc.pKuu->Get_GoalPosition() + m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vTargetOffSet);

		/* CamLookAt */
		Vec4 vLookAtOffset = { -1.f, 0.f, 0.f, 1.f };
		vCamLookAt = m_tActionTalkDesc.pKuu->Get_GoalPosition() + m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vLookAtOffset);

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Narrow);
	}
		break;
	case CCamera_Action::tagActionTalkDesc::NPC1:
	{
		
		
	}
		break;
	case CCamera_Action::tagActionTalkDesc::NPC2:
	{
		
	}
		break;
	case CCamera_Action::tagActionTalkDesc::ALL:
	{
		_float fDistanceToNpc = 0.f;

		Vec4 vPlayerKuuCenterPos = Vec4(m_tActionTalkDesc.pTransform_Kuu->Get_Position() + m_tActionTalkDesc.pTransform_Player->Get_Position()) * 0.5f;
		vPlayerKuuCenterPos.w = 1.f;

		Vec4 vDirToNpcLook = Vec4(m_tActionTalkDesc.pTransformNpc1->Get_Position()) - vPlayerKuuCenterPos;
		{
			fDistanceToNpc = vDirToNpcLook.Length();
			vDirToNpcLook.Normalize();
		}

		Vec4 vDirToNpcRight = XMVector3Cross(Vec3::UnitY, vDirToNpcLook.xyz());
		{
			vDirToNpcRight.Normalize();
		}

		Vec4 vCenterPosToNpc = m_tActionTalkDesc.pKuu->Get_GoalPosition() + (vDirToNpcLook.ZeroY() * fDistanceToNpc * 0.5f);
		{
			vCenterPosToNpc.w = 1.f;
		}

		/* CamPosition */
		vCamPosition = vCenterPosToNpc + (vDirToNpcRight.ZeroY() * fDistanceToNpc * 0.7f);
		vCamPosition.y += fDistanceToNpc * 0.1f;

		/* CamLookAt */
		vCamLookAt = vCenterPosToNpc;
		vCamLookAt.y += fDistanceToNpc * 0.05f;

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Default);

		/* 대화 시작 캐릭터 최초 룩 설정 */
		if (!m_tActionTalkDesc.bSet)
		{
			m_tActionTalkDesc.pTransformNpc1->LookAt_ForLandObject(vPlayerKuuCenterPos);
			m_tActionTalkDesc.pTransform_Player->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc1->Get_Position());
			m_tActionTalkDesc.pTransform_Kuu->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc1->Get_Position());

			m_tActionTalkDesc.bSet = true;
		}
	}
		break;
	default:
		break;
	}


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPosition.OneW());
	m_pTransformCom->LookAt(vCamLookAt.OneW());

	m_tActionTalkDesc.vPrevLookAt = vCamLookAt.OneW();
}

HRESULT CCamera_Action::Ready_Components()
{
	return S_OK;
}

CCamera_Action* CCamera_Action::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Action* pInstance = new CCamera_Action(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Action::Clone(void* pArg)
{
	CCamera_Action* pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Action::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
