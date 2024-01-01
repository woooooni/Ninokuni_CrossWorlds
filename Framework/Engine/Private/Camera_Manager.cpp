#include "..\Public\Camera_Manager.h"

#include "GameInstance.h"
#include "Camera.h"

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

CCamera* CCamera_Manager::Find_Camera(const _uint& iKey)
{
	auto iter = m_pCameras.find(iKey);

	if(m_pCameras.end() == iter)
		return nullptr;

	return iter->second;
}

void CCamera_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	for (auto& Pair : m_pCameras)
		Safe_Release(Pair.second);
}



