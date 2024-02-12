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
		case CCamera_Action::WINDMILL:
			Tick_WindMill(fTimeDelta);
			break;
		case CCamera_Action::SWORDMAN_BURST:
			Tick_SwordManBurst(fTimeDelta);
			break;
		case CCamera_Action::ENGINEER_BURST:
			Tick_EngineerBurst(fTimeDelta);
			break;
		case CCamera_Action::DESTROYER_BURST:
			Tick_DestroyerBurst(fTimeDelta);
			break;
		case CCamera_Action::STADIUM:
			Tick_Stadium(fTimeDelta);
			break;
		case CCamera_Action::ENDING:
			Tick_Ending(fTimeDelta);
			break;
		case CCamera_Action::WITCH_INVASION:
			Tick_Witch_Invasion(fTimeDelta);
			break;
		case CCamera_Action::WITCH_ROAR:
			Tick_Witch_Roar(fTimeDelta);
			break;
		case CCamera_Action::WITCH_AWAY:
			Tick_Witch_Away(fTimeDelta);
			break;
		case CCamera_Action::TOWER_DEFENSE:
			Tick_TowerDefense(fTimeDelta);
			break;
		case CCamera_Action::STELLIA_ROAR:
			Tick_Stellia_Roar(fTimeDelta);
			break;
		case CCamera_Action::STELLIA_DEAD:
			Tick_Stellia_Dead(fTimeDelta);
			break;
		default:
			break;
		}
	}
}

void CCamera_Action::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Action::Start_Action_Stadium(const _float& fDuration)
{	
	m_eCurActionType = CAMERA_ACTION_TYPE::STADIUM;
	
	/* 토크 이벤트 데이터 보관 */
	{
		memcpy(&m_tActionStadiumDesc.vPrevTalkLookAt, &m_pTransformCom->Get_LookAt(), sizeof(Vec4));
		memcpy(&m_tActionStadiumDesc.vPrevTalkPos, &m_pTransformCom->Get_Position(), sizeof(Vec4));
		m_tActionStadiumDesc.fPrevTalkFov = Get_Fov();
		Set_Fov(Cam_Fov_CurlingGame_Intro);
	}

	/* Time */
	{
		m_tActionStadiumDesc.fDurationPerView = fDuration / (_float)ACTION_STADIUM_DESC::VIEW_NUM::VIEW_NUM_END;
	}

	/* Transform */
	{	
		// 포지션을 먼저 세팅해주고 이후 룩앳을 세팅해야한다.
		m_pTransformCom->Set_Position(m_tActionStadiumDesc.ViewPositions[m_tActionStadiumDesc.iCurViewNum]);
		m_pTransformCom->Set_LookAtByDir(m_tActionStadiumDesc.ViewLooks[m_tActionStadiumDesc.iCurViewNum]);
	}

	/* Shake */
	{
		// Start_Shake(0.1f, 150.f, fDuration);
		// 쉐이킹 페이드 인아웃 예약
	}

	return S_OK;
}

HRESULT CCamera_Action::Finish_Action_Stadium()
{
	m_eCurActionType = CAMERA_ACTION_TYPE::TALK;

	/* 이전 데이터로 복귀*/
	{
		Set_Fov(m_tActionStadiumDesc.fPrevTalkFov);

		m_pTransformCom->Set_Position(m_tActionStadiumDesc.vPrevTalkPos.OneW());
		m_pTransformCom->LookAt(m_tActionStadiumDesc.vPrevTalkLookAt.OneW());
	}

	return S_OK;
}

void CCamera_Action::Set_TalkBackupDesc(class CTransform* pNpcTransform)
{
	if (nullptr == pNpcTransform)
		return;

	m_tActionTalkBackUpDesc.vOriginLookAt = pNpcTransform->Get_LookAt();

	m_tActionTalkBackUpDesc.vOriginPosition = pNpcTransform->Get_Position();
}

void CCamera_Action::Set_NpcTransformByBackupDesc(class CTransform* pNpcTransform)
{
	if (nullptr == pNpcTransform)
		return;

	pNpcTransform->LookAt(m_tActionTalkBackUpDesc.vOriginLookAt);

	pNpcTransform->Set_Position(m_tActionTalkBackUpDesc.vOriginPosition);
}

HRESULT CCamera_Action::Ready_Action_Witch_Invasion(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_tActionWitchInvasionDesc.pWitchObject = pGameObject;

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Witch_Invasion()
{
	if (nullptr == m_tActionWitchInvasionDesc.pWitchObject)
		return E_FAIL;

	m_eCurActionType = CAMERA_ACTION_TYPE::WITCH_INVASION;

	CModel* pWitchModel = m_tActionWitchInvasionDesc.pWitchObject->Get_Component_Model();
	CTransform* pWitchTransform = m_tActionWitchInvasionDesc.pWitchObject->Get_Component_Transform();

	if (nullptr == pWitchModel || nullptr == pWitchTransform)
		return E_FAIL;

	Vec4 vPos = Vec4(pWitchTransform->Get_Position()) + pWitchTransform->Get_RelativeOffset({ 1.f, -18.f, 4.25f, 1.f });
	m_pTransformCom->Set_Position(vPos.OneW());

	Matrix matLookWorld = pWitchModel->Get_SocketLocalMatrix(m_tActionWitchInvasionDesc.iBoneNumber) * pWitchTransform->Get_WorldMatrix();
	Vec4 vLookAt;
	memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));


	vLookAt += m_pTransformCom->Get_RelativeOffset(m_tActionWitchInvasionDesc.vLookAtOffset).ZeroW();

	m_pTransformCom->LookAt(vLookAt.OneW());

	return S_OK;
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
	case CAMERA_ACTION_TYPE::STADIUM:
	{
		return m_pTransformCom->Get_LookAt();
	}
	break;
	case CAMERA_ACTION_TYPE::WITCH_AWAY:
	{
		return m_tActionWitchAwayDesc.vPervLookAt;
	}
	break;
	default:
		break;
	}
	return m_pTransformCom->Get_LookAt();
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

