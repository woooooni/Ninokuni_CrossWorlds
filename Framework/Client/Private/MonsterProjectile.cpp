#include "stdafx.h"
#include "MonsterProjectile.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Glanix.h"

CMonsterProjectile::CMonsterProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
{
}

CMonsterProjectile::CMonsterProjectile(const CMonsterProjectile& rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonsterProjectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterProjectile::Initialize(void* pArg)
{
	if (pArg != nullptr)
	{
		m_pOwner = (CMonster*)pArg;
		Safe_AddRef(m_pOwner);
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonsterProjectile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);

	if(m_pRigidBodyCom != nullptr)
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	if (m_pControllerCom != nullptr)
		m_pControllerCom->Tick_Controller(fTimeDelta);
}

void CMonsterProjectile::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_pControllerCom != nullptr)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

#ifdef NDEBUG
	m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif

}

HRESULT CMonsterProjectile::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonsterProjectile::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	_uint iPassIndex = 0;
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}

	return S_OK;
}


void CMonsterProjectile::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_Dead(this);
	}
}

void CMonsterProjectile::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CMonsterProjectile::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CMonsterProjectile::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CMonsterProjectile::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CMonsterProjectile::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CMonsterProjectile::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CMonsterProjectile::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pRigidBodyCom);

	Safe_Release(m_pOwner);
}

