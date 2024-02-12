#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "Camera.h"
#include "Camera_Manager.h"
IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();	
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	pLight->Set_LightID(m_Lights.size());
	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Add_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp)
{
	auto iter = m_ShadowViewMatrix.find(iLevelIndex);

	if (iter != m_ShadowViewMatrix.end())
		return E_FAIL;

	_float4x4 LightViewMatrix; 
	XMStoreFloat4x4(&LightViewMatrix, XMMatrixLookAtLH(vEye, vAt, vUp));

	m_ShadowViewMatrix.emplace(iLevelIndex, LightViewMatrix);

	return S_OK;
}

HRESULT CLight_Manager::Set_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp)
{
	auto iter = m_ShadowViewMatrix.find(iLevelIndex);

	if (iter == m_ShadowViewMatrix.end())
		Add_ShadowLight(iLevelIndex, vEye, vAt, vUp);
	else
	{
		//_float4x4 LightViewMatrix;
		//XMStoreFloat4x4(&LightViewMatrix, XMMatrixLookAtLH(vEye, vAt, vUp));

		CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
		if (nullptr == pCameraManager)
			return S_OK;

		CCamera* pCamera = pCameraManager->Get_CurCamera();
		if (nullptr == pCamera)
			return S_OK;

		//const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
		Vec3 eyePos = vEye;
		Vec4 eyePosVec = Vec4(eyePos.x, eyePos.y, eyePos.z, 1.0f);
		Vec3 normalizeLightDir = vAt;
		normalizeLightDir.Normalize();
		Vec4 vTargetPosVec = Vec4(eyePos.x + normalizeLightDir.x, eyePos.y + normalizeLightDir.y, eyePos.z + normalizeLightDir.z, 1.0f);
		Vec4 vLightUp = vUp;
		Matrix lightVIewMatrix = ::XMMatrixLookAtLH(eyePosVec, vTargetPosVec, vUp);

		iter->second = lightVIewMatrix;
	}

	return S_OK;
}

