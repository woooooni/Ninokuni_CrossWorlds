#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_Follow::CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		m_tLerpDist.fCurValue = Cam_Dist_Follow_Default;

		/* 팔로우 카메라에서 룩앳 오프셋을 사용하는 일은 없다. 타겟 오프셋만을 사용한다.*/
		m_vLookAtOffset = Vec4::UnitW;
		m_vTargetOffset = Vec4{ 0.7f, 1.3f, 0.f, 1.f };

		m_vMouseSensitivity = Vec2{ 0.18f, 0.5f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::Tick(fTimeDelta); /* Shake, Fov, Dist Lerp Update */

	/* Position */
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Calculate_WorldPosition(fTimeDelta));
	
	/* Look & Shake */
	Vec4 vLookAtPos = Calculate_Look(fTimeDelta);

	m_pTransformCom->LookAt(Calculate_Look(fTimeDelta));

	__super::Tick(fTimeDelta); /* Shake, Fov, Dist */
	m_pControllerCom->Tick_Controller(fTimeDelta);

	
	if (Is_Shake())
	{
		const Vec4 vLookAtPos = Calculate_Look(fTimeDelta);

		if (Is_Shake())
			m_pTransformCom->LookAt(Vec4(vLookAtPos + Vec4(Get_ShakeLocalPos())).OneW());
		else
			m_pTransformCom->LookAt(vLookAtPos);
	}

	/* Test */
	{
		/*if (KEY_TAP(KEY::J))
		{
			CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		}
		if (KEY_TAP(KEY::K))
		{
			static _bool bLerpFov = false;

			if (!bLerpFov)
				Start_Lerp_Fov(XMConvertToRadians(85.0f), 0.5f);
			else
				Start_Lerp_Fov(Cam_Fov_Follow_Default, 0.5f);

			bLerpFov = !bLerpFov;
		}

		if (KEY_TAP(KEY::L))
		{
			static _bool bLerpDist = false;

			if (!bLerpDist)
				Start_Lerp_Distance(6.3f, 0.5f);
			else
				Start_Lerp_Distance(Cam_Dist_Follow_Default, 0.5f);

			bLerpDist = !bLerpDist;
		}*/
	}
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	m_pControllerCom->LateTick_Controller(fTimeDelta);
	__super::LateTick(fTimeDelta);
	
}

HRESULT CCamera_Follow::Render()
{
	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components()
{
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 0.f, 0.f };
	ControllerDesc.fHeight = 0.01f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 0.1f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	return S_OK;
}

Vec4 CCamera_Follow::Calculate_WorldPosition(_float fTimeDelta)
{
	/* 구면 로컬 위치 (반지름 1 기준) */
	Vec4 vLocalSpherical = Calculate_LoaclSphericalPosition(fTimeDelta);

	/* 디스턴스 반영 */
	vLocalSpherical *= m_tLerpDist.fCurValue;
	
	/* 카메라 목표 월드 위치  */
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	if (nullptr == pTargetTransform)
		return Vec4::UnitW;

	Vec4 vWorldGoal = vLocalSpherical
		+ Vec4(pTargetTransform->Get_Position());											 /* 타겟 포지션 */
		+ Calculate_ReleativePosition(m_vTargetOffset, pTargetTransform->Get_WorldMatrix()); /* 타겟의 회전을 반영한 오프셋 */

	vWorldGoal.w = 1.f;

	/* 댐핑 적용 월드 위치 (카메라의 현재 위치와 목표위치를 댐핑 계수에 따라 보간한다)*/
	if (m_tDampingDesc.bDamping)
		return Calculate_DampingPosition(vWorldGoal);

	return vWorldGoal;
}

