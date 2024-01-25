#include "stdafx.h"
#include "..\Public\Sun.h"
#include "GameInstance.h"

CSun::CSun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CSun::CSun(const CSun& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSun::Initialize_Prototype()
{	
	return S_OK;
}

HRESULT CSun::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSun::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSun::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
}

HRESULT CSun::Render()
{
	const LIGHTDESC* pLightDesc = GI->Get_LightDesc(0);
	Vec4 light_ss{};
	{
		Vec4 vCamPos = GI->Get_CamPosition();
		Vec3 vLightDir = pLightDesc->vTempDirection;
		Vec4 vLightPos = vCamPos - vLightDir;

		light_ss = vLightPos;
		light_ss.w = 1.0f;
	}

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, light_ss);

	if (FAILED(m_pShaderCom->Bind_RawValue("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("viewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("projectionMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSun::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Sun"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SkySun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CSun* CSun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CSun* pInstance = new CSun(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSun");
		Safe_Release<CSun*>(pInstance);
	}

	return pInstance;
}

CGameObject* CSun::Clone(void* pArg)
{
	CSun* pInstance = new CSun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSun");
		Safe_Release<CSun*>(pInstance);
	}

	return pInstance;
}

void CSun::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CModel*>(m_pModelCom);
}
