#include "stdafx.h"
#include "Camera_Action.h"
#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "Game_Manager.h"
#include "Player.h"
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
	
		return m_tActionTalkDesc.vPrevLookAt;
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

HRESULT CCamera_Action::Start_Action_Talk(CGameObject* pPlayer, CGameObject* pPet, CGameObject* pNpc1, CGameObject* pNpc2)
{
	if (nullptr == pPlayer || nullptr == pPet || nullptr == pNpc1)
		return E_FAIL;

	/* Default Setting */
	{
		m_eCurActionType = CAMERA_ACTION_TYPE::TALK;

		m_bAction = true;
	}

	/* Talker Setting */
	{
		m_tActionTalkDesc.pPlayer = pPlayer;
		m_tActionTalkDesc.pPet = pPet;
		m_tActionTalkDesc.pNpc1 = pNpc1;
		
		m_tActionTalkDesc.iTalker++;

		if (nullptr != pNpc2)
		{
			m_tActionTalkDesc.pNpc2 = pNpc2;
			m_tActionTalkDesc.iTalker++;
		}
	}

	return Set_TalkCharacterPosition();
}

HRESULT CCamera_Action::Start_Action_Talk(CGameObject* pPlayer, CGameObject* pPet)
{
	if (nullptr == pPlayer || nullptr == pPet)
		return E_FAIL;

	/* Default Setting */
	{
		m_eCurActionType = CAMERA_ACTION_TYPE::TALK;

		m_bAction = true;
	}

	/* Talker Setting */
	{
		m_tActionTalkDesc.pPlayer = pPlayer;	
		m_tActionTalkDesc.pPet = pPet;
	}

	return Set_TalkCharacterPosition();
}

HRESULT CCamera_Action::Set_Action_Talk_Target(CGameObject* pTalker)
{
	if (nullptr == pTalker)
		return E_FAIL;

	m_tActionTalkDesc.pCurTalker = pTalker;

	CTransform* pTalkerTransform = m_tActionTalkDesc.pCurTalker->Get_Component<CTransform>(L"Com_Transform");

	if (nullptr == pTalkerTransform)
		return E_FAIL;

	/* 타겟별 오프셋 설정 */
	{
		m_tTargetOffset.vCurVec.x *= -1.f;
	}

	/* Cam Position */
	{
		const Vec4 vCamPosition = (Vec4)pTalkerTransform->Get_Position() +
			pTalkerTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec).ZeroW();

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPosition);

	}

	/* Cam LookAt */
	{
		m_tActionTalkDesc.vPrevLookAt  = (Vec4)pTalkerTransform->Get_Position() +
			pTalkerTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW();

		m_pTransformCom->LookAt(m_tActionTalkDesc.vPrevLookAt);
	}

	return S_OK;
}

HRESULT CCamera_Action::Finish_Action_Talk()
{
	if (m_tActionTalkDesc.pCurTalker == m_tActionTalkDesc.pPlayer) 
		return E_FAIL;

	/* 팔로우 카메라로 블렌딩 */
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pFollowCam)
	{
		pFollowCam->Set_Default_Position();

		CCamera_Manager::GetInstance()->Change_Camera(CAMERA_TYPE::FOLLOW);

		/* 기존 데이터 삭제 */
		m_tActionTalkDesc.Clear();

		m_bAction = false;

		return S_OK;
	}

	return E_FAIL;
}