HRESULT CCamera_Action::Start_Action_Talk(CGameObject* pNpc, const _bool& bFinalBoss)
{
	m_tActionTalkDesc.Clear();

	m_eCurActionType = CAMERA_ACTION_TYPE::TALK;
	m_bAction = true;

	/* 정보 세팅 */
	{
		CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		if (nullptr == pPlayer)
			return E_FAIL;

		/* 플레이어 상태, 인풋 변경 */
		{
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
			pPlayer->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);

			m_tActionTalkDesc.pTransform_Player = pPlayer->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransform_Player)
				return E_FAIL;
		}

		if (!m_tActionTalkDesc.bInit)
		{
			m_tActionTalkDesc.pKuu = CGame_Manager::GetInstance()->Get_Kuu();
			if (nullptr == m_tActionTalkDesc.pKuu)
				return E_FAIL;

			m_tActionTalkDesc.pTransform_Kuu = m_tActionTalkDesc.pKuu->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransform_Kuu)
				return E_FAIL;

			m_tActionTalkDesc.bInit = true;
		}

		if (nullptr != pNpc)
		{
			m_tActionTalkDesc.pTransformNpc = pNpc->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == m_tActionTalkDesc.pTransformNpc)
				return E_FAIL;
		}
	}

	/* 카메라 세팅 */
	{
		/* 대화 중간 액션 카메라의 액션 타입이 바뀌는 경우 존재 */
		CCamera_Manager::GetInstance()->Set_CurCamera(m_iKey);

		/* 기존 코드 */
		//if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(m_iKey)))
		//	return E_FAIL;
	}


	if (!bFinalBoss) /* 대화 상대가 마지막 보스가 아닌 경우 */
	{
		if (nullptr == pNpc) /* 쿠우 + 플레이어*/
		{
			Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::KUU_AND_PLAYER);
		}
		else if (nullptr != pNpc) /* 쿠우 + 플레이어 + Npc */
		{
			Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::ALL_RIGTH);
			Set_Talk_Transform(ACTION_TALK_DESC::VIEW_TYPE::NPC); 
		}
	}
	else /* 대화 상대가 마지막 보스인 경우 (단독적 트랜스폼 설정) */
	{
		m_tActionTalkDesc.pTransform_Player->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc->Get_Position());

		m_tActionTalkDesc.pTransformNpc->LookAt_ForLandObject(m_tActionTalkDesc.pTransform_Player->Get_Position());

		m_tActionTalkDesc.bSet = true;

		m_tActionTalkDesc.bFinalBoss = true;
	}

	return S_OK;
}

HRESULT CCamera_Action::Change_Action_Talk_Object(const ACTION_TALK_DESC::VIEW_TYPE& eType)
{
	if (nullptr == m_tActionTalkDesc.pTransform_Kuu || nullptr == m_tActionTalkDesc.pTransform_Player)
		return E_FAIL;

	/* 액션카메라 미리 세팅하고 다른 카메라 다녀오는 경우 등 예외적인 경우가 있어서 여기서도 처리 */
	m_eCurActionType = CAMERA_ACTION_TYPE::TALK;

	Set_Talk_Transform(eType);

	return S_OK;
}

HRESULT CCamera_Action::Finish_Action_Talk(const CAMERA_TYPE& eNextCameraType)
{
	if (CAMERA_TYPE::FOLLOW == eNextCameraType)
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			pFollowCam->Reset_WideView_To_DefaultView(true);
			pFollowCam->Set_Default_Position();
			CCamera_Manager::GetInstance()->Change_Camera(pFollowCam->Get_Key());
		}
	}
	else if (CAMERA_TYPE::CAMERA_CURLING == eNextCameraType)
	{

	}

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
		pFollowCam->Reset_WideView_To_DefaultView(true);

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

void CCamera_Action::Tick_Ending(_float fTimeDelta)
{
	/* 대화 뷰에서 멀리 떨어진 곳까지 블렌딩 */
	if (m_tActionEndingDesc.bActive)
	{
		if (m_tActionEndingDesc.tLerpPos.bActive)
		{
			m_tActionEndingDesc.tLerpPos.Update_Lerp(fTimeDelta);

			m_pTransformCom->Set_Position(Vec4(m_tActionEndingDesc.tLerpPos.vCurVec).OneW());
		}

		if (m_tActionEndingDesc.tLerpLook.bActive)
		{
			m_tActionEndingDesc.tLerpLook.Update_Lerp(fTimeDelta);

			m_pTransformCom->Set_LookAtByDir(Vec4(m_tActionEndingDesc.tLerpLook.vCurVec.Normalized()).ZeroW());
		}

		if (!m_tActionEndingDesc.tLerpPos.bActive && !m_tActionEndingDesc.tLerpLook.bActive)
		{
			m_tActionEndingDesc.bActive = false;

			/* 높이 보간 시작 */
			{
				m_tActionEndingDesc.fOriginHeight = Vec4(m_pTransformCom->Get_Position()).y;
				m_tActionEndingDesc.tLerpHeight.Start(
					0.f,
					m_tActionEndingDesc.fHeightDelta,
					m_tActionEndingDesc.fHeightLerpDuration,
					m_tActionEndingDesc.eHeightLerpMode);
				
				m_tActionEndingDesc.bUp = true;
			}
		}
	}

	/* 블렌딩 이후 바람 타듯 높이 럴프*/
	if (m_tActionEndingDesc.tLerpHeight.bActive)
	{
		m_tActionEndingDesc.tLerpHeight.Update(fTimeDelta);

		Vec4 vPos = m_pTransformCom->Get_Position();

		vPos.y = m_tActionEndingDesc.fOriginHeight + m_tActionEndingDesc.tLerpHeight.fCurValue;

		m_pTransformCom->Set_Position(vPos);

		if (!m_tActionEndingDesc.tLerpHeight.bActive)
		{
			if (m_tActionEndingDesc.bUp)
			{
				m_tActionEndingDesc.tLerpHeight.Start(
					m_tActionEndingDesc.fHeightDelta,
					0.f,
					m_tActionEndingDesc.fHeightLerpDuration,
					m_tActionEndingDesc.eHeightLerpMode);
			}
			else
			{
				m_tActionEndingDesc.tLerpHeight.Start(
					0.f,
					m_tActionEndingDesc.fHeightDelta,
					m_tActionEndingDesc.fHeightLerpDuration,
					m_tActionEndingDesc.eHeightLerpMode);
			}

			m_tActionEndingDesc.bUp = !m_tActionEndingDesc.bUp;
		}
	}
}

