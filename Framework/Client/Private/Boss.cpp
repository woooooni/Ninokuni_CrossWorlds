#include "stdafx.h"
#include "Boss.h"

#include "GameInstance.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CBoss::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (true == m_bMotionTrail)
		Tick_MotionTrail(fTimeDelta);

}

void CBoss::LateTick(_float fTimeDelta)
{
	// __super::LateTick(fTimeDelta);
	LateUpdate_Collider(fTimeDelta);

	m_pControllerCom->LateTick_Controller(fTimeDelta);
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
#endif // DEBUG

}

HRESULT CBoss::Render()
{
	// __super::Render();

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	// Bloom -----------------
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;
	// ----------------- Bloom

	// RimLight -----------------
	_float4 vRimLightColor = _float4(0.f, 0.f, 0.f, 0.f);

	if (m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_COUNTER])
	{
		vRimLightColor = m_vCounterRimColor;
	}
	else if(m_bIsRimUse)
	{
		vRimLightColor = m_vRimLightColor;
	}
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimLightColor, sizeof(_float4))))
		return E_FAIL;
	// ----------------- RimLight

	// Dissolve -----------------
	if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// ----------------- Dissolve

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (m_bReserveDead)
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

HRESULT CBoss::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CBoss::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}

HRESULT CBoss::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}



void CBoss::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CBoss::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CBoss::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CBoss::Generate_MotionTrail(const MOTION_TRAIL_DESC& MotionTrailDesc)
{
	m_bMotionTrail = true;
	m_MotionTrailDesc = MotionTrailDesc;
}

void CBoss::Stop_MotionTrail()
{
	m_bMotionTrail = false;
}

CHierarchyNode* CBoss::Get_Socket(const wstring& strSocketName)
{
	return nullptr;
}

void CBoss::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}

HRESULT CBoss::Ready_Components()
{
	return S_OK;
}

HRESULT CBoss::Ready_States()
{
	return S_OK;
}

HRESULT CBoss::Ready_Colliders()
{
	return S_OK;
}

void CBoss::LookAt_DamagedObject(CGameObject* pAttacker)
{
}

void CBoss::Play_DamagedSound()
{
}

void CBoss::Tick_MotionTrail(_float fTimeDelta)
{
	m_MotionTrailDesc.fAccMotionTrail += fTimeDelta;
	if (m_MotionTrailDesc.fAccMotionTrail >= m_MotionTrailDesc.fMotionTrailTime)
	{
		m_MotionTrailDesc.fAccMotionTrail = 0.f;

		MOTION_TRAIL_DESC TrailDesc;
		TrailDesc.WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		TrailDesc.fAlphaSpeed = 1.f;
		TrailDesc.pModel = m_pModelCom;
		TrailDesc.TweenDesc = m_pModelCom->Get_TweenDesc();
		TrailDesc.vBloomPower = m_MotionTrailDesc.vBloomPower;
		TrailDesc.vRimColor = m_MotionTrailDesc.vRimColor;
		TrailDesc.fBlurPower = m_MotionTrailDesc.fBlurPower;


		TrailDesc.pRenderModel = m_pModelCom;
		if (nullptr != TrailDesc.pRenderModel)
		{
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
			{
				MSG_BOX("MotionTrail_Failed");
			}
		}
	}
}

void CBoss::Free()
{
	__super::Free();
}
