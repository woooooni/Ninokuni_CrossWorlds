#include "stdafx.h"
#include "GameInstance.h"
#include "Character_Projectile.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

CCharacter_Projectile::CCharacter_Projectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag)
	:CGameObject(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CHARACTER_PROJECTILE)
{

}

CCharacter_Projectile::CCharacter_Projectile(const CCharacter_Projectile& rhs)
	: CGameObject(rhs)
	, m_fAccDeletionTime(0.f)
	, m_fDeletionTime(rhs.m_fDeletionTime)
{

}


HRESULT CCharacter_Projectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter_Projectile::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	CHARACTER_PROJECTILE_DESC* pProjectileDesc = (CHARACTER_PROJECTILE_DESC*)pArg;
	if (nullptr == pProjectileDesc->pOwner)
		return E_FAIL;


	m_pOwner = pProjectileDesc->pOwner;
	Safe_AddRef(m_pOwner);

	return S_OK;
}

void CCharacter_Projectile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fAccDeletionTime += fTimeDelta;
	if (m_fAccDeletionTime >= m_fDeletionTime)
	{
		Set_Dead(true);
		m_fAccDeletionTime = 0.f;
	}
}

void CCharacter_Projectile::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.f))
	{
		m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
	}

	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);
#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CCharacter_Projectile::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint iPassIndex = 0;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}
	return S_OK;
}





void CCharacter_Projectile::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOwner);
}
