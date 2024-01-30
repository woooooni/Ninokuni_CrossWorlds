#include "stdafx.h"
#include "State_CurlingGame_Intro.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"
#include "Camera_Manager.h"
#include "CurlingGame_Manager.h"

#include "Camera_Group.h"

#include "Player.h"
#include "Character.h"

CState_CurlingGame_Intro::CState_CurlingGame_Intro(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Intro::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Intro::Enter_State(void* pArg)
{
	if (FAILED(Ready_Stauium()))
		return;

	if (FAILED(Ready_Decals()))
		return;

	if (FAILED(Ready_UIs()))
		return;

	if (FAILED(Ready_Characters()))
		return;
}

void CState_CurlingGame_Intro::Tick_State(const _float& fTimeDelta)
{
	if (m_tStadiumDesc.tLerHeight.bActive)
		Tick_Stadium(fTimeDelta);

	if (KEY_TAP(KEY::L))
	{
		CCamera_CurlingGame* pCurlingGameCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CAMERA_CURLING));

		if (nullptr != pCurlingGameCam)
		{
			if (FAILED(pCurlingGameCam->Change_Target(m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pOwner)))
				return;
		}
	}
}

void CState_CurlingGame_Intro::LateTick_State(const _float& fTimeDelta)
{
}

void CState_CurlingGame_Intro::Exit_State()
{
}

HRESULT CState_CurlingGame_Intro::Render()
{
	return S_OK;
}