void CCamera_Action::Tick_Criminal(_float fTimeDelta)
{
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
	if (Is_Shake())
	{
		m_tActionTalkDesc.vPrevLookAt += Vec4(Get_ShakeLocalPos());

		m_pTransformCom->LookAt(m_tActionTalkDesc.vPrevLookAt);
	}
}

HRESULT CCamera_Action::Start_Action_Witch_Away(CGameObject* pGameObject)
{
	m_bAction = true;

	m_eCurActionType = CAMERA_ACTION_TYPE::WITCH_AWAY;

	m_tActionWitchAwayDesc.pWitchObject = pGameObject;
	
	m_tTargetOffset.vCurVec = { 0.f, 1.75f, 4.f, 1.f };
	
	m_tLookAtOffset.vCurVec = { -0.25f, 0.f, 0.f, 1.f };
	
	return S_OK;
}

void CCamera_Action::Tick_Blending(const _float fDeltaTime)
{
}

void CCamera_Action::Test(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::INSERT))
	{
		Start_Action_WindMill(true);
	}
}

void CCamera_Action::Tick_Stadium(_float fTimeDelta)
{
	/* Time Update */
	m_tActionStadiumDesc.fAcc += fTimeDelta;
	if (m_tActionStadiumDesc.fDurationPerView <= m_tActionStadiumDesc.fAcc)
	{
		m_tActionStadiumDesc.fAcc = 0.f;
		m_tActionStadiumDesc.iCurViewNum++;

		/* Increase View Type */
		if (ACTION_STADIUM_DESC::VIEW_NUM::VIEW_NUM_END == m_tActionStadiumDesc.iCurViewNum)
		{
			Finish_Action_Stadium();
			return;
		}

		/* Set Transform */
		{
			m_pTransformCom->Set_Position(m_tActionStadiumDesc.ViewPositions[m_tActionStadiumDesc.iCurViewNum]);
			m_pTransformCom->Set_LookAtByDir(m_tActionStadiumDesc.ViewLooks[m_tActionStadiumDesc.iCurViewNum]);
		}
	}

	if (Is_Shake())
	{
		m_pTransformCom->Set_LookAtByDir(m_tActionStadiumDesc.ViewLooks[m_tActionStadiumDesc.iCurViewNum]);

		Vec4 vLookAt = m_pTransformCom->Get_LookAt() + Vec4(Get_ShakeLocalPos());
		
		m_pTransformCom->LookAt(vLookAt.OneW());
	}

	//if (ACTION_STADIUM_DESC::VIEW_NUM::V4_FINAL == m_tActionStadiumDesc.iCurViewNum)
	//{
	//	m_pTransformCom->Translate(Vec3(m_pTransformCom->Get_Look()).Normalized() * -1.f * fTimeDelta);
	//}
}

void CCamera_Action::Tick_WindMill(_float fTimeDelta)
{
	if (m_tActionWindMillDesc.tLookAt.bActive)
	{
		m_tActionWindMillDesc.tLookAt.Update_Lerp(fTimeDelta);
		m_pTransformCom->LookAt(m_tActionWindMillDesc.tLookAt.vCurVec);

		if (!m_tActionWindMillDesc.tLookAt.bActive)
		{
			if (m_tActionWindMillDesc.bNpcToWindMill) /* NPC -> 풍차 완료 */
			{

			}
			else /* 풍차 -> NPC 완료 */
			{
				m_eCurActionType = CAMERA_ACTION_TYPE::TALK;
			}
		}
	}
	else
	{
		if (m_tActionWindMillDesc.bNpcToWindMill)
		{
			m_tActionWindMillDesc.fAcc += fTimeDelta;

			if (m_tActionWindMillDesc.fWaitTime <= m_tActionWindMillDesc.fAcc)
			{
				m_tActionWindMillDesc.fAcc = 0.f;

				Start_Action_WindMill(false);
			}
		}
	}
}

void CCamera_Action::Tick_SwordManBurst(_float fTimeDelta)
{
	m_tActionSwordManBurstDesc.fAcc +=  fTimeDelta;

	/* Rotation */
	_float fRadian = min(m_tActionSwordManBurstDesc.fAcc, XMConvertToRadians(90.f));
	
	if (fRadian < XMConvertToRadians(90.f))
	{
		m_pTransformCom->RevolutionRotation(m_tActionSwordManBurstDesc.pSwordManTransform->Get_Position(), Vec3(-0.2f, 0.9f, 0.f), 2.3f * fRadian * fTimeDelta);
	}
	
}

void CCamera_Action::Tick_EngineerBurst(_float fTimeDelta)
{

}

