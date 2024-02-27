#include "stdafx.h"
#include "Stellia_Crystal.h"

#include "GameInstance.h"

#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"

#include "Particle_Manager.h"

#include "Stellia.h"

CStellia_Crystal::CStellia_Crystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, LAYER_TYPE::LAYER_PROP)
{
}

CStellia_Crystal::CStellia_Crystal(const CStellia_Crystal& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStellia_Crystal::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStellia_Crystal::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	Vec4 vPos = *(Vec4*)pArg;
	vPos.y += 8.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Set_Scale(Vec3(4.f, 4.f, 4.f));

	m_vBloomPower = _float3(1.f, 1.f, 1.f);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(1);

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// Start Dissolve
	m_bStartDissolve = true;
	m_fDissolveWeight = 10.f;
	m_pDissoveTexture = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));
	if (m_pDissoveTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

void CStellia_Crystal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// Start Dissolve
	if (true == m_bStartDissolve)
	{
		switch (m_iCrystalType)
		{
		case 0: // CRYSTAL_AURA
			m_vDissolveColor = _float4(0.212f, 0.620f, 1.f, 1.f);
			break;
		case 1: // CRYSTAL_SKY
			m_vDissolveColor = _float4(0.715f, 0.995f, 1.f, 1.f);
			break;
		case 2: // CRYSTAL_GOLD
			m_vDissolveColor = _float4(1.f, 0.96f, 0.466f, 1.f);
			break;
		}

		m_fDissolveWeight -= m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight < 0.f)
		{
			m_fDissolveWeight = 0.f;
			m_bStartDissolve = false;
		}
	}

	GI->Add_CollisionGroup(COLLISION_GROUP::PROP, this);

	if (m_bReserveDead) // Dissolve
	{
		if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
			return;
		}
		else
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
	}
	else if (m_pModelCom->Is_Finish())
		m_bReserveDead = true;
}

void CStellia_Crystal::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // _DEBUG
}

HRESULT CStellia_Crystal::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 1.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3	))))
		return E_FAIL;

	// Dissolve --------------------------------------------------------------------
	if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// -----------------------------------------------------------------------------

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (i != 2)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iCrystalType)))
				return E_FAIL;
		}

		if (m_bStartDissolve || m_bReserveDead)
			iPassIndex = 2;
		else if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStellia_Crystal::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CStellia_Crystal::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Stellia_Crystals"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stellia_Crystal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStellia_Crystal::Ready_Colliders()
{
	return S_OK;
}

CStellia_Crystal* CStellia_Crystal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CStellia_Crystal* pInstance = new CStellia_Crystal(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CStellia_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStellia_Crystal::Clone(void* pArg)
{
	CStellia_Crystal* pInstance = new CStellia_Crystal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStellia_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia_Crystal::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pDissoveTexture);
}
