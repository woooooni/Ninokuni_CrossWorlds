#include "stdafx.h"
#include "Particle.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Utils.h"
#include "VIBuffer_Particle.h"



CParticle::CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const wstring& strEffectName)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
	, m_strPrototypeEffectTag(strEffectName)
{

}

CParticle::CParticle(const CParticle& rhs)
	: CGameObject(rhs)
	, m_tParticleDesc(rhs.m_tParticleDesc)
	, m_strPrototypeEffectTag(rhs.m_strPrototypeEffectTag)
{
	
}

HRESULT CParticle::Initialize_Prototype(const PARTICLE_DESC& ParticleDesc)
{
	m_tParticleDesc = ParticleDesc;

	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	if(m_pVIBufferCom->Get_Finished())
		Set_Dead(true);
	else
		m_pVIBufferCom->Tick(fTimeDelta);
}

void CParticle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CParticle::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render(m_tParticleDesc.iNumEffectCount);

	return S_OK;
}

HRESULT CParticle::Bind_ShaderResource()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	// ÅØ½ºÃ³
	if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

Client::CEffect* CParticle::Generate_Effect()
{

	//CEffect::EFFECT_DESC EffectDesc = pEffect->Get_EffectDesc();
	//EffectDesc.bBillboard = m_tParticleDesc.bBillboard;
	//

	//_float fSpeed = m_tParticleDesc.fSpeed;
	//if (m_tParticleDesc.bRandomSpeed)
	//{
	//	fSpeed = CUtils::Random_Float(1.f, fSpeed);
	//}
	//EffectDesc.fMoveSpeed = fSpeed;



	//_float3 vDir = m_tParticleDesc.vDir;
	//if (m_tParticleDesc.bRandomDir)
	//{
	//	vDir.x = CUtils::Random_Float(-vDir.x, vDir.x);
	//	vDir.y = CUtils::Random_Float(0.f, vDir.y);
	//	vDir.z = CUtils::Random_Float(-vDir.z, vDir.z);
	//}
	//XMStoreFloat3(&EffectDesc.vMoveDir, XMVector3Normalize(XMLoadFloat3(&vDir)));

	//pEffect->Set_Gravity(m_tParticleDesc.bRigidActive);
	//_float3 vForceDir = m_tParticleDesc.vForceDir;
	//if (m_tParticleDesc.bRandomForceDir)
	//{
	//	vForceDir.x = CUtils::Random_Float(-vForceDir.x, vForceDir.x);
	//	vForceDir.y = CUtils::Random_Float(-vForceDir.y, vForceDir.y);
	//	vForceDir.z = CUtils::Random_Float(-vForceDir.z, vForceDir.z);
	//}

	//_float fFrictionScale = m_tParticleDesc.fFrictionScale;
	//if (m_tParticleDesc.bRandomFriction)
	//{
	//	fFrictionScale = CUtils::Random_Float(0.f, fFrictionScale);
	//}
	//pEffect->Get_RigidBodyCom()->Set_Friction_Scale(fFrictionScale);
	//pEffect->Get_RigidBodyCom()->Add_Velocity(XMVector3Normalize(XMLoadFloat3(&vForceDir)), m_tParticleDesc.fForce);

	//EffectDesc.fBlurPower = m_tParticleDesc.fBlurPower;
	//EffectDesc.fDestAlphaSpeed = m_tParticleDesc.fDestAlphaSpeed;
	//
	//pEffect->Set_EffectDesc(EffectDesc);

	//pEffect->Set_DeletionTime(m_tParticleDesc.fLifeTime);
	
	// return pEffect;
	return nullptr;
}

HRESULT CParticle::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC tBufferInfo = {};
	//tBufferInfo.iNumCount = m_tParticleDesc.iNumEffectCount;
	tBufferInfo.bLoop = m_tParticleDesc.bLoop;
	tBufferInfo.bSameRate = m_tParticleDesc.bSameRate;
	tBufferInfo.fScale = m_tParticleDesc.fScale;
	tBufferInfo.fRange = m_tParticleDesc.fRange;
	tBufferInfo.vVelocityMin = m_tParticleDesc.vVelocityMin;
	tBufferInfo.vVelocityMax = m_tParticleDesc.vVelocityMax;
	tBufferInfo.fSpeed = m_tParticleDesc.fSpeed;
	tBufferInfo.fLifeTime = m_tParticleDesc.fLifeTime;
	tBufferInfo.fBoxMin = m_tParticleDesc.fBoxMin;
	tBufferInfo.fBoxMax = m_tParticleDesc.fBoxMax;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &tBufferInfo)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Diffuse_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test"), TEXT("Com_Diffuse_Texture"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;

	return S_OK;
}

CParticle* CParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const wstring& strEffectName, const PARTICLE_DESC& ParticleDesc)
{
	CParticle* pInstance = new CParticle(pDevice, pContext, strObjectTag, strEffectName);
	if (FAILED(pInstance->Initialize_Prototype(ParticleDesc)))
	{
		MSG_BOX("Failed to Created : CParticle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle::Clone(void* pArg)
{
	CParticle* pInstance = new CParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CParticle::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
}