void CCamera_Action::Tick_Witch_Roar(_float fTimeDelta)
{
	CModel* pWitchModel = m_tActionWitchRoarDesc.pWitchObject->Get_Component_Model();
	CTransform* pWitchTransform = m_tActionWitchRoarDesc.pWitchObject->Get_Component_Transform();

	if (nullptr == pWitchModel || nullptr == pWitchTransform)
		return;

	if (m_tActionWitchRoarDesc.tTargetOffset.bActive)
	{
		m_tActionWitchRoarDesc.tTargetOffset.Update_Lerp(fTimeDelta);
		if (!m_tActionWitchRoarDesc.bZoomIn && !m_tActionWitchRoarDesc.tTargetOffset.bActive)
		{
			Start_Action_Witch_Roar();
		}
	}

	if (m_tActionWitchRoarDesc.tLookAtOffset.bActive)
		m_tActionWitchRoarDesc.tLookAtOffset.Update_Lerp(fTimeDelta);
	
	/* Position */
	{
		Vec4 vPos = Vec4(pWitchTransform->Get_Position()) + pWitchTransform->Get_RelativeOffset(m_tActionWitchRoarDesc.tTargetOffset.vCurVec);
		m_pTransformCom->Set_Position(vPos.OneW());
	}

	/* LookAt */
	{
		Matrix matLookWorld = pWitchModel->Get_SocketLocalMatrix(m_tActionWitchRoarDesc.iBoneNumber) * pWitchTransform->Get_WorldMatrix();
		Vec4 vLookAt;
		memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));
		vLookAt += m_pTransformCom->Get_RelativeOffset(m_tActionWitchRoarDesc.tLookAtOffset.vCurVec).ZeroW();

		m_pTransformCom->LookAt(vLookAt.OneW());
	}
}

void CCamera_Action::Tick_DestroyerBurst(_float fTimeDelta)
{

}

