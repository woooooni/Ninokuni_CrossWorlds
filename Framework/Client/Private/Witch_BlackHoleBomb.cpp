#include "stdafx.h"
#include "Witch_BlackHoleBomb.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

CWitch_BlackHoleBomb::CWitch_BlackHoleBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_BlackHoleBomb::CWitch_BlackHoleBomb(const CWitch_BlackHoleBomb& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_BlackHoleBomb::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWitch_BlackHoleBomb::Initialize(void* pArg)
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
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_BlackHole_Drain"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pEffectBomb);

	return S_OK;
}

void CWitch_BlackHoleBomb::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWitch_BlackHoleBomb::LateTick(_float fTimeDelta)
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

void CWitch_BlackHoleBomb::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CWitch_BlackHoleBomb::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWitch_BlackHoleBomb::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWitch_BlackHoleBomb::Ready_Components()
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
HRESULT CWitch_BlackHoleBomb::Ready_Colliders()
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

CWitch_BlackHoleBomb* CWitch_BlackHoleBomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_BlackHoleBomb* pInstance = new CWitch_BlackHoleBomb(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_BlackHoleBomb");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_BlackHoleBomb::Clone(void* pArg)
{
	CWitch_BlackHoleBomb* pInstance = new CWitch_BlackHoleBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_BlackHoleBomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_BlackHoleBomb::Free()
{
	__super::Free();
}