HRESULT CLight_Manager::Add_ShadowProj(_uint iLevelIndex, _float fFovAngleY, _float fAspectRatio, _float fNearZ, _float fFarZ)
{
	auto iter = m_ShadowProjMatrix.find(iLevelIndex);

	if (iter != m_ShadowProjMatrix.end())
		return E_FAIL;

	_float4x4 LightProjMatrix;
	XMStoreFloat4x4(&LightProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(fFovAngleY), fAspectRatio, fNearZ, fFarZ));

	m_ShadowProjMatrix.emplace(iLevelIndex, LightProjMatrix);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		LIGHTDESC::TYPE eType = pLight->Get_LightDesc()->eType;

		if (LIGHTDESC::TYPE_DIRECTIONAL == pLight->Get_LightDesc()->eType)
		{
			if (FAILED(pLight->Render(pShader, pVIBuffer)))
				return E_FAIL;
		}
		else if ((LIGHTDESC::TYPE_POINT == eType) || (LIGHTDESC::TYPE_SPOT == eType))
		{
			CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
			
			if (nullptr == pCameraManager)
				return S_OK;

			CCamera* pCamera = pCameraManager->Get_CurCamera();
			if (nullptr == pCamera)
				return S_OK;

			Vec3 pCameraPosition = pCamera->Get_Transform()->Get_Position();
			Vec3 pLightPosition = pLight->Get_LightDesc()->vTempPosition;

			Vec3 vLength = pCameraPosition - pLightPosition;
			_float fDistance = ::fabs(vLength.Length());

			if (fDistance > pLight->Get_LightDesc()->fTempRange && false == pLight->Get_ModifyLightDesc()->bNonCull)
				continue;
			else if(fDistance <= pLight->Get_LightDesc()->fTempRange && false == pLight->Get_LightDesc()->bNonCull)
			{
				if (FAILED(pLight->Render(pShader, pVIBuffer, fDistance)))
					return E_FAIL;
			}
			else if (true == pLight->Get_LightDesc()->bNonCull)
			{
				if (FAILED(pLight->Render(pShader, pVIBuffer)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

_float4x4 CLight_Manager::Get_ShadowLightViewMatrix(_uint iLevelIndex)
{	
	_float4x4 IdentityMatrix;  
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	auto iter = m_ShadowViewMatrix.find(iLevelIndex);

	if (iter == m_ShadowViewMatrix.end())
		return IdentityMatrix;

	return iter->second;
}

_float4x4 CLight_Manager::Get_ShadowLightViewMatrix_Inverse(_uint iLevelIndex)
{
	_float4x4 LightViewMatrix;
	XMStoreFloat4x4(&LightViewMatrix, XMMatrixIdentity());
	auto iter = m_ShadowViewMatrix.find(iLevelIndex);

	if (iter == m_ShadowViewMatrix.end())
		return LightViewMatrix;

	XMStoreFloat4x4(&LightViewMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&iter->second)));
	return LightViewMatrix;
}

_float4x4 CLight_Manager::Get_ShadowLightProjMatrix(_uint iLevelIndex)
{
	_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	auto iter = m_ShadowProjMatrix.find(iLevelIndex);

	if (iter == m_ShadowProjMatrix.end())
		return IdentityMatrix;

	return iter->second;
}

XMMATRIX CLight_Manager::GetLightViewMatrix()
{
	CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);
	if (nullptr == pCameraManager)
		return XMMATRIX();
	
	CCamera* pCamera = pCameraManager->Get_CurCamera();
	if (nullptr == pCamera)
		return XMMATRIX();

	const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
	Vec3 vCamPos = Vec3(pCamera->Get_Transform()->Get_Position());
	Vec3 eyePos = Vec3(10.0f, 10.0f, 10.0f);
	Vec4 eyePosVec = Vec4(eyePos.x, eyePos.y, eyePos.z, 1.0f);
	Vec3 normalizeLightDir = pLightDesc->vTempDirection;
	normalizeLightDir.Normalize();
	Vec4 vTargetPosVec = Vec4(eyePos.x + normalizeLightDir.x, eyePos.y + normalizeLightDir.y, eyePos.z + normalizeLightDir.z, 1.0f);
	Vec4 vUp = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
	Matrix lightVIewMatrix = ::XMMatrixLookAtLH(eyePosVec, vTargetPosVec, vUp);

	return lightVIewMatrix;
}

HRESULT CLight_Manager::Reset_Lights()
{
	Free();
	return S_OK;
}

HRESULT CLight_Manager::Add_Light_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight* pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	pLight->Set_LightID(m_Lights_UI.size());
	m_Lights_UI.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_UI(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights_UI)
	{
		LIGHTDESC::TYPE eType = pLight->Get_LightDesc()->eType;

		if (LIGHTDESC::TYPE_DIRECTIONAL == pLight->Get_LightDesc()->eType)
		{
			if (FAILED(pLight->Render(pShader, pVIBuffer)))
				return E_FAIL;
		}
		else if ((LIGHTDESC::TYPE_POINT == eType) || (LIGHTDESC::TYPE_SPOT == eType))
		{
			CCamera_Manager* pCameraManager = GET_INSTANCE(CCamera_Manager);

			if (nullptr == pCameraManager)
				return S_OK;

			CCamera* pCamera = pCameraManager->Get_CurCamera();
			if (nullptr == pCamera)
				return S_OK;

			Vec3 pCameraPosition = pCamera->Get_Transform()->Get_Position();
			Vec3 pLightPosition = pLight->Get_LightDesc()->vTempPosition;

			Vec3 vLength = pCameraPosition - pLightPosition;
			_float fDistance = ::fabs(vLength.Length());

			if (fDistance > pLight->Get_LightDesc()->fTempRange && false == pLight->Get_ModifyLightDesc()->bNonCull)
				continue;
			else if (fDistance <= pLight->Get_LightDesc()->fTempRange && false == pLight->Get_LightDesc()->bNonCull)
			{
				if (FAILED(pLight->Render(pShader, pVIBuffer, fDistance)))
					return E_FAIL;
			}
			else if (true == pLight->Get_LightDesc()->bNonCull)
			{
				if (FAILED(pLight->Render(pShader, pVIBuffer)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CLight_Manager::Add_Sun(CGameObject* pSun)
{
	if (nullptr != m_pSun)
		return E_FAIL;

	m_pSun = pSun;

	return S_OK;
}

Vec4 CLight_Manager::Get_SunScreenPos()
{
	const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
	Vec4 light_ss{};
	{
		Vec4 vCamPos = GI->Get_CamPosition();
		Vec3 vLightDir = pLightDesc->vTempDirection;
		Vec4 vLightPos = vCamPos - vLightDir;

		Matrix view = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		Matrix proj = GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
		Matrix viewProj = view * proj;

		Vec4 ProjPos;
		ProjPos = Vec4::Transform(vLightPos, viewProj);

		ProjPos.x = (ProjPos.x + 1.0f) * 0.5f;
		ProjPos.y = (ProjPos.y - 1.0f) * -0.5f;

		light_ss.x = ProjPos.x;
		light_ss.y = ProjPos.y;
		light_ss.z = ProjPos.z / ProjPos.w;
		light_ss.w = 1.0f;
	}

	return light_ss;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);
	m_Lights.clear();

	for (auto& pLight : m_Lights_UI)
		Safe_Release(pLight);
	m_Lights_UI.clear();
}
