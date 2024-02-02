#include "stdafx.h"
#include "GameInstance.h"
#include "Biplane_ThunderCloud.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"

CBiplane_Thunder_Cloud::CBiplane_Thunder_Cloud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Biplane_Thunder_Cloud", OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
{

}

CBiplane_Thunder_Cloud::CBiplane_Thunder_Cloud(const CBiplane_Thunder_Cloud& rhs)
	: CVehicleFlying_Projectile(rhs)
{
}


HRESULT CBiplane_Thunder_Cloud::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBiplane_Thunder_Cloud::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fMoveSpeed = 10.f;
	m_pTransformCom->Set_Scale(Vec3(0.15f, 0.15f, 0.15f));
	Set_Collider_Elemental(dynamic_cast<CCharacter*>(m_pOwner->Get_Rider())->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STUN, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	m_fDeletionTime = 5.f;

	return S_OK;
}

void CBiplane_Thunder_Cloud::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.1f), fTimeDelta, TEXT("Particle_Smoke"), this);

	Tick_Target(fTimeDelta);

	if (nullptr == m_pTarget)
	{
		Find_Target(fTimeDelta);
		if (nullptr == m_pTarget)
		{
			Set_Dead(true);
			return;
		}
	}

	if (m_fAccDeletionTime >= 3.f)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	}

	if (nullptr != m_pTarget)
	{
		CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr != pTargetTransform)
		{
			Vec4 vPosition = pTargetTransform->Get_Position() + XMVectorSet(0.f, 5.f, 0.f, 0.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}
}

void CBiplane_Thunder_Cloud::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBiplane_Thunder_Cloud::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	

	return S_OK;
}





HRESULT CBiplane_Thunder_Cloud::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	CCollider_AABB::AABB_COLLIDER_DESC AABBDesc;
	BoundingBox AABBBox;
	AABBBox.Extents = Vec3(5.f, 20.f, 5.f);
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, -1.f * AABBBox.Extents.y, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::ATTACK, &AABBDesc)))
		return E_FAIL;


	return S_OK;
}

void CBiplane_Thunder_Cloud::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
}

void CBiplane_Thunder_Cloud::Tick_Target(_float fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;


	if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
		m_pTarget = nullptr;
}

void CBiplane_Thunder_Cloud::Find_Target(_float fTimeDelta)
{
	list<CGameObject*>& TargetObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_NPC);

	_float fMinDistance = 50.f;
	for (auto& pTarget : TargetObjects)
	{
		CTransform* pTargetTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");

		Vec4 vPosition = m_pTransformCom->Get_Position();
		Vec4 vTargetPosition = pTargetTransform->Get_Position();

		Vec3 vDir = vTargetPosition - vPosition;
		if (fMinDistance > vDir.Length())
		{
			fMinDistance = vDir.Length();
			m_pTarget = pTarget;
		}
	}
}


CBiplane_Thunder_Cloud* CBiplane_Thunder_Cloud::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBiplane_Thunder_Cloud* pInstance = new CBiplane_Thunder_Cloud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CBiplane_Thunder_Cloud");
		Safe_Release<CBiplane_Thunder_Cloud*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CBiplane_Thunder_Cloud::Clone(void* pArg)
{
	CBiplane_Thunder_Cloud* pInstance = new CBiplane_Thunder_Cloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CBiplane_Thunder_Cloud");
		Safe_Release<CBiplane_Thunder_Cloud*>(pInstance);
	}

	return pInstance;
}

void CBiplane_Thunder_Cloud::Free()
{
	__super::Free();
}
