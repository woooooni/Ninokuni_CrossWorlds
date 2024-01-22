#include "stdafx.h"
#include "GameInstance.h"
#include "Destroyer_FrengeCharge_Thunder.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"
#include "Effect.h"

CDestroyer_FrengeCharge_Thunder::CDestroyer_FrengeCharge_Thunder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"Destroyer_FrengeCharge_Thunder")
{

}

CDestroyer_FrengeCharge_Thunder::CDestroyer_FrengeCharge_Thunder(const CDestroyer_FrengeCharge_Thunder& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CDestroyer_FrengeCharge_Thunder::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDestroyer_FrengeCharge_Thunder::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STUN, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_fDeletionTime = 1.f;
	return S_OK;
}

void CDestroyer_FrengeCharge_Thunder::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fAccOnOff += fTimeDelta;
	if (m_fAccOnOff >= m_fOnOffTime)
	{
		m_fAccOnOff = 0.f;
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, !Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active());
	}
}

void CDestroyer_FrengeCharge_Thunder::LateTick(_float fTimeDelta)
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




HRESULT CDestroyer_FrengeCharge_Thunder::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CCollider_AABB::AABB_COLLIDER_DESC AABBDesc;
	BoundingBox AABBBox;
	AABBBox.Extents = { 3.f, 10.f, 3.f};
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, 10.f, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::ATTACK, &AABBDesc)))
		return E_FAIL;


	return S_OK;
}

void CDestroyer_FrengeCharge_Thunder::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Ele_Impact_Fire_" + to_wstring(GI->RandomInt(4, 8)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
	
}


CDestroyer_FrengeCharge_Thunder* CDestroyer_FrengeCharge_Thunder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDestroyer_FrengeCharge_Thunder* pInstance = new CDestroyer_FrengeCharge_Thunder(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDestroyer_FrengeCharge_Thunder");
		Safe_Release<CDestroyer_FrengeCharge_Thunder*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CDestroyer_FrengeCharge_Thunder::Clone(void* pArg)
{
	CDestroyer_FrengeCharge_Thunder* pInstance = new CDestroyer_FrengeCharge_Thunder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDestroyer_FrengeCharge_Thunder");
		Safe_Release<CDestroyer_FrengeCharge_Thunder*>(pInstance);
	}

	return pInstance;
}

void CDestroyer_FrengeCharge_Thunder::Free()
{
	__super::Free();
}