void CState_CurlingGame_Intro::Tick_Stadium(const _float& fTimeDelta)
{
	m_tStadiumDesc.tLerHeight.Update(fTimeDelta);

	const _float fDeltaHeight = m_tStadiumDesc.tLerHeight.fCurValue - m_tStadiumDesc.fPrevHeight;

	for (auto& iter : m_pManager->m_pStadiumObjects)
	{
		if (nullptr == iter)
			continue;

		CTransform* pTransform = iter->Get_Component<CTransform>(TEXT("Com_Transform"));
		if (nullptr != pTransform)
		{
			Vec4 vPos = pTransform->Get_Position();
			vPos.y += fDeltaHeight;

			pTransform->Set_State(CTransform::STATE::STATE_POSITION, vPos.OneW());
		}
	}

	/* 경기장이 다 올라왔다면 */
	if (!m_tStadiumDesc.tLerHeight.bActive)
	{
		for (auto& pStadiumObject : m_pManager->m_pStadiumObjects)
		{
			if (FAILED(GI->Add_Building(pStadiumObject,
				pStadiumObject->Get_Component<CModel>(L"Com_Model"),
				pStadiumObject->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
			{
				MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
				return;
			}
		}

		if (FAILED(Set_CharacterTransform()))
			return;

		if (FAILED(Set_CameraTransform()))
			return;
	}

	m_tStadiumDesc.fPrevHeight = m_tStadiumDesc.tLerHeight.fCurValue;
}

HRESULT CState_CurlingGame_Intro::Ready_Stauium()
{
	for (auto& iter : m_pManager->m_pStadiumObjects)
	{
		if (nullptr != iter && iter->Get_ObjectTag() == TEXT("Winter_MiniGameMap_Stair"))
		{
			CTransform* pTransform = iter->Get_Component<CTransform>(TEXT("Com_Transform"));
			if (nullptr == pTransform)
				return E_FAIL;

			m_tStadiumDesc.tLerHeight.Start(0.f,
				m_tStadiumDesc.fTargetHeight,
				m_tStadiumDesc.fLerpTime, m_tStadiumDesc.eLerpMode);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CState_CurlingGame_Intro::Ready_UIs()
{
	/* Input */
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Attack_Input(false);
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Skill_Input(false);

	/* UI */
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
	}

	return S_OK;
}

HRESULT CState_CurlingGame_Intro::Ready_Decals()
{
	/* Ring Decal */
	{
		const Matrix matWorld = Matrix::CreateTranslation(m_pManager->m_tStandardDesc.vGoalPosition);

		for (size_t i = 0; i < CCurlingGame_Manager::STANDARD_DESC::RING_TYPE::RING_TYPEEND; i++)
		{
			Vec3 vScale = Vec3(m_pManager->m_tStandardDesc.fRingScalesForRender[i]);
			vScale.y = m_pManager->m_tStandardDesc.fHeight;

			if (FAILED(CEffect_Manager::GetInstance()->Generate_Decal(m_pManager->m_tStandardDesc.wstrRingNames[i], matWorld,
				Vec3::Zero, vScale, Vec3::Zero, nullptr, nullptr, false)))
				return E_FAIL;
		}
	}

	/* StartLine */
	{
		const Matrix matWorld = Matrix::CreateTranslation(m_pManager->m_tStandardDesc.vStartLinePosition);

		Vec3 vScale = m_pManager->m_tStandardDesc.vStartLineScale;
		vScale.y = m_pManager->m_tStandardDesc.fHeight;

		if (FAILED(CEffect_Manager::GetInstance()->Generate_Decal(m_pManager->m_tStandardDesc.wstrStartLineName, matWorld,
			Vec3::Zero, vScale, m_pManager->m_tStandardDesc.vStartLineRotation, nullptr, nullptr, false)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CState_CurlingGame_Intro::Set_CameraTransform()
{
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_CurCamera());
	CCamera_CurlingGame* pCurlingGameCam = dynamic_cast<CCamera_CurlingGame*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CAMERA_CURLING));
	
	if (nullptr == pFollowCam || nullptr == pCurlingGameCam)
		return E_FAIL;

	/* Follow Camera */
	{
		pFollowCam->Set_CanWideView(false);

	}

	/* CurlingGame Camera */
	{
		if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::CAMERA_CURLING)))
			return E_FAIL;
	
		const Vec4 vTargetPos = (m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].pTransformCom->Get_Position() 
								+ m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].pTransformCom->Get_Position())
								* 0.5f;

		/* 컬링 미니게임 시작 카메라 세팅 */
		{
			const Vec4 vLookAt = pCurlingGameCam->Calculate_LookAt(vTargetPos).OneW();

			// 기초 작업 1
			{
				const Vec4 vCamPosForLook = Vec4(m_pManager->m_tStandardDesc.vStartLinePosition + (m_pManager->m_tStandardDesc.vStartLook * -5.f)).OneW();
				pCurlingGameCam->Get_Transform()->Set_State(CTransform::STATE_POSITION, vCamPosForLook);
				pCurlingGameCam->Get_Transform()->LookAt(vLookAt);
			}

			// 기초 작업 2
			{
				const Vec4 vCamPosForLook = pCurlingGameCam->Calculate_CamPosition(vTargetPos).OneW();
				pCurlingGameCam->Get_Transform()->Set_State(CTransform::STATE_POSITION, vCamPosForLook);
			}

			// 파이널 룩앳 세팅 
			pCurlingGameCam->Get_Transform()->LookAt(vLookAt);

			// 파이널 포지션 세팅
			const Vec4 vCamPos = pCurlingGameCam->Calculate_CamPosition(vTargetPos).OneW();
			pCurlingGameCam->Get_Transform()->Set_State(CTransform::STATE_POSITION, vCamPos);

		}
	}

	return S_OK;
}

HRESULT CState_CurlingGame_Intro::Ready_Characters()
{
	/* Player */
	{
		CGameObject* pGameObject = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		if (nullptr == pGameObject)
			return E_FAIL;

		const _uint iType = CCurlingGame_Manager::PARTICIPANT_PLAYER;

		m_pManager->m_tParticipants[iType].pOwner = pGameObject;
		m_pManager->m_tParticipants[iType].pModelCom = pGameObject->Get_Component<CModel>(L"Com_Model");
		m_pManager->m_tParticipants[iType].pTransformCom = pGameObject->Get_Component<CTransform>(L"Com_Transform");
	}

	/* Npc */
	{
		CGameObject* pGameObject = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"Destroyer_Dummy");

		if (nullptr == pGameObject)
			return E_FAIL;

		const _uint iType = CCurlingGame_Manager::PARTICIPANT_NPC;

		m_pManager->m_tParticipants[iType].pOwner = pGameObject;
		m_pManager->m_tParticipants[iType].pModelCom = pGameObject->Get_Component<CModel>(L"Com_Model");
		m_pManager->m_tParticipants[iType].pTransformCom = pGameObject->Get_Component<CTransform>(L"Com_Transform"); 
	}

	for (size_t i = 0; i < CCurlingGame_Manager::PARTICIPANT_TYPEEND; i++)
	{
		if (nullptr == m_pManager->m_tParticipants[i].pOwner
			|| nullptr == m_pManager->m_tParticipants[i].pModelCom
			|| nullptr == m_pManager->m_tParticipants[i].pTransformCom
			)
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CState_CurlingGame_Intro::Set_CharacterTransform()
{
	const Vec3 vLook = m_pManager->m_tStandardDesc.vStartLook;

	const Vec3 vRight = Vec3(XMVector3Cross(Vec3::UnitY, vLook)).ZeroY().Normalized();

	for (_uint i = 0; i < CCurlingGame_Manager::PARTICIPANT_TYPEEND; i++)
	{
		Vec3 vStartPosition = m_pManager->m_tStandardDesc.vStartLinePosition + (vLook * -3.f);

		if (CCurlingGame_Manager::PARTICIPANT_PLAYER == i)
		{
			vStartPosition += vRight * m_pManager->m_tStandardDesc.vStartPosDelta;
		}
		else if (CCurlingGame_Manager::PARTICIPANT_NPC == i)
		{
			vStartPosition += -vRight * m_pManager->m_tStandardDesc.vStartPosDelta;
		}

		m_pManager->m_tParticipants[i].pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(vStartPosition).OneW());

		m_pManager->m_tParticipants[i].pTransformCom->LookAt_ForLandObject(m_pManager->m_tStandardDesc.vGoalPosition);
	}
	return S_OK;
}

CState_CurlingGame_Intro* CState_CurlingGame_Intro::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Intro* pInstance = new CState_CurlingGame_Intro(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Intro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Intro::Free()
{
	__super::Free();
}

