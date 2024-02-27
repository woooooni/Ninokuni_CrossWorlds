#include "stdafx.h"
#include "GameInstance.h"
#include "Engineer_BurstCall_Missile.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"
#include "Effect.h"

CEngineer_BurstCall_Missile::CEngineer_BurstCall_Missile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"Engineer_BurstCall_Missile")
{

}

CEngineer_BurstCall_Missile::CEngineer_BurstCall_Missile(const CEngineer_BurstCall_Missile& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CEngineer_BurstCall_Missile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEngineer_BurstCall_Missile::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STUN, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_fDeletionTime = 2.f;
	return S_OK;
}

void CEngineer_BurstCall_Missile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEngineer_BurstCall_Missile::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG
}




HRESULT CEngineer_BurstCall_Missile::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 5.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CEngineer_BurstCall_Missile::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Ele_Impact_Fire_" + to_wstring(GI->RandomInt(4, 8)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
	
}


CEngineer_BurstCall_Missile* CEngineer_BurstCall_Missile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngineer_BurstCall_Missile* pInstance = new CEngineer_BurstCall_Missile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEngineer_BurstCall_Missile");
		Safe_Release<CEngineer_BurstCall_Missile*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEngineer_BurstCall_Missile::Clone(void* pArg)
{
	CEngineer_BurstCall_Missile* pInstance = new CEngineer_BurstCall_Missile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEngineer_BurstCall_Missile");
		Safe_Release<CEngineer_BurstCall_Missile*>(pInstance);
	}

	return pInstance;
}

void CEngineer_BurstCall_Missile::Free()
{
	__super::Free();
}
