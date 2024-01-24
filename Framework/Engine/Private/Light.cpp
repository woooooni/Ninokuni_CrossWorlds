#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

#pragma pack(push,1)
struct CB_DIRECTIONAL
{
	Vec4 vAmbientLower;
	Vec4 vAmbientRange;
	Vec4 vDirToLight;
	Vec4 vDirectionalColor;
};
#pragma pack(pop)

const Vec4 GammaToLinear(const Vec4& vColor)
{
	return Vec4(vColor.x * vColor.x, vColor.y * vColor.y, vColor.z * vColor.z, vColor.w * vColor.w);
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
	_uint		iPassIndex = 1;

#pragma region °³Á¶ LIGHT
	//switch (m_LightDesc.eType)
	//{
	//case LIGHTDESC::TYPE_DIRECTIONAL:
	//	if (FAILED(DirectionalLight(pShader)))
	//		return E_FAIL;
	//	break;
	//case LIGHTDESC::TYPE_POINT:
	//	break;
	//case LIGHTDESC::TYPE_SPOT:
	//	break;
	//default:
	//	break;
	//}
#pragma endregion 
	if (LIGHTDESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;

		iPassIndex = 2;
	}
	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	pShader->Begin(iPassIndex);
	pVIBuffer->Render();

	return S_OK;
}

HRESULT CLight::DirectionalLight(class CShader* pShader)
{
	m_LightDesc.vTempDirection.Normalize();
	if (FAILED(pShader->Bind_RawValue("vAmbientLower", &m_LightDesc.vAmbientLowerColor, sizeof(Vec3))))
		return E_FAIL;
	Vec3 vAmbientRange = m_LightDesc.vAmbientUpperColor - m_LightDesc.vAmbientLowerColor;
	if (FAILED(pShader->Bind_RawValue("vAmbientRange", &vAmbientRange, sizeof(Vec3))))
		return E_FAIL;
	Vec3 vInverseLightDir = -m_LightDesc.vTempDirection;
	if (FAILED(pShader->Bind_RawValue("vDirToLight", &vInverseLightDir, sizeof(Vec3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("vDirectionalColor", &m_LightDesc.vTempColor, sizeof(Vec3))))
		return E_FAIL;

	pShader->Begin(1);
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
