#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"


const Vec3 GammaToLinear(const Vec3& vColor)
{
	return Vec3(vColor.x * vColor.x, vColor.y * vColor.y, vColor.z * vColor.z);
}

CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLight::Initialize(const LIGHTDESC & LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

HRESULT CLight::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	switch (m_LightDesc.eType)
	{
	case LIGHTDESC::TYPE_DIRECTIONAL:
		if (FAILED(DirectionalLight(pShader)))
			return E_FAIL;
		break;
	case LIGHTDESC::TYPE_POINT:
		if (FAILED(PointLight(pShader)))
			return E_FAIL;
		break;
	case LIGHTDESC::TYPE_SPOT:
		if (FAILED(SpotLight(pShader)))
			return E_FAIL;
		break;
	default:
		break;
	}
	pVIBuffer->Render();

	return S_OK;
}

HRESULT CLight::DirectionalLight(class CShader* pShader)
{
	m_LightDesc.vTempDirection.Normalize();
	Vec3 vAmbientLowerToGamma = GammaToLinear(m_LightDesc.vAmbientLowerColor);
	if (FAILED(pShader->Bind_RawValue("vLightAmbientDown", &vAmbientLowerToGamma, sizeof(Vec3))))
		return E_FAIL;
	Vec3 vAmbientRangeToGamma = GammaToLinear(m_LightDesc.vAmbientUpperColor) - GammaToLinear(m_LightDesc.vAmbientLowerColor);
	if (FAILED(pShader->Bind_RawValue("vLightAmbientUp", &vAmbientRangeToGamma, sizeof(Vec3))))
		return E_FAIL;
	Vec3 vInverseLightDir = -m_LightDesc.vTempDirection;
	if (FAILED(pShader->Bind_RawValue("vDirToLight", &vInverseLightDir, sizeof(Vec3))))
		return E_FAIL;
	Vec3 vLightDiffuseToGamma = GammaToLinear(m_LightDesc.vTempColor);
	if (FAILED(pShader->Bind_RawValue("vDirLightColor", &vLightDiffuseToGamma, sizeof(Vec3))))
		return E_FAIL;

	pShader->Begin(1);
	return S_OK;
}

HRESULT CLight::PointLight(CShader* pShader)
{
	if(FAILED(pShader->Bind_RawValue("vPointLightPos", &m_LightDesc.vTempPosition, sizeof(Vec3))))
		return E_FAIL;
	
	_float PointLightRange = 1.0f / m_LightDesc.fTempRange;
	if(FAILED(pShader->Bind_RawValue("fPointLightRangeRcp", &PointLightRange, sizeof(_float))))
		return E_FAIL;

	Vec3 vLightDiffuseToGamma = GammaToLinear(m_LightDesc.vTempColor);
	if (FAILED(pShader->Bind_RawValue("vPointColor", &vLightDiffuseToGamma, sizeof(Vec3))))
		return E_FAIL;

	pShader->Begin(2);
	return S_OK;
}

HRESULT CLight::SpotLight(CShader* pShader)
{
	_float fCosInnerAngle = ::cosf(m_LightDesc.fInnerAngle);
	_float fSinOuterAngle = ::sinf(m_LightDesc.fOuterAngle);
	_float fCosOuterAngle = ::cosf(m_LightDesc.fOuterAngle);

	Vec3 vSpotLightPos = m_LightDesc.vTempPosition;
	m_LightDesc.vTempDirection.Normalize();
	Vec3 vDirToLight = -m_LightDesc.vTempDirection;
	_float fSpotLightRangeRcp = 1.0f / m_LightDesc.fTempRange;
	_float fSpotCosOuterCone = fCosOuterAngle;
	_float fSpotInnerConeRcp = 1.0f / fCosInnerAngle;
	Vec3 vGammaToSpotColor = GammaToLinear(m_LightDesc.vTempColor);

	if (FAILED(pShader->Bind_RawValue("vSpotLightPos", &vSpotLightPos, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("fSpotLightRangeRcp", &fSpotLightRangeRcp, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("vSpotDirToLight", &vDirToLight, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("fSpotCosOuterCone", &fSpotCosOuterCone, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("vSpotColor", &vGammaToSpotColor, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("fSpotCosInnerConeRcp", &fSpotInnerConeRcp, sizeof(_float))))
		return E_FAIL;

	pShader->Begin(8);

	return S_OK;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight*			pInstance = new CLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed To Created : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
