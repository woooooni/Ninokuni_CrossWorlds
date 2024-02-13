#include "stdafx.h"
#include "Stellia_Crystal_Explosion.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "GameInstance.h"

#include "Stellia_Crystal_Destructible.h"
#include "Particle_Manager.h"

CStellia_Crystal_Explosion::CStellia_Crystal_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CStellia_Crystal_Explosion::CStellia_Crystal_Explosion(const CStellia_Crystal_Explosion& rhs)
	: CMonsterProjectile(rhs)
	, m_fTime(0.f)
	, m_fDelteTime(rhs.m_fDelteTime)
{

}

HRESULT CStellia_Crystal_Explosion::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStellia_Crystal_Explosion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_Stat().eElementType);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::BLOW, 4.f, 8.f, 0.f, true);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	m_fTime = 0.f;
	m_fDelteTime = 2.5f;

	return S_OK;
}

void CStellia_Crystal_Explosion::Tick(_float fTimeDelta)
{
	// Effect Create
	if (false == m_bEffect)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Crystal_Explosion_Circle"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_bEffect = true;
	}

	m_fTime += fTimeDelta;

	__super::Tick(fTimeDelta);

	if (m_fTime > 0.5f)
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	// 나중에는 이펙트 재생이 끝나면
	if (m_fTime > m_fDelteTime)
	{
		m_fTime = m_fDelteTime - m_fTime;
		// Reserve_Dead(true);
		Set_Dead(true);
	}

}

void CStellia_Crystal_Explosion::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CStellia_Crystal_Explosion::Render()
{
	return S_OK;
}

HRESULT CStellia_Crystal_Explosion::Render_ShadowDepth()
{
	//if (FAILED(__super::Render_ShadowDepth()))
	//	return E_FAIL;

	return S_OK;
}


void CStellia_Crystal_Explosion::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		tInfo.pOther->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-tInfo.pOther->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Look(), 5.f, false);

		tInfo.pOther->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			{0.f, 1.f, 0.f}, 5.f, false);
	}
}

void CStellia_Crystal_Explosion::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStellia_Crystal_Explosion::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CStellia_Crystal_Explosion::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_pOwner != nullptr)
	{
		/* For.Com_Transform */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
			return E_FAIL;

		m_pCrystal = dynamic_cast<CStellia_Crystal_Destructible*>(m_pOwner);

		m_pTransformCom->Set_WorldMatrix(m_pCrystal->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix());

		Vec4 vBombPos = m_pTransformCom->Get_Position();
		vBombPos.y += 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vBombPos);
		// m_pTransformCom->Set_Scale(Vec3(1.f, 1.f, 1.f));
	}
	else
	{
		MSG_BOX("Fail Create : Crystal Bomb, m_pOwner == nullptr");
		return E_FAIL;
	}


	/* For.Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_GlanixIceBall"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	//	return E_FAIL;


	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CStellia_Crystal_Explosion::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 5.f;
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

CStellia_Crystal_Explosion* CStellia_Crystal_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CStellia_Crystal_Explosion* pInstance = new CStellia_Crystal_Explosion(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CStellia_Crystal_Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CStellia_Crystal_Explosion::Clone(void* pArg)
{
	CStellia_Crystal_Explosion* pInstance = new CStellia_Crystal_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStellia_Crystal_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia_Crystal_Explosion::Free()
{
	__super::Free();
}

