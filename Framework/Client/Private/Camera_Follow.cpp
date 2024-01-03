#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

/* Test */
#include "Camera_CutScene_Map.h"
#include "Game_Manager.h"
#include "Player.h"

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
		m_tTargetOffset.vCurVec = Cam_TargetOffset_Follow_Default;
		m_tLookAtOffset.vCurVec = Cam_LookAtOffset_Follow_Default;

		m_vMouseSensitivity = Vec2{ 0.18f, 0.5f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (!m_bActive || nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::Tick(fTimeDelta); 

	/* Check Lock */
	if (LOCK_PROGRESS::FINISH_BLEIDING == m_eLockProgress)
	{
		if (!m_tBlendingLookAtPosition.bActive)
			m_eLockProgress = LOCK_PROGRESS::OFF;
	}

	/* Position */
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Calculate_WorldPosition(fTimeDelta));
	
	/* Look & Shake */
	const Vec4 vLookAtPos = Calculate_Look(fTimeDelta);
	if (Is_Shake())
		m_pTransformCom->LookAt(Vec4(vLookAtPos + Vec4(Get_ShakeLocalPos())).OneW());
	else
		m_pTransformCom->LookAt(vLookAtPos);
	
	/* Collision */
	if(nullptr != m_pControllerCom)
		m_pControllerCom->Tick_Controller(fTimeDelta);

	/* Test */
	Test(fTimeDelta);
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (!m_bActive || nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

void CCamera_Follow::Set_Default_Position()
{
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	/* 현재 타겟의 Y축 회전 상태를 CW 0 ~ 360로 반환한다. */
	_float fRot = 0.f;
	{
		Vec3 vLook = pTargetTransform->Get_Look();

		_float fAngle = atan2(vLook.x, vLook.z);

		const _float fDegreesPerRadian = 180.0f / DirectX::XM_PI;
		fRot = fAngle * fDegreesPerRadian;
		if (fRot < 0)
			fRot += 360.f;
	}

	float result = 0.f;

	if (fRot >= 0 && fRot <= 270) {
		// 0 ~ 270의 값은 -0.5 pi ~ -2 pi로 변환
		result = -0.5f * XM_PI - fRot * (1.5f * XM_PI / 270.0f);
	}
	else if (fRot > 270 && fRot <= 360) {
		// 270 ~ 360의 값은 0 ~ -0.5pi로 변환
		result = 0.0f - (fRot - 270) * (0.5f * XM_PI / 90.0f); //0.0f - fRot * (0.5f * XM_PI / 90.0f);
	}
	else {
		// 범위를 벗어나면 그대로 반환
		result = fRot;
	}

	m_vAngle = { result, 1.f };

	m_tDampingDesc.bSet = false;
}

HRESULT CCamera_Follow::Start_LockOn(CGameObject* pTargetObject, const Vec4& vTargetOffset, const Vec4& vLookAtOffset, const _float& fLockOnBlendingTime)
{
	if (nullptr == pTargetObject || LOCK_PROGRESS::OFF != m_eLockProgress)
		return E_FAIL;

	Change_LookAtObj(pTargetObject, fLockOnBlendingTime);

	Change_LookAtOffSet(vLookAtOffset, fLockOnBlendingTime);

	Change_TargetOffSet(vTargetOffset, fLockOnBlendingTime);
	
	m_eLockProgress = LOCK_PROGRESS::START_BLENDING;

	return S_OK;
}

HRESULT CCamera_Follow::Finish_LockOn(CGameObject* pTargetObject, const _float& fLockOnBlendingTime)
{
	if (nullptr == pTargetObject || LOCK_PROGRESS::OFF == m_eLockProgress)
		return E_FAIL;

	Change_LookAtObj(pTargetObject, fLockOnBlendingTime);

	Change_LookAtOffSet(Cam_LookAtOffset_Follow_Default, fLockOnBlendingTime);

	Change_TargetOffSet(Cam_TargetOffset_Follow_Default, fLockOnBlendingTime);

	m_eLockProgress = LOCK_PROGRESS::FINISH_BLEIDING;

	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components()
{
	/* CPhysX_Controller */
	{
		CPhysX_Controller::CONTROLLER_DESC ControllerDesc;
		{
			ControllerDesc.eType = CPhysX_Controller::CAPSULE;
			ControllerDesc.pTransform = m_pTransformCom;
			ControllerDesc.vOffset = { 0.f, 0.f, 0.f };
			ControllerDesc.fHeight = 0.01f;
			ControllerDesc.fMaxJumpHeight = 10.f;
			ControllerDesc.fRaidus = 0.1f;
			ControllerDesc.pOwner = this;
		}

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
			return E_FAIL;
	}

	if(nullptr != m_pControllerCom)
		m_pControllerCom->Set_Active(true);

	return S_OK;
}

Vec4 CCamera_Follow::Calculate_WorldPosition(_float fTimeDelta)
{
	Vec4 vWorldGoal		= Vec4::UnitW;
	Vec4 vTargetOffset	= Vec4::UnitW;

	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	if (LOCK_PROGRESS::OFF != m_eLockProgress)
	{
		/* 카메라의 회전 상태를 반영한 오프셋 */
		vTargetOffset = Calculate_ReleativePosition(m_tTargetOffset.vCurVec, m_pTransformCom->Get_WorldMatrix());

		/* 카메라의 월드 행렬 상태 변환으로 인해 오프셋의 y가 -가 되어 땅을 뚫는 현상 방지*/
		if (vTargetOffset.y < m_fLockTargetOffsetMinY)
			vTargetOffset.y = m_fLockTargetOffsetMinY;

		/* 카메라 목표 월드 위치 */
		vWorldGoal = vTargetOffset + (Vec4)pTargetTransform->Get_Position();
	}
	else
	{
		/* 구면 로컬 위치 (원점, 반지름 1 기준) */
		Vec4 vLocalSpherical = Calculate_LoaclSphericalPosition(fTimeDelta);
	
		/* 디스턴스 반영 */
		vLocalSpherical *= m_tLerpDist.fCurValue;
		
		/* 타겟의 회전 상태를 반영한 오프셋 */
		vTargetOffset = Calculate_ReleativePosition(m_tTargetOffset.vCurVec, pTargetTransform->Get_WorldMatrix()); 
			
		/* 카메라 목표 월드 위치 */
		vWorldGoal = vLocalSpherical + vTargetOffset + (Vec4)pTargetTransform->Get_Position();											
	}
	vWorldGoal.w = 1.f;

	/* 댐핑 적용 최종 월드 위치 (카메라의 현재 위치와 목표위치를 댐핑 계수에 따라 보간한다) */
	if (m_tDampingDesc.bDamping)
		return Calculate_DampingPosition(vWorldGoal);

	return vWorldGoal;
}

Vec4 CCamera_Follow::Calculate_LoaclSphericalPosition(_float fTimeDelta)
{
	if (m_bCanInput) 
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
	Vec4 vLookAt, vLookAtOffset;

	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	/* 룩앳 위치 */
	if (LOCK_PROGRESS::OFF != m_eLockProgress)
	{
		/* 룩앳 오브젝트가 현재 블렌딩 중이라면 */
		if (Is_Blending_LookAtObj()) 
			vLookAt = m_tBlendingLookAtPosition.vCurVec;
		else
		{
			Matrix matLookWorld = m_pLookAtObj->Get_Component<CModel>(L"Com_Model")->Get_SocketLocalMatrix(0)
								* m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix();

			memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));
		}
	}
	else
		vLookAt = Vec4(pTargetTransform->Get_Position());

	/* 룩앳 오프셋 위치 */
	vLookAtOffset = Calculate_ReleativePosition(m_tLookAtOffset.vCurVec, m_pTransformCom->Get_WorldMatrix()); /* 카메라의 회전 상태를 반영한 타겟 오프셋 */

	return Vec4(vLookAt + vLookAtOffset).OneW();
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

void CCamera_Follow::Test(_float fTimeDelta)
{
	/* Test */
	{
		/* CutScene - Evermore */
		if (KEY_TAP(KEY::INSERT))
		{
			vector<string> CutSceneNames;
			CutSceneNames.push_back("Evermore_Street_00");
			CutSceneNames.push_back("Evermore_Street_01");

			dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP))->Start_CutScenes(CutSceneNames);
		}

		/* Lock On Off */
		if (KEY_TAP(KEY::DEL))
		{
			if (LOCK_PROGRESS::OFF == m_eLockProgress)
			{
				const _int iBossCount = 3;
				wstring strBossNames[iBossCount] = { L"Glanix", L"DreamerMazeWitch", L"Stellia" };

				for (size_t i = 0; i < iBossCount; i++)
				{
					CGameObject * pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_MONSTER, strBossNames[i]);
					if (nullptr != pTarget)
						Start_LockOn(pTarget, Cam_Target_Offset_LockOn_Glanix, Cam_LookAt_Offset_LockOn_Glanix);
				}
			}
			else
				Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		}

		/* Default Position */
		if (KEY_TAP(KEY::HOME))
		{
			Set_Default_Position();
		}
	}

	{
		CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

		/* 현재 타겟의 Y축 회전 상태를 CW 0 ~ 360로 반환한다. */
		_float fRot = 0.f;
		{
			Vec3 vLook = pTargetTransform->Get_Look();

			_float fAngle = atan2(vLook.x, vLook.z);

			const _float fDegreesPerRadian = 180.0f / DirectX::XM_PI;
			fRot = fAngle * fDegreesPerRadian;
			if (fRot < 0)
				fRot += 360.f;
		}

		float result = 0.f;// -1.5f * XM_PI + fRot * (0.5f * XM_PI) / 90.0f;

		if (fRot >= 0 && fRot <= 270) {
			// 0 ~ 270의 값은 -0.5 pi ~ -2 pi로 변환
			result =  -0.5f * XM_PI - fRot * (1.5f * XM_PI / 270.0f);
		}
		else if (fRot > 270 && fRot <= 360) {
			// 270 ~ 360의 값은 0 ~ -0.5pi로 변환
			result =  0.0f - fRot * (0.5f * XM_PI / 90.0f);
		}
		else {
			// 범위를 벗어나면 그대로 반환
			result = fRot;
		}
	}
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
	Safe_Release(m_pControllerCom);
}
