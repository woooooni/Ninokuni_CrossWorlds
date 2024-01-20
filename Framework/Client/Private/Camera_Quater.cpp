#include "stdafx.h"
#include "Camera_Quater.h"

#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Player.h"

CCamera_Quater::CCamera_Quater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Quater::CCamera_Quater(const CCamera_Quater& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Quater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Quater::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_VirtualTarget()))
		return E_FAIL;

	/* Set Camera */
	{
		Set_Fov(Cam_Fov_Quater_Default);
		Set_Distance(Cam_Dist_Quater_Default);	
	}
	return S_OK;
}

void CCamera_Quater::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (CCamera_Quater::MODE_TYPE::TRANSLATION == m_eModeType || CCamera_Quater::MODE_TYPE::NONE == m_eModeType)
		Tick_Input();

	switch (m_eModeType)
	{
	case CCamera_Quater::MODE_TYPE::TRANSLATION:
		Tick_Translation(fTimeDelta);
		break;
	case CCamera_Quater::MODE_TYPE::ZOOM:
		Tick_Zoom(fTimeDelta);
		break;
	case CCamera_Quater::MODE_TYPE::ROTATION:
		Tick_Rotation(fTimeDelta);
		break;
	default:
		break;
	}

	Test(fTimeDelta);
}

void CCamera_Quater::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

Vec4 CCamera_Quater::Get_LookAt()
{
	return Vec4::UnitW;
}

void CCamera_Quater::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	const Vec4 vLook = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_pTransformCom->LookAt(vLook);
}

void CCamera_Quater::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

}

void CCamera_Quater::Set_Active(const _bool bActive)
{
	__super::Set_Active(bActive);

	if (m_bActive)
	{
		/* Player All Input Off */
		{
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);

			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
		}

		/* 카메라 바뀔시 시작 포지션과 룩 세팅 (할아버지 기준) */
		CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"TreeGrandfa");
		if (nullptr != pTarget)
		{
			CTransform* pTargetTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");
			if(nullptr != pTargetTransform)
			{
				m_pVirtualTargetTransform->Set_WorldMatrix(pTargetTransform->Get_WorldMatrix());

				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_RIGHT, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[0]).Normalized());
				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_UP, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[1]).Normalized());
				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_LOOK, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[2]).Normalized());

				m_pVirtualTargetTransform->Set_State(CTransform::STATE_POSITION, pTargetTransform->Get_Position());
			}
		}

		/* 최초 시작 카메라 트랜스폼 세팅 */
		{
			/* Cam Position */
			{
				Vec4 vLookRight = Vec4(m_pVirtualTargetTransform->Get_Look() + m_pVirtualTargetTransform->Get_Right()).Normalized();

				Vec4 vGoalPos = (Vec4)m_pVirtualTargetTransform->Get_Position() /* 타겟 원점 포지션 */
					+ vLookRight.ZeroY() * m_tLerpDist.fCurValue /* x, z 세팅 */
					+ Vec4::UnitY * m_fInitHeight; /* y 세팅 */

				m_vCurPos = vGoalPos.OneW();

				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, m_vCurPos);
			}

			/* Cam LoookAt*/
			m_pTransformCom->LookAt(m_pVirtualTargetTransform->Get_Position());
		}

		/* Tick Transform */
		Calculate_CamHeightFromDistance();
		Tick_Translation(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));
	}
	else
	{
		/* Player All Input On */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
		m_eViewType = CCamera_Quater::VIEW_TYPE::NE;
		m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
	}
}

HRESULT CCamera_Quater::Ready_Components()
{
	return S_OK;
}

HRESULT CCamera_Quater::Ready_VirtualTarget()
{
	m_pVirtualTargetTransform = dynamic_cast<CTransform*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Transform"));

	if (nullptr == m_pVirtualTargetTransform)
		return E_FAIL;

	return S_OK;
}

void CCamera_Quater::Tick_Input()
{
	/* Zoom */
	m_iMouseWheel = GI->Get_DIMMoveState(DIMM::DIMM_WHEEL);
	if (0 != m_iMouseWheel)
	{
		m_eModeType = MODE_TYPE::ZOOM;

		// 댐핑 리셋

		return;
	}

	/* Roatiton */
	{
		if (KEY_TAP(KEY::Q))
		{
			_int iType = (_int)m_eViewType + 1;
			if (VIEW_TYPE::NW < iType)
				iType = VIEW_TYPE::NE;

			m_eViewType = (CCamera_Quater::VIEW_TYPE)iType;
			m_eModeType = MODE_TYPE::ROTATION;

			// 댐핑 리셋
			return;
		}
		else if (KEY_TAP(KEY::E))
		{
			_int iType = (_int)m_eViewType - 1;
			if (iType < 0)
				iType = VIEW_TYPE::NW;

			m_eViewType = (CCamera_Quater::VIEW_TYPE)iType;
			m_eModeType = MODE_TYPE::ROTATION;

			// 댐핑 리셋
			return;
		}
	}

	/* Translation */
	{
		if (KEY_HOLD(KEY::W) && KEY_HOLD(KEY::D))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look()
				+ m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::D) && KEY_HOLD(KEY::S))
		{
			m_vVirtualTargetMoveDir = (m_pTransformCom->Get_Look() * -1.f)
				+ m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::S) && KEY_HOLD(KEY::A))
		{
			m_vVirtualTargetMoveDir = (m_pTransformCom->Get_Look() * -1.f)
				+ (m_pTransformCom->Get_Right() * -1.f);
		}
		else if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::W))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look()
				+ (m_pTransformCom->Get_Right() * -1.f);
		}
		else if (KEY_HOLD(KEY::W))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look();
		}
		else if (KEY_HOLD(KEY::S))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look() * -1.f;
		}
		else if (KEY_HOLD(KEY::D))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::A))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Right() * -1.f;
		}

		if (0.f < m_vVirtualTargetMoveDir.Length())
		{
			m_vVirtualTargetMoveDir.y = 0.f;
			m_vVirtualTargetMoveDir.Normalize();
			m_eModeType = MODE_TYPE::TRANSLATION;
		}
	}
}

