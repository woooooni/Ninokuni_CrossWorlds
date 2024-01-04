#include "..\Public\Camera_Manager.h"

#include "GameInstance.h"
#include "Camera.h"

#include "Utils.h"

IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{

}

HRESULT CCamera_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CCamera_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurCamera)
		return;

	/* Blending */
	if (m_bBlending)
		Tick_Blending(fTimeDelta);

	/* Cur Camera Update */
	m_pCurCamera->Tick(fTimeDelta);

	/* V(WI) */
	{
		GI->Set_Transform(CPipeLine::D3DTS_VIEW, m_pCurCamera->Get_Transform()->Get_WorldMatrixInverse());
	}

	/* P */
	{
		const CCamera::PROJ_DESC& tDesc = m_pCurCamera->Get_ProjDesc();
		GI->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(tDesc.tLerpFov.fCurValue, tDesc.fAspect, tDesc.fNear, tDesc.fFar));
	}
}

void CCamera_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pCurCamera)
		return;

	m_pCurCamera->LateTick(fTimeDelta);

	/* 아직 보간 처리 X 기본 싱글 카메라만 파이프라인에 전달 */

	/* V(WI) */
	{
		GI->Set_Transform(CPipeLine::D3DTS_VIEW, m_pCurCamera->Get_Transform()->Get_WorldMatrixInverse());
	}

	/* P */
	{
		const CCamera::PROJ_DESC& tDesc = m_pCurCamera->Get_ProjDesc();
		GI->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(tDesc.tLerpFov.fCurValue, tDesc.fAspect, tDesc.fNear, tDesc.fFar));
	}
}

CCamera* CCamera_Manager::Get_Camera(const _uint& iKey)
{
	return Find_Camera(iKey);
}

HRESULT CCamera_Manager::Set_CurCamera(const _uint& iKey)
{
	if (m_bBlending)
		return E_FAIL;

	CCamera* pCamera = Find_Camera(iKey);

	if (nullptr == pCamera)
		return E_FAIL;

	if (nullptr == m_pCurCamera) /* 최초 카메라 Set */
	{
		m_pCurCamera = pCamera;
		m_pCurCamera->Set_Active(true);
	}
	else
	{
		if (pCamera == m_pCurCamera)
			return S_OK;

		m_pPrevCamera = m_pCurCamera;

		m_pCurCamera->Set_Active(false);

		m_pCurCamera = pCamera;

		m_pCurCamera->Set_Active(true);
	}

	return S_OK;
}

HRESULT CCamera_Manager::Set_PrevCamera()
{
	if (nullptr == m_pPrevCamera || nullptr == m_pCurCamera)
		return E_FAIL;

	CCamera* pTemp = m_pCurCamera;

	m_pCurCamera->Set_Active(false);

	m_pCurCamera = m_pPrevCamera;

	m_pCurCamera->Set_Active(true);

	m_pPrevCamera = pTemp;

	return S_OK;
}

const _bool CCamera_Manager::Is_Empty_Camera(const _uint& iKey)
{
	return (nullptr == Find_Camera(iKey)) ? true : false;
}

HRESULT CCamera_Manager::Add_Camera(const _uint& iKey, CCamera* pCamera)
{
	if (nullptr != Find_Camera(iKey))
		return E_FAIL;

	m_pCameras.emplace(iKey, pCamera);

	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Shake_Default()
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	m_pCurCamera->Start_Shake(0.1f, 17.f, 0.3f);
	
	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Shake(const _float& fAmplitude, const _float& fFrequency, const _float& fDuration)
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	m_pCurCamera->Start_Shake(fAmplitude, fFrequency, fDuration);

	return S_OK;
}

HRESULT CCamera_Manager::Change_Camera(const _uint& iKey, const _float& fBlendingDuration, const LERP_MODE& eLerpMode)
{
	/* 바꿀 카메라를 현재 카메라로 지정하고, 캠포지션과 룩앳을 매니저에서 보간하여 받아 사용한다. 나머지는 카메라 자체 보간 처리 */

	CCamera* pNextCamera = Find_Camera(iKey);

	if (nullptr == pNextCamera || nullptr == m_pCurCamera || fBlendingDuration < 0.f)
		return E_FAIL;

	/* Set Property */
	{
		m_pCurCamera->Set_Blending(true);
		pNextCamera->Set_Blending(true);

		m_pCurCamera->Set_CanInput(false);
		pNextCamera->Set_CanInput(false);
	}

	/* 1. 캠 포지션 */
	m_tBlendingPosition.Start(m_pCurCamera->Get_Transform()->Get_Position(), 
								pNextCamera->Get_Transform()->Get_Position(), 
								fBlendingDuration, eLerpMode);
	
	/* 2. 룩앳 */
	Vec4 vSrc = (Vec4)m_pCurCamera->Get_LookAtObj()->Get_Component<CTransform>(L"Com_Transform")->Get_Position()
				+ m_pCurCamera->Get_LookAtObj()->Get_Component<CTransform>(L"Com_Transform")->Get_RelativeOffset(m_pCurCamera->Get_LookAtOffset());

	Vec4 vDest = (Vec4)pNextCamera->Get_LookAtObj()->Get_Component<CTransform>(L"Com_Transform")->Get_Position()
				+ pNextCamera->Get_LookAtObj()->Get_Component<CTransform>(L"Com_Transform")->Get_RelativeOffset(pNextCamera->Get_LookAtOffset());

	m_tBlendingLookAt.Start(vSrc.OneW(), vDest.OneW(), fBlendingDuration, eLerpMode);

	/* 3. 타겟, 룩앳 오프셋 */
	pNextCamera->Lerp_TargetOffset(m_pCurCamera->Get_TargetOffset(), pNextCamera->Get_TargetOffset(), fBlendingDuration, eLerpMode);
	pNextCamera->Lerp_LookAtOffSet(m_pCurCamera->Get_LookAtOffset(), pNextCamera->Get_LookAtOffset(), fBlendingDuration, eLerpMode);


	/* 4. fov, distance */
	pNextCamera->Start_Lerp_Fov(m_pCurCamera->Get_Fov(), pNextCamera->Get_Fov(), fBlendingDuration, eLerpMode);
	pNextCamera->Start_Lerp_Distance(m_pCurCamera->Get_Distance(), pNextCamera->Get_Distance(), fBlendingDuration, eLerpMode);

	if(FAILED(Set_CurCamera(iKey)))
		return E_FAIL;

	m_bBlending = true; /* Set_CurCamera()는 m_bBlending일 경우 return 되므로*/


	CUtils::ConsoleOut(m_tBlendingLookAt.vStartVec);

	return S_OK;
}

CCamera* CCamera_Manager::Find_Camera(const _uint& iKey)
{
	auto iter = m_pCameras.find(iKey);

	if(m_pCameras.end() == iter)
		return nullptr;

	return iter->second;
}

void CCamera_Manager::Tick_Blending(_float fTimeDelta)
{
	if (!m_tBlendingPosition.bActive)
	{
		m_bBlending = false;

		m_pPrevCamera->Set_Blending(false);
		m_pCurCamera->Set_Blending(false);

		m_pPrevCamera->Set_CanInput(true);
		m_pCurCamera->Set_CanInput(true);

		return;
	}

	m_tBlendingPosition.Update_Lerp(fTimeDelta);
	m_tBlendingLookAt.Update_Lerp(fTimeDelta);
}

void CCamera_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	for (auto& Pair : m_pCameras)
		Safe_Release(Pair.second);
}



