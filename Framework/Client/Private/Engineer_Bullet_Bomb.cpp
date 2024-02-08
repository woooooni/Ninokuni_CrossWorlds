#include "stdafx.h"
#include "GameInstance.h"
#include "Engineer_Bullet_Bomb.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"
#include "Effect.h"

CEngineer_Bullet_Bomb::CEngineer_Bullet_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"Engineer_Bullet_Bomb")
{

}

CEngineer_Bullet_Bomb::CEngineer_Bullet_Bomb(const CEngineer_Bullet_Bomb& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CEngineer_Bullet_Bomb::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEngineer_Bullet_Bomb::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3(10.f, 10.f, 10.f));
	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STUN, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_fDeletionTime = 3.f;

	m_fOnOffTime = 0.05f;
	return S_OK;
}

void CEngineer_Bullet_Bomb::Tick(_float fTimeDelta)
{
	if (false == m_bReserveDead)
	{
		__super::Tick(fTimeDelta);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
		m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Look()), -1.f * XMConvertToRadians(180.f) * 6.f * fTimeDelta);

		GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.3f), fTimeDelta, TEXT("Particle_Spark_Fire"), this);
		GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.6f), fTimeDelta, TEXT("Particle_SparkCircle_Fire"), this);

		return;
	}
	else
	{
		m_fAccDeletionTime += fTimeDelta;
		if (m_fAccDeletionTime >= m_fDeletionTime)
		{
			Set_Dead(true);
			m_fAccDeletionTime = 0.f;
			return;
		}

		m_fAccOnOff += fTimeDelta;
		if (m_fAccOnOff >= m_fOnOffTime)
		{
			m_fAccOnOff = 0.f;
			_bool bActive = Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active();
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, !bActive);
		}
	}
}

void CEngineer_Bullet_Bomb::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEngineer_Bullet_Bomb::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CEngineer_Bullet_Bomb::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Engineer_Bullet"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.4f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CEngineer_Bullet_Bomb::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (m_bReserveDead)		
			return;

		if (false == m_bCollisionEffect)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_ExplosionShot_Boom"), m_pTransformCom->Get_WorldMatrix(), tInfo.pOther);
			m_bCollisionEffect = true;
		}

		wstring strSoundKey = L"Ele_Impact_Fire_" + to_wstring(GI->RandomInt(4, 8)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);

		Reserve_Dead(true);
		m_fAccDeletionTime = 0.f;

		vector<CCollider*>& AttackColliders = Get_Collider(CCollider::DETECTION_TYPE::ATTACK);
		for (auto& pCollider : AttackColliders)
		{
			pCollider->Set_AttackType(CCollider::ATTACK_TYPE::WEAK);
			pCollider->Set_Radius(10.f);
		}
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	}
	
}

CEngineer_Bullet_Bomb* CEngineer_Bullet_Bomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngineer_Bullet_Bomb* pInstance = new CEngineer_Bullet_Bomb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEngineer_Bullet_Bomb");
		Safe_Release<CEngineer_Bullet_Bomb*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEngineer_Bullet_Bomb::Clone(void* pArg)
{
	CEngineer_Bullet_Bomb* pInstance = new CEngineer_Bullet_Bomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEngineer_Bullet_Bomb");
		Safe_Release<CEngineer_Bullet_Bomb*>(pInstance);
	}

	return pInstance;
}

void CEngineer_Bullet_Bomb::Free()
{
	__super::Free();
}
