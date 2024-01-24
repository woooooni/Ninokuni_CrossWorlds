#include "stdafx.h"
#include "GameInstance.h"
#include "Destroyer_HyperStrike_Hammer.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"
#include "Effect.h"

CDestroyer_HyperStrike_Hammer::CDestroyer_HyperStrike_Hammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"Destroyer_HyperStrike_Hammer")
{

}

CDestroyer_HyperStrike_Hammer::CDestroyer_HyperStrike_Hammer(const CDestroyer_HyperStrike_Hammer& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CDestroyer_HyperStrike_Hammer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDestroyer_HyperStrike_Hammer::Initialize(void* pArg)
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

void CDestroyer_HyperStrike_Hammer::Tick(_float fTimeDelta)
{
	if (nullptr == m_pEffect)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_HyperStrike_Hammer"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.2f, 0.f), _float3(10.f, 10.f, 10.f), _float3(0.f, 60.f, 40.f), this, &m_pEffect, false);
		Safe_AddRef(m_pEffect);
	}

	__super::Tick(fTimeDelta);

	m_fAccOnOff += fTimeDelta;
	if (m_fAccOnOff >= m_fOnOffTime)
	{
		m_fAccOnOff = 0.f;
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, !Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active());
	}
}

void CDestroyer_HyperStrike_Hammer::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG
}




HRESULT CDestroyer_HyperStrike_Hammer::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 10.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CDestroyer_HyperStrike_Hammer::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Ele_Impact_Fire_" + to_wstring(GI->RandomInt(4, 8)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
	
}


CDestroyer_HyperStrike_Hammer* CDestroyer_HyperStrike_Hammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDestroyer_HyperStrike_Hammer* pInstance = new CDestroyer_HyperStrike_Hammer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDestroyer_HyperStrike_Hammer");
		Safe_Release<CDestroyer_HyperStrike_Hammer*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDestroyer_HyperStrike_Hammer::Clone(void* pArg)
{
	CDestroyer_HyperStrike_Hammer* pInstance = new CDestroyer_HyperStrike_Hammer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDestroyer_HyperStrike_Hammer");
		Safe_Release<CDestroyer_HyperStrike_Hammer*>(pInstance);
	}

	return pInstance;
}

void CDestroyer_HyperStrike_Hammer::Free()
{
	__super::Free();

	if (nullptr != m_pEffect)
	{
		m_pEffect->Set_Dead(true);
		Safe_Release(m_pEffect);
	}
}