void CCamera_Action::Set_Talk_Transform(const ACTION_TALK_DESC::VIEW_TYPE& eType)
{
	Vec4 vCamPosition, vCamLookAt;

	switch (eType)
	{
	case CCamera_Action::tagActionTalkDesc::KUU:
	{
		if (m_tActionTalkDesc.bFinalBoss)
		{

		}
		else
		{

		}

		const _float fDistance			= 2.f;
		const _float fCamLookAtHeight	= 0.2f;

		/* CamPosition */
		{
			const Vec4 vTargetOffset = { -0.2f, 0.3f, 0.f, 1.f };
			vCamPosition = Vec4(m_tActionTalkDesc.pKuu->Get_GoalPosition()) 
				+ Vec4(m_tActionTalkDesc.pTransform_Kuu->Get_Look()).ZeroY().Normalized() * fDistance
				+ m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vTargetOffset);
			vCamPosition.w = 1.f;
		}

		/* CamLookAt */
		{
			vCamLookAt = m_tActionTalkDesc.pKuu->Get_GoalPosition();
			vCamLookAt.y += fCamLookAtHeight;
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_SuperNarrow);
	}
		break;
	case CCamera_Action::tagActionTalkDesc::KUU_AND_PLAYER:
	{

		if (m_tActionTalkDesc.bFinalBoss)
		{

		}
		else
		{

		}

		/* CamPosition */
		{
			Vec4 vTargetOffSet = { 0.5f, 0.4f, 2.f, 1.f };
			vCamPosition = m_tActionTalkDesc.pKuu->Get_GoalPosition() + m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vTargetOffSet);
		}

		/* CamLookAt */
		{
			Vec4 vLookAtOffset = { -1.f, 0.f, 0.f, 1.f };
			vCamLookAt = m_tActionTalkDesc.pKuu->Get_GoalPosition() + m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vLookAtOffset);
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Narrow);
	}
		break;
	case CCamera_Action::tagActionTalkDesc::NPC:
	{
		if (nullptr == m_tActionTalkDesc.pTransformNpc)
			return;
		
		_float	fDistance = 0.f, fCamPostionHeight = 0.f, fCamLookAtHeight = 0.f;
		Vec4	vTargetOffset		= { };
		if (m_tActionTalkDesc.bFinalBoss)
		{
			fDistance			= 4.f;
			fCamPostionHeight	= 1.5f;
			fCamLookAtHeight	= 1.5f;
			vTargetOffset		= { -0.3f, 0.f, 0.f, 1.f };
		}
		else
		{
			fDistance			= 2.3f;
			fCamPostionHeight	= 1.4f;
			fCamLookAtHeight	= 1.2f;
			vTargetOffset		= { -0.3f, 0.f, 0.f, 1.f };
		}

		/* CamPosition */
		{
			vCamPosition = Vec4(m_tActionTalkDesc.pTransformNpc->Get_Position())
							+ Vec4(m_tActionTalkDesc.pTransformNpc->Get_Look()).ZeroY().Normalized() * fDistance
							+ m_tActionTalkDesc.pTransformNpc->Get_RelativeOffset(vTargetOffset);
			vCamPosition.y += fCamPostionHeight;
		}

		/* CamLookAt */
		{
			vCamLookAt = m_tActionTalkDesc.pTransformNpc->Get_Position();
			vCamLookAt.y += fCamLookAtHeight;
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Narrow);
	}
		break;
	case CCamera_Action::tagActionTalkDesc::ALL_RIGTH:
	{
		if (nullptr == m_tActionTalkDesc.pTransformNpc)
			return;

		/* 대화 시작 최초 캐릭터 포지션 세팅 */
		if (!m_tActionTalkDesc.bSet)
		{
			const _float fDist = 3.f;
			Vec4 vNpcLook = m_tActionTalkDesc.pTransformNpc->Get_Look();
			Vec4 vNpcPos = m_tActionTalkDesc.pTransformNpc->Get_Position();

			vNpcLook.Normalize();

			Vec4 vPlayerPos = vNpcPos + (vNpcLook * fDist);
			m_tActionTalkDesc.pTransform_Player->Set_State(CTransform::STATE_POSITION, vPlayerPos.OneW());
			m_tActionTalkDesc.pKuu->Set_GoalPosition();
		}

		_float fDistanceToNpc = 0.f;

		Vec4 vPlayerKuuCenterPos = Vec4(m_tActionTalkDesc.pTransform_Kuu->Get_Position() + m_tActionTalkDesc.pTransform_Player->Get_Position()) * 0.5f;
		vPlayerKuuCenterPos.w = 1.f;

		Vec4 vDirToNpcLook = Vec4(m_tActionTalkDesc.pTransformNpc->Get_Position()) - vPlayerKuuCenterPos;
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
		vCamPosition.y -= fDistanceToNpc * 0.0f;

		/* CamLookAt */
		vCamLookAt = vCenterPosToNpc;
		vCamLookAt.y -= fDistanceToNpc * 0.0f;

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Default);

		/* 대화 시작 캐릭터 최초 룩 설정 */
		if (!m_tActionTalkDesc.bSet)
		{
			m_tActionTalkDesc.pTransformNpc->LookAt_ForLandObject(vPlayerKuuCenterPos);
			m_tActionTalkDesc.pTransform_Player->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc->Get_Position());
			m_tActionTalkDesc.pTransform_Kuu->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc->Get_Position());

			m_tActionTalkDesc.bSet = true;
		}
	}
		break;
	case CCamera_Action::tagActionTalkDesc::ALL_LEFT:
	{
		if (nullptr == m_tActionTalkDesc.pTransformNpc)
			return;

		/* 대화 시작 최초 캐릭터 포지션 세팅 */
		if (!m_tActionTalkDesc.bSet)
		{
			const _float fDist = 3.f;
			Vec4 vNpcLook = m_tActionTalkDesc.pTransformNpc->Get_Look();
			Vec4 vNpcPos = m_tActionTalkDesc.pTransformNpc->Get_Position();

			vNpcLook.Normalize();

			Vec4 vPlayerPos = vNpcPos + (vNpcLook * fDist);
			m_tActionTalkDesc.pTransform_Player->Set_State(CTransform::STATE_POSITION, vPlayerPos.OneW());
			m_tActionTalkDesc.pKuu->Set_GoalPosition();
		}

		_float fDistanceToNpc = 0.f;

		Vec4 vPlayerKuuCenterPos = Vec4(m_tActionTalkDesc.pTransform_Kuu->Get_Position() + m_tActionTalkDesc.pTransform_Player->Get_Position()) * 0.5f;
		vPlayerKuuCenterPos.w = 1.f;

		Vec4 vDirToNpcLook = Vec4(m_tActionTalkDesc.pTransformNpc->Get_Position()) - vPlayerKuuCenterPos;
		{
			fDistanceToNpc = vDirToNpcLook.Length();
			vDirToNpcLook.Normalize();
		}

		Vec4 vDirToNpcLeft = XMVector3Cross(Vec3::UnitY, vDirToNpcLook.xyz()) * -1.f;
		{
			vDirToNpcLeft.Normalize();
		}

		Vec4 vCenterPosToNpc = m_tActionTalkDesc.pKuu->Get_GoalPosition() + (vDirToNpcLook.ZeroY() * fDistanceToNpc * 0.5f);
		{
			vCenterPosToNpc.w = 1.f;
		}

		/* CamPosition */
		{
			Vec4 vTwoThirdsPosToNpc = vPlayerKuuCenterPos + (vDirToNpcLook.ZeroY() * fDistanceToNpc * 0.7f);
			vCamPosition = vTwoThirdsPosToNpc + (vDirToNpcLeft.ZeroY() * fDistanceToNpc * 1.f);
			vCamPosition.y += fDistanceToNpc * 0.25f;
		}

		/* CamLookAt */
		{
			vCamLookAt = vCenterPosToNpc;
			vCamLookAt.y -= fDistanceToNpc * 0.0f;
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Default);

		/* 대화 시작 캐릭터 최초 룩 설정 */
		if (!m_tActionTalkDesc.bSet)
		{
			m_tActionTalkDesc.pTransformNpc->LookAt_ForLandObject(vPlayerKuuCenterPos);
			m_tActionTalkDesc.pTransform_Player->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc->Get_Position());
			m_tActionTalkDesc.pTransform_Kuu->LookAt_ForLandObject(m_tActionTalkDesc.pTransformNpc->Get_Position());

			m_tActionTalkDesc.bSet = true;
		}
	}
	break;
	case CCamera_Action::tagActionTalkDesc::KUU_AND_PLAYER_FROM_BACK_NPC:
	{
		if (nullptr == m_tActionTalkDesc.pTransformNpc)
			return;

		Vec4 vTargetOffset = {};

		if (m_tActionTalkDesc.bFinalBoss)
		{
			vTargetOffset = { 1.f, 2.25f, -2.f, 1.f };
		}
		else
		{
			vTargetOffset = { 1.f, 2.f, -1.f, 1.f };
		}

		/* CamPosition */
		{
			vCamPosition = (Vec4)m_tActionTalkDesc.pTransformNpc->Get_Position() + (Vec4)m_tActionTalkDesc.pTransformNpc->Get_RelativeOffset(vTargetOffset);
			vCamPosition.w = 1.f;
		}

		/* CamLookAt */
		{
			Vec4 vPlayerKuuCenterPos = Vec4(m_tActionTalkDesc.pTransform_Kuu->Get_Position() + m_tActionTalkDesc.pTransform_Player->Get_Position()) * 0.5f;
			vCamLookAt = vPlayerKuuCenterPos;
			vCamLookAt.w = 1.f;
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Narrow);
	}
	break;
	case CCamera_Action::tagActionTalkDesc::NPC_FROM_BACK_KUU_AND_PLAYER:
	{
		if (nullptr == m_tActionTalkDesc.pTransformNpc)
			return;

		Vec4 vTargetOffset = {}, vLookAtOffset = {};

		if (m_tActionTalkDesc.bFinalBoss)
		{
			vTargetOffset = { 0.f, 0.5f, -1.3f, 1.f };
			vLookAtOffset = { 0.f, 1.f, 0.f, 1.f };
		}
		else
		{
			vTargetOffset = { 0.f, 1.f, -1.3f, 1.f };
			vLookAtOffset = { 0.f, 1.f, 0.f, 1.f };
		}

		/* CamPosition */
		{
			vCamPosition = m_tActionTalkDesc.pKuu->Get_GoalPosition() + m_tActionTalkDesc.pTransform_Kuu->Get_RelativeOffset(vTargetOffset);
			vCamPosition.w = 1.f;
		}

		/* CamLookAt */
		{
			vCamLookAt = (Vec4)m_tActionTalkDesc.pTransformNpc->Get_Position() + (Vec4)m_tActionTalkDesc.pTransformNpc->Get_RelativeOffset(vLookAtOffset);
			vCamLookAt.w = 1.f;
		}

		/* Fov */
		Set_Fov(Cam_Fov_Action_Talk_Narrow);
	}
	break;
	default:
		break;
	}


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPosition.OneW());
	m_pTransformCom->LookAt(vCamLookAt.OneW());

	m_tActionTalkDesc.vPrevLookAt = vCamLookAt.OneW();
}

