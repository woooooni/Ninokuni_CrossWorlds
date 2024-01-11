#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "GameObject.h"
#include "GameInstance.h"
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
		_float4x4 LightViewMatrix;
		XMStoreFloat4x4(&LightViewMatrix, XMMatrixLookAtLH(vEye, vAt, vUp));

		iter->second = LightViewMatrix;
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
		pLight->Render(pShader, pVIBuffer);

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

HRESULT CLight_Manager::Reset_Lights()
{
	Free();
	return S_OK;
}

HRESULT CLight_Manager::Add_Sun(CGameObject* pSun)
{
	if (nullptr != m_pSun)
		return E_FAIL;

	m_pSun = pSun;

	return S_OK;
}

Vec3 CLight_Manager::Get_SunScreenPos()
{
	if (nullptr == m_pSun)
		return Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

	CTransform* pTransform = m_pSun->Get_Component<CTransform>(TEXT("Com_Transform"));
	Vec3 vSunWorldPos = pTransform->Get_Position();

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	Matrix view = pPipeLine->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	Matrix proj = pPipeLine->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	Matrix viewProj = view * proj;

	Vec3 ProjPos;
	ProjPos = ::XMVector3TransformCoord(vSunWorldPos, viewProj);

	ProjPos.x = (ProjPos.x + 1.0f) * 0.5f;
	ProjPos.y = (ProjPos.y - 1.0f) * -0.5f;

	return ProjPos;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
