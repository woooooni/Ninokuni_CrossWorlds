#include "stdafx.h"
#include "Stellia_Crystal_Destructible.h"

#include "GameInstance.h"
#include "Animation.h"

#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"

#include "Particle_Manager.h"

#include "Stellia.h"

#include "Stellia_Crystal_FailBomb.h"

CStellia_Crystal_Destructible::CStellia_Crystal_Destructible(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia_Crystal_Destructible::CStellia_Crystal_Destructible(const CStellia_Crystal_Destructible& rhs)
	: CMonster(rhs)
{
}

HRESULT CStellia_Crystal_Destructible::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		Vec4 vPos = *(Vec4*)pArg;
		vPos.y -= 3.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransformCom->Set_Scale(Vec3(1.5f, 1.5f, 1.5f));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(1);

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType, 2.5f);

	m_vBloomPower = _float3(0.7f, 0.7f, 0.7f);

	return S_OK;
}

void CStellia_Crystal_Destructible::Tick(_float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_GachaCommonCrystal03.ao|GachaCommonCrystalStart") &&
		m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pModelCom->Set_Animation(TEXT("SKM_GachaCommonCrystal03.ao|GachaCommonCrystalLoop"));
	}

	if (m_tStat.fHp <= 0.f)
	{
		if (m_iSelfType != m_iBingoType)
		{
			// Set_Dead ÇÏ±â Àü¿¡ Æø¹ß °´Ã¼ »ý¼º ¹× ½ºÅÚ¸®¾Æ ÆäÀÏ Ä«¿îÆ® Ãß°¡
			GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Stellia_Crystal_FailBomb"), this);

			m_pStellia->Set_CrystalFailCount(1);
		}
		else if (m_iSelfType == m_iBingoType)
		{
			m_pStellia->Set_CrystalBingoCount(1);
		}


		this->Set_Dead(true);
	}

	if (nullptr != m_pHPBar)
		m_pHPBar->Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);

	if (m_bIsRimUse) // RimLight
	{
		m_vRimLightColor = { 1.f, 0.f, 0.f, 1.f };
		Tick_RimLight(fTimeDelta);
	}
	else
	{
		m_vRimLightColor = { 0.f, 0.f, 0.f, 0.f };
	}

	if (m_bReserveDead) // Dissolve
	{
		if (!m_bDissolveEffect)
		{
			m_bDissolveEffect = true;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Monster_Dissolve"), m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pDissolveObject);
		}
		else if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
			return;
		}
		else
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
	}

}

void CStellia_Crystal_Destructible::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);*/
	// m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());

	m_AnimInstanceDesc.fDissolveDuration = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveSpeed = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveTotal = m_fDissolveTotal;
	m_AnimInstanceDesc.fDissolveWeight = m_fDissolveWeight;
	m_AnimInstanceDesc.vDissolveColor = m_vDissolveColor;
	m_AnimInstanceDesc.vRimColor = m_vRimLightColor;

	Compute_CamZ(m_pTransformCom->Get_Position());
	if (m_fCamDistance <= 50.f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 10.f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
	}

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG

}

HRESULT CStellia_Crystal_Destructible::Render()
{
	__super::Render();

	if (nullptr != m_pHPBar)
		m_pHPBar->Render();

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	// __super::Render();

	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
		return E_FAIL;

	// Bloom -----------------
	if (FAILED(pInstancingShader->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;
	// ----------------- Bloom

	// RimLight -----------------

	// ----------------- RimLight

	// Dissolve -----------------
	if (FAILED(m_pDissoveTexture->Bind_ShaderResource(pInstancingShader, "g_DissolveTexture", 51)))
		return E_FAIL;
	// ----------------- Dissolve

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (i != 2)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(pInstancingShader, "g_DiffuseTexture", m_iSelfType)))
				return E_FAIL;
		}

		if (m_bReserveDead)
			iPassIndex = 2;
		else if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (i != 2)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iSelfType)))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}


	return S_OK;
}

void CStellia_Crystal_Destructible::Collision_Enter(const COLLISION_INFO& tInfo)
{
	/* ÇÇ°Ý */
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
		{
			On_Damaged(tInfo);
		}
	}
}

void CStellia_Crystal_Destructible::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStellia_Crystal_Destructible::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CStellia_Crystal_Destructible::Set_CrystalData(STELLIA_CRYSTAL_DESC* pDesc)
{
	m_iSelfType = pDesc->iSelfType;
	m_iBingoType = pDesc->iBingoType;
	m_pStellia = dynamic_cast<CStellia*>(pDesc->pStellia);
}

HRESULT CStellia_Crystal_Destructible::Ready_Components()
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

HRESULT CStellia_Crystal_Destructible::Ready_States()
{
	strKorName = TEXT("¾Ç¸ù Å©¸®½ºÅ»");
	strSubName = TEXT("¸¶³àÀÇ ½£");
	m_tStat.eElementType = ELEMENTAL_TYPE::FIRE;

	m_tStat.iLv = 15;
	m_tStat.fMaxHp = 15000;
	m_tStat.fHp = 15000;
	m_tStat.iAtk = 250;
	m_tStat.iDef = 0;

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 20.f, 75.f, 20.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 75.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CStellia_Crystal_Destructible* CStellia_Crystal_Destructible::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia_Crystal_Destructible* pInstance = new CStellia_Crystal_Destructible(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CStellia_Crystal_Destructible");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStellia_Crystal_Destructible::Clone(void* pArg)
{
	CStellia_Crystal_Destructible* pInstance = new CStellia_Crystal_Destructible(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStellia_Crystal_Destructible");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia_Crystal_Destructible::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pHPBar);
}
