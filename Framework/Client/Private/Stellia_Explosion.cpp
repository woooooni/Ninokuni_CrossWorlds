#include "stdafx.h"
#include "Stellia_Explosion.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Decal.h"

CStellia_Explosion::CStellia_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CStellia_Explosion::CStellia_Explosion(const CStellia_Explosion& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CStellia_Explosion::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CStellia_Explosion::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_Stat().eElementType);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 50.f, true);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	m_fDeletionTime = 2.f;
	m_fTime = 0.f;

	// Effect Create
	// GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Icicle_Back"), m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), this);

	return S_OK;
}

void CStellia_Explosion::Tick(_float fTimeDelta)
{
	if (m_pDecal == nullptr)
	{
		CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), m_pTransformCom->Get_WorldMatrix(),
			Vec3(0.f, 0.f, 0.f), Vec3(2.5f, 5.f, 2.5f), Vec3(0.f, 0.f, 0.f), nullptr, &m_pDecal, false);
		Safe_AddRef(m_pDecal);
	}
	else
	{
		m_fTime += fTimeDelta;
		// 지우기 
		if (m_fTime >= m_pDecal->Get_DecalDesc().fLifeTime)
		{
			this->Set_Dead(true);
			return;
		}
		// 콜라이더 끄기
		if (m_fTime >= m_pDecal->Get_DecalDesc().fLifeTime - 1.5f)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
		}

		// 임시로. 나중에는 데칼이 일정 크기만큼 다 커지면 펑퍼러러펑펑
		if (m_fTime >= m_pDecal->Get_DecalDesc().fLifeTime - 2.f)
		{
			if (!m_bIsExplosion)
			{
				m_bIsExplosion = true;
				Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CStellia_Explosion::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

void CStellia_Explosion::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CStellia_Explosion::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CStellia_Explosion::Collision_Exit(const COLLISION_INFO& tInfo)
{
	
}

HRESULT CStellia_Explosion::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CStellia_Explosion::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = {};
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CStellia_Explosion* CStellia_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CStellia_Explosion* pInstance = new CStellia_Explosion(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CStellia_Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CStellia_Explosion::Clone(void* pArg)
{
	CStellia_Explosion* pInstance = new CStellia_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStellia_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia_Explosion::Free()
{
	__super::Free();

	if (m_pDecal != nullptr)
	{
		m_pDecal->Set_Dead(true);
		Safe_Release(m_pDecal);
	}
}