void CCamera_Action::Tick_Witch_Invasion(_float fTimeDelta)
{
	if (nullptr == m_tActionWitchInvasionDesc.pWitchObject)
		return;

	CModel* pWitchModel = m_tActionWitchInvasionDesc.pWitchObject->Get_Component_Model();
	CTransform* pWitchTransform = m_tActionWitchInvasionDesc.pWitchObject->Get_Component_Transform();

	if (nullptr == pWitchModel || nullptr == pWitchTransform)
		return;

	Matrix matLookWorld = pWitchModel->Get_SocketLocalMatrix(m_tActionWitchInvasionDesc.iBoneNumber) * pWitchTransform->Get_WorldMatrix();
	Vec4 vLookAt;
	memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));

	vLookAt += m_pTransformCom->Get_RelativeOffset(m_tActionWitchInvasionDesc.vLookAtOffset).ZeroW();

	m_pTransformCom->LookAt(vLookAt.OneW());
}

void CCamera_Action::Tick_Stellia_Roar(_float fTimeDelta)
{
	if (nullptr == m_tActionStelliaRoarDesc.pStellia)
		return;

	CModel* pModelStellia = m_tActionStelliaRoarDesc.pStellia->Get_Component_Model();
	CTransform* pTransformStellia = m_tActionStelliaRoarDesc.pStellia->Get_Component_Transform();

	if (nullptr == pModelStellia || nullptr == pTransformStellia)
		return;

	/* Position */
	m_pTransformCom->Set_Position(m_tActionStelliaRoarDesc.vPosition);

	/* LookAt */
	{
		Matrix matLookWorld = 
			pModelStellia->Get_SocketLocalMatrix(m_tActionStelliaRoarDesc.iBoneNum) * pTransformStellia->Get_WorldMatrix();
		
		Vec4 vLookAt;
		memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));

		vLookAt += m_pTransformCom->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW();

		if (Is_Shake())
			vLookAt += Vec4(Get_ShakeLocalPos());

		if (!m_tActionStelliaRoarDesc.bSet)
		{
			m_tActionStelliaRoarDesc.bSet = true;
			m_tActionStelliaRoarDesc.fOriginLookAtHeight = vLookAt.y + m_tActionStelliaRoarDesc.fDeltaLookAtHeight;
		}

		if (m_tActionStelliaRoarDesc.bLower)
		{
			if (vLookAt.y < m_tActionStelliaRoarDesc.fOriginLookAtHeight)
				vLookAt.y = m_tActionStelliaRoarDesc.fOriginLookAtHeight;
			else
				m_tActionStelliaRoarDesc.bLower = false;
		}

		m_pTransformCom->LookAt(vLookAt.OneW());
	}
}

void CCamera_Action::Tick_Stellia_Dead(_float fTimeDelta)
{
	if (nullptr == m_tActionStelliaDeadDesc.pStellia)
		return;

	CModel* pModelStellia = m_tActionStelliaDeadDesc.pStellia->Get_Component_Model();
	CTransform* pTransformStellia = m_tActionStelliaDeadDesc.pStellia->Get_Component_Transform();

	if (nullptr == pModelStellia || nullptr == pTransformStellia)
		return;

	/* Position */
	{
		Vec4 vPos = Vec4(pTransformStellia->Get_Position()) + pTransformStellia->Get_RelativeOffset(m_tTargetOffset.vCurVec);
		m_pTransformCom->Set_Position(vPos.OneW());
	}

	/* LookAt */
	{
		Matrix matLookWorld =
			pModelStellia->Get_SocketLocalMatrix(m_tActionStelliaDeadDesc.iBoneNum) * pTransformStellia->Get_WorldMatrix();

		Vec4 vLookAt;
		memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));

		vLookAt += m_pTransformCom->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW();
		m_pTransformCom->LookAt(vLookAt.OneW());
	}
}


HRESULT CCamera_Action::Ready_Components()
{
	return S_OK;
}

void CCamera_Action::Tick_Witch_Away(_float fTimeDelta)
{
	CModel* pWitchModel = m_tActionWitchAwayDesc.pWitchObject->Get_Component_Model();
	CTransform* pWitchTransform = m_tActionWitchAwayDesc.pWitchObject->Get_Component_Transform();

	if (nullptr == pWitchModel || nullptr == pWitchTransform)
		return;

	if (m_tTargetOffset.bActive)
		m_tTargetOffset.Update_Lerp(fTimeDelta);

	if (m_tLookAtOffset.bActive)
		m_tLookAtOffset.Update_Lerp(fTimeDelta);

	/* Position */
	{
		Vec4 vPos = Vec4(pWitchTransform->Get_Position()) + pWitchTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);
		m_pTransformCom->Set_Position(vPos.OneW());
	}


	/* LookAt */
	{
		Vec4 vLookAt = {};
		const Matrix matLookWorld = pWitchModel->Get_SocketLocalMatrix(m_tActionWitchAwayDesc.iBoneNumber) * pWitchTransform->Get_WorldMatrix();
		memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));
		vLookAt += m_pTransformCom->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW();

		m_tActionWitchAwayDesc.vPervLookAt = vLookAt.OneW();

		m_pTransformCom->LookAt(m_tActionWitchAwayDesc.vPervLookAt);
	}
}

