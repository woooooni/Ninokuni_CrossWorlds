#include "..\Public\Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag, _uint iObjectID)
	: CGameObject(pDevice, pContext, strObjTag, iObjectID)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera::Initialize_Prototype()
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tProjDesc, pArg, sizeof(PROJ_DESC));

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	Tick_Lerp(fTimeDelta);
	Tick_Shake(fTimeDelta);
}

void CCamera::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

HRESULT CCamera::Render()
{
	if (!m_bActive)
		return S_OK;

	return S_OK;
}

void CCamera::Start_Lerp_Fov(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tProjDesc.tLerpFov.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Fov(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tProjDesc.tLerpFov.Start(m_tProjDesc.tLerpFov.fCurValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Distance(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tLerpDist.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Distance(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tLerpDist.Start(m_tLerpDist.fCurValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Shake(const _float& fAmplitude, const _float& fFrequency, const _float& fDuration)
{
	m_tShakeDesc.bActive = true;

	m_tShakeDesc.fAmplitude = fAmplitude;
	m_tShakeDesc.fFrequency = fFrequency;
	m_tShakeDesc.fDuration = fDuration;

	m_tShakeDesc.fFreqDelta = m_tShakeDesc.fDuration / m_tShakeDesc.fFrequency;

	const _uint iRand = rand() % MAX_UNIT_RAND;

	const Vec3 vTargetPos = { v2UnitRand[iRand].x, v2UnitRand[iRand].y, 0.f };

	m_tShakeDesc.tLerpShakeUnitPos.Start(Vec3::Zero, vTargetPos, m_tShakeDesc.fFreqDelta, LERP_MODE::SMOOTHER_STEP);
}

void CCamera::Tick_Lerp(const _float fDeltaTime)
{
	if (m_tProjDesc.tLerpFov.bActive)
	{
		m_tProjDesc.tLerpFov.Update(fDeltaTime);
	}

	if (m_tLerpDist.bActive)
	{
		m_tLerpDist.Update(fDeltaTime);
	}
}

void CCamera::Tick_Shake(const _float fDeltaTime)
{
	if (m_tShakeDesc.bActive)
	{
		m_tShakeDesc.fAccDuration += fDeltaTime;
		m_tShakeDesc.fAccFrequency += fDeltaTime;

		/* 최종 종료 체크 */
		if (m_tShakeDesc.fDuration <= m_tShakeDesc.fAccDuration)
		{
			/* 바로 뚝 끝나는게 아니라 보간 필요 */

			/* 블렌드 인 아웃 타임 적용 가능 */
			m_tShakeDesc.Clear();
			return;
		}

		/* 목표 위치 갱신 */
		if (m_tShakeDesc.fFreqDelta <= m_tShakeDesc.fAccFrequency)
		{
			m_tShakeDesc.fAccFrequency -= m_tShakeDesc.fFreqDelta;

			const Vec3 vStartPos = m_tShakeDesc.tLerpShakeUnitPos.vCurVec;

			const _uint iRand = rand() % MAX_UNIT_RAND;

			const Vec3 vTargetPos = { v2UnitRand[iRand].x, v2UnitRand[iRand].y, 0.f };

			m_tShakeDesc.tLerpShakeUnitPos.Start(vStartPos, vTargetPos, m_tShakeDesc.fFreqDelta, LERP_MODE::SMOOTHER_STEP);
		}
		else /* 위치 보간 */
		{
			m_tShakeDesc.tLerpShakeUnitPos.Update_Lerp(fDeltaTime);
		}
	}
}

HRESULT CCamera::Ready_Components()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
