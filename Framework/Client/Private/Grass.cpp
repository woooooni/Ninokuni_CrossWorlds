#include "stdafx.h"
#include "Grass.h"
#include "GameInstance.h"
#include "Mesh.h"

CGrass::CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_GRASS)
{
}

CGrass::CGrass(const CGrass& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrass::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrass::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_CBGrass.fBendDelta = GI->RandomFloat(0.0f, 0.6f);
	m_pModelCom->Get_Meshes()[0]->Set_Topology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	return S_OK;
}

void CGrass::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime += fTimeDelta;
	if (m_fTime >= FLT_MAX - 0.1f)
		m_fTime = 0.0f;
}

void CGrass::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 10.0f))
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
}

HRESULT CGrass::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrass::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[GRASS_TEX::WIND_MAP]->Bind_ShaderResource(pInstancingShader, "WindTexture", 0)))
		return E_FAIL; 
	if (FAILED(m_pTextureCom[GRASS_TEX::SHADE_MAP]->Bind_ShaderResource(pInstancingShader, "BladeTexture", 0)))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, 0, pInstancingBuffer, WorldMatrices, 8)))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("vBottomColor", &vBottomColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("vUpperColor", &vUpperColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrass::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Grass"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_GrassPlane"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Grass_WindMap"), TEXT("Com_WindTexture"), (CComponent**)&m_pTextureCom[GRASS_TEX::WIND_MAP])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Grass_ShadeMap"), TEXT("Com_ShadeTexture"), (CComponent**)&m_pTextureCom[GRASS_TEX::SHADE_MAP])))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrass::Bind_ShaderResource()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("world", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("view", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("projection", &GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("fBendDelta", &m_CBGrass.fBendDelta, sizeof(_float))))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom[GRASS_TEX::WIND_MAP]->Bind_ShaderResource(m_pShaderCom, "WindTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[GRASS_TEX::SHADE_MAP]->Bind_ShaderResource(m_pShaderCom, "BladeTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CGrass* CGrass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrass* pInstance = new CGrass(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrass");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGrass::Clone(void* pArg)
{
	CGrass* pInstance = new CGrass(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGrass");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGrass::Free()
{
	__super::Free();

	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CVIBuffer_LensFlare*>(m_pVIBufferCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CModel*>(m_pModelCom);

	for (_uint i = 0; i < GRASS_TEX::GRASS_TEXEND; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}