HRESULT CCamera_Action::Start_Action_TowerDefense()
{
	Set_Fov(Cam_Fov_Free_Default);

	m_bAction = true;

	m_eCurActionType = CAMERA_ACTION_TYPE::TOWER_DEFENSE;

	m_pTransformCom->Set_Position(m_tActionTowerDefenseDesc.vPositions[m_tActionTowerDefenseDesc.iCurViewIndex]);

	m_pTransformCom->Set_LookAtByDir(m_tActionTowerDefenseDesc.vLooks[m_tActionTowerDefenseDesc.iCurViewIndex]);

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
	
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);

	CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_WindMill(const _bool& bNpcToWindMill)
{
	const _uint iLevel = GI->Get_CurrentLevel();

	if (LEVELID::LEVEL_EVERMORE != iLevel && LEVELID::LEVEL_TOOL != iLevel)
		return E_FAIL;

	CGameObject* pWindMill = GI->Find_GameObject(iLevel, LAYER_BUILDING, m_tActionWindMillDesc.strWindMillName);
	if (nullptr == pWindMill)
		return E_FAIL;

	CTransform* pWindMillTransform = pWindMill->Get_Component<CTransform>(L"Com_Transform");

	if (nullptr == pWindMillTransform)
		return E_FAIL;

	if (bNpcToWindMill) /* NPC -> 풍차*/
	{
		Vec4 vTargetLook = Vec4(pWindMill->Get_Component<CTransform>(L"Com_Transform")->Get_Position())
							+ pWindMillTransform->Get_RelativeOffset(m_tActionWindMillDesc.vOffset);

		/* 룩앳 보간 시작 */
		m_tActionWindMillDesc.tLookAt.Start(
			m_tActionTalkDesc.vPrevLookAt,
			vTargetLook.OneW(),
			m_tActionWindMillDesc.fLerpDuration,
			LERP_MODE::SMOOTHER_STEP
		);

		m_eCurActionType = CAMERA_ACTION_TYPE::WINDMILL;
		m_tActionWindMillDesc.bNpcToWindMill = true;
	}
	else
	{
		/* 룩앳 보간 시작 */
		m_tActionWindMillDesc.tLookAt.Start(
			m_tActionWindMillDesc.tLookAt.vCurVec,
			m_tActionTalkDesc.vPrevLookAt,
			m_tActionWindMillDesc.fLerpDuration,
			LERP_MODE::SMOOTHER_STEP
		);

		m_eCurActionType = CAMERA_ACTION_TYPE::WINDMILL;
		m_tActionWindMillDesc.bNpcToWindMill = false;
	}

	return S_OK;
}

HRESULT CCamera_Action::Ready_Action_Witch_Roar(CGameObject* pGameObject)
{
	m_tActionWitchRoarDesc.pWitchObject = pGameObject;

	CModel*		pWitchModel		= m_tActionWitchRoarDesc.pWitchObject->Get_Component_Model();
	CTransform* pWitchTransform = m_tActionWitchRoarDesc.pWitchObject->Get_Component_Transform();

	if (nullptr == pWitchModel || nullptr == pWitchTransform)
		return E_FAIL;

	m_eCurActionType = CAMERA_ACTION_TYPE::WITCH_ROAR;

	m_tActionWitchRoarDesc.tTargetOffset.vCurVec = { 0.f, 1.2f, m_tActionWitchRoarDesc.fOriginDist, 1.f };
	m_tActionWitchRoarDesc.tLookAtOffset.vCurVec = { 0.f, -0.5f, 0.f, 1.f };

	/* Position */
	{
		Vec4 vPos = Vec4(pWitchTransform->Get_Position()) + pWitchTransform->Get_RelativeOffset(m_tActionWitchRoarDesc.tTargetOffset.vCurVec);
		m_pTransformCom->Set_Position(vPos.OneW());
	}

	/* LookAt */
	{
		Matrix matLookWorld = pWitchModel->Get_SocketLocalMatrix(m_tActionWitchRoarDesc.iBoneNumber) * pWitchTransform->Get_WorldMatrix();
		Vec4 vLookAt;
		memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));
		vLookAt += m_pTransformCom->Get_RelativeOffset(m_tActionWitchRoarDesc.tLookAtOffset.vCurVec).ZeroW();

		m_pTransformCom->LookAt(vLookAt.OneW());
	}

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Witch_Roar()
{
	switch (m_tActionWitchRoarDesc.iCount)
	{
	case 0 :
	{
		m_tActionWitchRoarDesc.bZoomIn = true;

		// Zoom In 
		m_tActionWitchRoarDesc.tTargetOffset.Start(
			{ 0.f, 2.f, 1.f, 1.f }, 3.f, LERP_MODE::SMOOTHER_STEP);

		m_tActionWitchRoarDesc.tLookAtOffset.Start(
			{ 0.f, 0.f, 0.f, 1.f }, 2.f, LERP_MODE::SMOOTHER_STEP);

		break;
	}
	case 1:
	{
		m_tActionWitchRoarDesc.bZoomIn = false;

		// Zoom Out 1
		m_tActionWitchRoarDesc.tTargetOffset.Start(
			{ 0.f, 2.f, 1.75f, 1.f }, 0.25f, LERP_MODE::EASE_IN);

		break;
	}
	case 2:
	{
		// Zoom Out 2
		m_tActionWitchRoarDesc.tTargetOffset.Start(
			{ 0.f, 2.f, 3.f, 1.f }, 3.f, LERP_MODE::DEFAULT);
		break;
	}
	default:
		break;
	}

	++m_tActionWitchRoarDesc.iCount;

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_SwordManBurst(CTransform* pSwordManTransform)
{
	m_eCurActionType = CAMERA_ACTION_TYPE::SWORDMAN_BURST;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);

	Set_TargetOffSet(Vec4(0.f, 2.f, -5.f, 1.f));
	Set_LookAtOffSet(Vec4(0.f, 1.f, 0.f, 1.f));

	Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());

	Vec4 vOffsetPosition = pSwordManTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);
	Vec4 vOffsetLookAt = pSwordManTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec);

	Vec4 vFinalPosition = Vec4(pSwordManTransform->Get_Position()) + vOffsetPosition;
	Vec4 vFinalLookAt = Vec4(pSwordManTransform->Get_Position()) + vOffsetLookAt;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalPosition);
	m_pTransformCom->LookAt(vFinalLookAt.OneW());


	m_tActionSwordManBurstDesc.vCamStartPosition = m_pTransformCom->Get_Position();
	m_tActionSwordManBurstDesc.pSwordManTransform = pSwordManTransform;
	m_tActionSwordManBurstDesc.fAcc = 0.f;

	m_bAction = true;

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_EngineerBurst(CTransform* pEngineerTransform)
{
	m_eCurActionType = CAMERA_ACTION_TYPE::ENGINEER_BURST;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
	Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());

	m_tActionEngineerBurstDesc.pEngineerTransform = pEngineerTransform;
	m_tActionEngineerBurstDesc.fAcc = 0.f;

	m_bAction = true;

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_DestroyerBurst(CTransform* pDestroyerTransform)
{
	m_eCurActionType = CAMERA_ACTION_TYPE::DESTROYER_BURST;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);

	Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());

	m_tActionDestroyerBurstDesc.pDestroyerTransform = pDestroyerTransform;
	m_tActionDestroyerBurstDesc.fAcc = 0.f;

	m_bAction = true;
	
	return S_OK;
}