void CCamera_Action::Tick_Lobby(_float fTimeDelta)
{
	if (!m_tActionLobbyDesc.vLerpCamLookAt.bActive)
		m_bAction = false;

	if (m_tActionLobbyDesc.vLerpCamLookAt.bActive)
		m_tActionLobbyDesc.vLerpCamLookAt.Update_Lerp(fTimeDelta);

	//if (m_tTargetOffset.bActive)
	//{
	//	m_tTargetOffset.Update_Lerp(fTimeDelta);
	//	if(!m_tTargetOffset.bActive)
	//		m_bAction = false;
	//}
	
	Vec4 vLookAt = m_tActionLobbyDesc.vLerpCamLookAt.vCurVec * m_tActionLobbyDesc.fLookMag;

	m_pTransformCom->LookAt(vLookAt.OneW());

	Vec4 vPos = m_tActionLobbyDesc.vCamPosition + Vec4(m_tTargetOffset.vCurVec);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos.OneW());

	//if (KEY_TAP(KEY::J))
	//{
	//	const _float fHeight = 6.f;
	//	const _float fLerpTime = 5.f;
	//
	//	Vec4 vTargetLook = m_tActionLobbyDesc.vLerpCamLookAt.vCurVec;
	//	vTargetLook.y += fHeight * 0.5f;
	//
	//	m_tActionLobbyDesc.vLerpCamLookAt.Start(m_tActionLobbyDesc.vLerpCamLookAt.vCurVec, vTargetLook, fLerpTime, LERP_MODE::EASE_OUT);
	//
	//	Vec4 vTargetOffSet = m_tTargetOffset.vCurVec;
	//	vTargetOffSet.y += fHeight;
	//
	//	m_tTargetOffset.Start(m_tTargetOffset.vCurVec, vTargetOffSet, fLerpTime, LERP_MODE::EASE_IN);
	//}
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
	//if (KEY_TAP(KEY::B))
	//{
	//	CGameObject* pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
	//
	//	if (nullptr != pTarget && nullptr != m_tActionTalkDesc.pNpc1)
	//	{
	//		Set_Action_Talk_Target(pTarget);
	//	}
	//}
	//if (KEY_TAP(KEY::N))
	//{
	//	CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"HumanFL04");
	//
	//	if (nullptr != pTarget)
	//	{
	//		Set_Action_Talk_Target(pTarget);
	//	}
	//}
	//if (KEY_TAP(KEY::M))
	//{
	//	Finish_Action_Talk();
	//}
}

HRESULT CCamera_Action::Set_TalkCharacterPosition()
{
	switch (m_tActionTalkDesc.iTalker)
	{
	case 2:
	{
		/* OffSet Setting */
		{
			m_tTargetOffset.vCurVec = m_tActionTalkDesc.vTargetOffset;
			m_tLookAtOffset.vCurVec = m_tActionTalkDesc.vLookAtOffset;
		}


		return Set_Action_Talk_Target(m_tActionTalkDesc.pPet);
	}
		break;
	case 3 :
	{

		/* OffSet Setting */
		{
			m_tTargetOffset.vCurVec = m_tActionTalkDesc.vTargetOffset;
			m_tLookAtOffset.vCurVec = m_tActionTalkDesc.vLookAtOffset;
		}

		CTransform* pNpcTransform = m_tActionTalkDesc.pNpc1->Get_Component<CTransform>(L"Com_Transform");
		CTransform* pPlayerTransform = m_tActionTalkDesc.pPlayer->Get_Component<CTransform>(L"Com_Transform");

		/* 대화 참여자들의 트랜스폼 세팅 */
		if (nullptr != pNpcTransform && nullptr != pPlayerTransform)
		{
			/* 플레이어 포지션 세팅 */
			Vec4 vPlayerPos = pNpcTransform->Get_RelativeOffset(Vec4{ 0.f, 0.f, m_tActionTalkDesc.fPlayerNpcDistance, 1.f });
			vPlayerPos += pNpcTransform->Get_Position();

			pPlayerTransform->Set_State(CTransform::STATE_POSITION, vPlayerPos);

			/* 플레이어가 Npc 바라보게 */
			pPlayerTransform->LookAt_ForLandObject(pNpcTransform->Get_Position());
		}
		return Set_Action_Talk_Target(m_tActionTalkDesc.pNpc1);
	}
		break;
	case 4:
	{



		return Set_Action_Talk_Target(m_tActionTalkDesc.pNpc1);
	}
		break;
	default:
		break;
	}

	return E_FAIL;
}

void CCamera_Action::Tick_Blending(const _float fDeltaTime)
{
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
