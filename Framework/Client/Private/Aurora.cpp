#include "..\Public\Aurora.h"
#include "Aurora.h"
#include "GameInstance.h"

CAurora::CAurora(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag,eType)
{

}

CAurora::CAurora(const CAurora& rhs)
	: CGameObject(rhs)
{
}

HRESULT CAurora::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAurora::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_strObjectTag == TEXT("Sky_Aurora_Reverse"))
		m_PSAuroraDesc.fGlowWaveSpeed *= -1.f;

	return S_OK;
}

void CAurora::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime >= FLT_MAX - 5.0f)
		m_fTime = 0.0f;
}

void CAurora::LateTick(_float fTimeDelta)
{
	Vec4 vCamePos = GI->Get_CamPosition();

	// m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamePos);
	 m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_AURORA, this);
}

HRESULT CAurora::Render()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("viewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("projectionMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("VS_AuroraDesc", &m_VSAuroraDesc, sizeof(VS_AURORA_DESC))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("PS_AuroraDesc", &m_PSAuroraDesc, sizeof(PS_AURORA_DESC))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	//_int iIndex = m_pTextureCom[LEVEL_AURORA::WINTER_AURORA]->Find_Index(TEXT("T_Aura_001"));
	//if (FAILED(m_pTextureCom[LEVEL_AURORA::WINTER_AURORA]->Bind_ShaderResource(m_pShaderCom, "AuroraTexture")))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(0)))
	//	return E_FAIL;
	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "AuroraTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CAurora::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	// Aurora Shader.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Aurora"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	//	return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aurora"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Aura"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[LEVEL_AURORA::WINTER_AURORA]))))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Aurora"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[LEVEL_AURORA::WINTER_AURORA]))))
	//	return E_FAIL;

	return S_OK;
}

CAurora* CAurora::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CAurora* pInstance = new CAurora(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAurora");
		Safe_Release<CAurora*>(pInstance);
	}

	return pInstance;
}

CGameObject* CAurora::Clone(void* pArg)
{
	CAurora* pInstance = new CAurora(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAurora");
		Safe_Release<CAurora*>(pInstance);
	}

	return pInstance;
}

void CAurora::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);
	Safe_Release<CModel*>(m_pModelCom);

	for (_uint i = 0; i < LEVEL_AURORA::AURORA_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}