void CCamera_Quater::Tick_Translation(const _float fDeltaTime)
{
	/* 가상 타겟 먼저 이동 */
	m_pVirtualTargetTransform->Translate(m_vVirtualTargetMoveDir * m_fVirtualTargetMoveSpeed * fDeltaTime);
	m_vVirtualTargetMoveDir = Vec3::Zero;

	/* Cam Position */
	{
		Vec4 vDir;

		switch (m_eViewType)
		{
		case CCamera_Quater::VIEW_TYPE::NE:
			vDir = (Vec4(m_pVirtualTargetTransform->Get_Look()) + Vec4(m_pVirtualTargetTransform->Get_Right())).ZeroW().Normalized();
			break;
		case CCamera_Quater::VIEW_TYPE::SE:
			vDir = (Vec4(m_pVirtualTargetTransform->Get_Look() * -1.f) + Vec4(m_pVirtualTargetTransform->Get_Right())).ZeroW().Normalized();
			break;
		case CCamera_Quater::VIEW_TYPE::SW:
			vDir = (Vec4(m_pVirtualTargetTransform->Get_Look() * -1.f) + Vec4(m_pVirtualTargetTransform->Get_Right() * -1.f)).ZeroW().Normalized();
			break;
		case CCamera_Quater::VIEW_TYPE::NW:
			vDir = (Vec4(m_pVirtualTargetTransform->Get_Look()) + Vec4(m_pVirtualTargetTransform->Get_Right() * -1.f)).ZeroW().Normalized();
			break;
		default:
			break;
		}

		Vec4 vGoalPos = (Vec4)m_pVirtualTargetTransform->Get_Position() /* 타겟 원점 포지션 */
			+ vDir.ZeroY() * m_tLerpDist.fCurValue; /* x, z 세팅 */

		vGoalPos.w = 1.f;
		vGoalPos.y = m_tHeight.fCurValue;

		if (m_fDampingMaxDistance < Vec4::Distance(vGoalPos, m_vCurPos))
		{
			m_vCurPos = vGoalPos;
		}
		else
		{
			const Vec4 vDist = (vGoalPos.ZeroW() - m_vCurPos.ZeroW()) * m_fDampingCoefficient;
			m_vCurPos += vDist;
			m_vCurPos.y = vGoalPos.y;
		}
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vGoalPos.OneW());
	}

	/* Cam LoookAt*/
	m_pTransformCom->LookAt(m_pVirtualTargetTransform->Get_Position());

	/* 댐핑이 끝나면 상태를 none으로 교체한다. */
	m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
}

void CCamera_Quater::Tick_Zoom(const _float fDeltaTime)
{
	if (0 != m_iMouseWheel)
	{
		if (0 < m_iMouseWheel)
		{
			--m_tLerpDist.fCurValue;
			if (m_tLerpDist.fCurValue < Cam_Dist_Quater_Min)
				m_tLerpDist.fCurValue = Cam_Dist_Quater_Min;
		}
		else
		{
			++m_tLerpDist.fCurValue;
			if (Cam_Dist_Quater_Max < m_tLerpDist.fCurValue)
				m_tLerpDist.fCurValue = Cam_Dist_Quater_Max;
		}

		Calculate_CamHeightFromDistance();

		m_iMouseWheel = 0;
	}

	/* 임시 */
	Tick_Translation(fDeltaTime);

	/* 줌 보간이 끝나면 상태를 None로 바꾼다. */
	m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
}

void CCamera_Quater::Tick_Rotation(const _float fDeltaTime)
{
	/* 임시 */
	Tick_Translation(fDeltaTime);

	/* 회전 보간이 끝나면 상태를 None로 바꾼다. */
	m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
}

void CCamera_Quater::Test(_float fTimeDelta)
{
	if (KEY_TAP(KEY::INSERT))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			pFollowCam->Reset_WideView_To_DefaultView();
			pFollowCam->Set_Default_Position();
			CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key());
		}
	}
}

const _float& CCamera_Quater::Calculate_CamHeightFromDistance()
{
	const Vec4 vDir_VirtualTargetToCam = Vec4(m_pTransformCom->Get_Position() - m_pVirtualTargetTransform->Get_Position()).Normalized();
	const _float fTheta = acosf(Vec4::UnitZ.Dot(vDir_VirtualTargetToCam)); /* 두 벡터(타겟으로부터 카메라, UnitZ) 사이의 각 -> acos 해보고 판단 */
	m_tHeight.fCurValue = m_tLerpDist.fCurValue * fTheta * m_fHeightMag;

	return m_tHeight.fCurValue;
}

CCamera_Quater* CCamera_Quater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Quater* pInstance = new CCamera_Quater(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Quater::Clone(void* pArg)
{
	CCamera_Quater* pInstance = new CCamera_Quater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Quater::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pVirtualTargetTransform);
}
