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

	Tick_Transform(fTimeDelta);

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
		{
			const wstring strTargetName = L"TreeGrandfa";

			CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, strTargetName);
			if (nullptr == pTarget)
				return;

			CTransform* pTargetTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");
			if(nullptr != pTargetTransform)
			{
				Vec4 vLookRight = Vec4(pTargetTransform->Get_Look() + pTargetTransform->Get_Right()).Normalized();

				Vec4 vCamPosition = (Vec4)pTargetTransform->Get_Position() /* 타겟 원점 포지션 */
					+ vLookRight.ZeroY() * m_tLerpDist.fCurValue /* x, z 세팅 */
					+ Vec4::UnitY * 25.f; /* y 세팅 */

				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition.OneW());
				m_pTransformCom->LookAt(pTarget->Get_Component<CTransform>(L"Com_Transform")->Get_Position());
			}
		}
	}
	else
	{
		/* Player All Input On */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
	}
}

HRESULT CCamera_Quater::Ready_Components()
{
	return S_OK;
}

void CCamera_Quater::Tick_Transform(const _float fDeltaTime)
{
	Vec3 vDir;

	if (KEY_HOLD(KEY::W) && KEY_HOLD(KEY::D))
	{
		vDir = m_pTransformCom->Get_Look() + m_pTransformCom->Get_Right();
	}
	else if(KEY_HOLD(KEY::D) && KEY_HOLD(KEY::S))
	{
		vDir = (m_pTransformCom->Get_Look() * -1.f) + m_pTransformCom->Get_Right();
	}
	else if (KEY_HOLD(KEY::S) && KEY_HOLD(KEY::A))
	{
		vDir = (m_pTransformCom->Get_Look() * -1.f) + (m_pTransformCom->Get_Right() * -1.f);
	}
	else if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::W))
	{
		vDir = m_pTransformCom->Get_Look() + (m_pTransformCom->Get_Right() * -1.f);
	}
	else if (KEY_HOLD(KEY::W))
	{
		vDir = m_pTransformCom->Get_Look();
	}
	else if (KEY_HOLD(KEY::S))
	{
		vDir = m_pTransformCom->Get_Look() * -1.f;
	}
	else if (KEY_HOLD(KEY::D))
	{
		vDir = m_pTransformCom->Get_Right();
	}
	else if (KEY_HOLD(KEY::A))
	{
		vDir = m_pTransformCom->Get_Right() * -1.f;
	}

	m_pTransformCom->Translate(vDir.ZeroY().Normalized() * m_fMoveSpeed * fDeltaTime);
}

void CCamera_Quater::Test(_float fTimeDelta)
{
	if (KEY_TAP(KEY::INSERT))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key());
		}
	}
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
}