HRESULT CCamera_Action::Stop_ActionSwordMan_Burst()
{
	m_eCurActionType = CAMERA_ACTION_TYPE::SWORDMAN_BURST;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_tActionSwordManBurstDesc.vCamStartPosition);

	m_bAction = false;
	return S_OK;
}

HRESULT CCamera_Action::Stop_ActionEngineer_Burst()
{
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	m_bAction = false;
	return S_OK;
}

HRESULT CCamera_Action::Stop_ActionDestroyer_Burst()
{
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	m_bAction = false;
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

HRESULT CCamera_Action::Start_Action_Stellia_Roar(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_tActionStelliaRoarDesc.pStellia = pGameObject;

	Set_Fov(Cam_Fov_Free_Default);

	memcpy(&m_tLookAtOffset.vCurVec, &m_tActionStelliaRoarDesc.vLookAtOffset, sizeof(Vec4));

	m_bAction = true;

	m_eCurActionType = CAMERA_ACTION_TYPE::STELLIA_ROAR;

	CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Stellia_Dead(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_tActionStelliaDeadDesc.pStellia = pGameObject;

	Set_Fov(XMConvertToRadians(65.f));

	m_tTargetOffset.vCurVec = Vec4{ 0.f, 3.f, 9.f, 1.f };
	m_tLookAtOffset.vCurVec = Vec4{ 0.f, 3.f, 0.f, 1.f };

	m_bAction = true;

	m_eCurActionType = CAMERA_ACTION_TYPE::STELLIA_DEAD;

	CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
}


void CCamera_Action::Tick_TowerDefense(_float fTimeDelta)
{
	m_tActionTowerDefenseDesc.fAcc += fTimeDelta;
	if (m_tActionTowerDefenseDesc.fLimitPerView <= m_tActionTowerDefenseDesc.fAcc)
	{
		m_tActionTowerDefenseDesc.fAcc = 0.f;
		m_tActionTowerDefenseDesc.iCurViewIndex++;

		if (ACTION_TOWER_DEFENSE_DESC::VIEW_NUM::VIEW_NUM_END == m_tActionTowerDefenseDesc.iCurViewIndex)
		{
			CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
			if (nullptr != pFollowCam)
			{
				pFollowCam->Reset_WideView_To_DefaultView(true);
				pFollowCam->Set_Default_Position();
			}
			CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
			return;
		}
		else
		{
			m_pTransformCom->Set_Position(m_tActionTowerDefenseDesc.vPositions[m_tActionTowerDefenseDesc.iCurViewIndex]);
			m_pTransformCom->Set_LookAtByDir(m_tActionTowerDefenseDesc.vLooks[m_tActionTowerDefenseDesc.iCurViewIndex]);
		}
	}
}

HRESULT CCamera_Action::Start_Action_Ending()
{
	m_bAction = true;

	m_eCurActionType = CAMERA_ACTION_TYPE::ENDING;

	m_tActionEndingDesc.bActive = true;

	ACTION_ENDING_DESC::VIEW_NUM eView = ACTION_ENDING_DESC::VIEW_NUM::V0;
	{
		m_tActionEndingDesc.tLerpPos.Start(
			CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Position(), 
			m_tActionEndingDesc.ViewPositions[eView],
			m_tActionEndingDesc.fLerpDuration, 
			m_tActionEndingDesc.eLerpMode);

		m_tActionEndingDesc.tLerpLook.Start(
			Vec3(CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look()).Normalized(),
			m_tActionEndingDesc.ViewLooks[eView],
			m_tActionEndingDesc.fLerpDuration,
			m_tActionEndingDesc.eLerpMode);
	}

	Start_Lerp_Fov(m_tActionEndingDesc.fTargetFov, m_tActionEndingDesc.fLerpDuration* 0.7f, m_tActionEndingDesc.eLerpMode);

	return S_OK;
}