Vec4 CCamera_Follow::Calculate_LoaclSphericalPosition(_float fTimeDelta)
{
	_long	MouseMove = 0l;

	if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
	{
		_float fDelta = MouseMove * m_vMouseSensitivity.y * fTimeDelta * -1.f;
		
		/* y축 회전량이 너무 많을 경우 카메라가 획 도는 현상 방지 하기 위한 제한 */
		{
			if (fDelta < m_fMinRotLimitDeltaY)
				fDelta = m_fMinRotLimitDeltaY;

			if (m_fMaxRotLimitDeltaY < fDelta)
				fDelta = m_fMaxRotLimitDeltaY;
		}

		m_vAngle.x += fDelta;
	}

	if (MouseMove = GI->Get_DIMMoveState(DIMM_Y))
	{
		m_vAngle.y += MouseMove * m_vMouseSensitivity.x * fTimeDelta;

		if (m_vAngle.y <= m_fMinLimitY) /* Min : 0.f */
		{
			m_vAngle.y = m_fMinLimitY;
		}
		else if (m_fMaxLimitY < m_vAngle.y) /* Max : 3.14*/
		{
			m_vAngle.y = m_fMaxLimitY;
		}
	}

	/* 구면 좌표계(극좌표계) -> 왼손 직교 좌표계 */
	const _float fRadius = 1.f;

	Vec4 vCamLocal
	{
		fRadius * sinf(m_vAngle.y) * cosf(m_vAngle.x),	// x = r * sin(위도 앙각) * cos(경도 방위각)
		fRadius * cosf(m_vAngle.y),						// y = r * cos(위도 앙각)
		fRadius * sinf(m_vAngle.y) * sinf(m_vAngle.x),	// z = r * sin(위도 앙각) * sin(경도 방위각)
		1.f
	};

	return vCamLocal;
}

Vec4 CCamera_Follow::Calculate_Look(_float fTimeDelta)
{
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	Vec4 vPosition_LookAtObject = Vec4(pTargetTransform->Get_Position());

	Vec4 vPosition_Offset = Calculate_ReleativePosition(m_vTargetOffset, m_pTransformCom->Get_WorldMatrix()); /* 카메라의 회전상태를 반영한 타겟 오프셋 */

	return Vec4(vPosition_LookAtObject + vPosition_Offset).OneW();
}

Vec4 CCamera_Follow::Calculate_ReleativePosition(Vec4 vPos, Matrix matWorld)
{
	/* 월드형렬에서 회전상태만을 반영하여 포지션을 적용한다. */

	/* 행렬의 포지션 초기화 */
	matWorld.Translation(Vec3::Zero);

	/* 행렬의 라업룩 정규화 */
	matWorld.Right(XMVector3Normalize(matWorld.Right()));
	matWorld.Up(XMVector3Normalize(matWorld.Up()));
	matWorld.Backward(XMVector3Normalize(matWorld.Backward()));
	
	/* 행렬의 회전 적용된 포지션 값 */
	Vec3 vRelativePos = XMVector3TransformCoord(vPos, matWorld);

	return Vec4(vRelativePos.x, vRelativePos.y, vRelativePos.z, 1.f);
}

Vec4 CCamera_Follow::Calculate_DampingPosition(Vec4 vGoalPos)
{
	if (!m_tDampingDesc.bSet) /* 댐핑이 켜졌지만, 최초 세팅이 안 된 경우 세팅한다. */
	{
		m_tDampingDesc.bSet = true;
		m_tDampingDesc.vCurPos = vGoalPos;

		return m_tDampingDesc.vCurPos;
	}
	else /* 이전에 세팅이 이루어 졌다면 댐핑 계산을 적용한다. */
	{
		/* 만약 타겟의 룩과 카메라의 룩이 역방향에 가까울 경우 댐핑의 속도를 올린다. */
		Vec4 vTargetLook	= m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Look();
		Vec4 vCamLook		= m_pTransformCom->Get_Look();

		vTargetLook.y = vCamLook.y = 0.f;
		
		const _float fRad = acosf(vCamLook.Normalized().Dot(vTargetLook.Normalized()));

		_float fCoeff = 0.f;
		if (m_tDampingDesc.fDampingBackLimitRad <= fRad)
			fCoeff = m_tDampingDesc.fDampingCoefficient * m_tDampingDesc.fDampingCoefficientBackMag;
		else
			fCoeff = m_tDampingDesc.fDampingCoefficient;

		const Vec4 vDist = (vGoalPos.ZeroW() - m_tDampingDesc.vCurPos.ZeroW()) * fCoeff;

		m_tDampingDesc.vCurPos += vDist;

		return m_tDampingDesc.vCurPos.OneW();
	}
	
	return vGoalPos;
}

CCamera_Follow * CCamera_Follow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	__super::Free();
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pTransformCom);
}
