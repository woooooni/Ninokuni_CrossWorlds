#include "stdafx.h"
#include "GameInstance.h"
#include "Enemy_Biplane_Bullet.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"

#include "Particle.h"
#include "Grandprix_Manager.h"
#include "Riding_Manager.h"

#include "Vehicle_Flying_Biplane.h"

CEnemy_Biplane_Bullet::CEnemy_Biplane_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Enemy_Biplane_Bullet", OBJ_TYPE::OBJ_GRANDPRIX_ENEMY_PROJECTILE)
{

}

CEnemy_Biplane_Bullet::CEnemy_Biplane_Bullet(const CEnemy_Biplane_Bullet& rhs)
	: CVehicleFlying_Projectile(rhs)
{
}


HRESULT CEnemy_Biplane_Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_Biplane_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_vBloomPower = Vec3(1.f, 1.f, 1.f);
	m_fDeletionTime = 5.f;

	m_pParticle = dynamic_cast<CParticle*>(GI->Clone_GameObject(L"Prototype_Particle_Enemy_Bullet", LAYER_TYPE::LAYER_EFFECT));

	if (nullptr == m_pParticle)
	{
		MSG_BOX("Clone Particle Failed. : CEnemy_Biplane_Bullet::Initialize");
		return E_FAIL;
	}

	m_pParticle->Set_Owner(this);
	return S_OK;
}

void CEnemy_Biplane_Bullet::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr == m_pTargetTransform)
	{
		m_pTargetTransform = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform();
	}

	/*GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.1f), fTimeDelta, TEXT("Particle_Smoke"), this);*/
	m_pTransformCom->Set_Scale(Vec3(0.5f, 0.5f, 0.5f));
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

	if (true == m_bDead)
	{
		CPool<CEnemy_Biplane_Bullet>::Return_Obj(this);
		return;
	}
}

void CEnemy_Biplane_Bullet::LateTick(_float fTimeDelta)
{
	__super::LateUpdate_Collider(fTimeDelta);
	if (nullptr != m_pTargetTransform)
	{
		Vec3 vDir = m_pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();

		if (5.f >= vDir.Length())
			GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_PROJECTILE, this);
	}
	
	
#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CEnemy_Biplane_Bullet::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CEnemy_Biplane_Bullet::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationY(180.f);
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CEnemy_Biplane_Bullet::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER) && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);

		Set_Dead(true);
		if (false == CPool<CEnemy_Biplane_Bullet>::Return_Obj(this))
		{
			MSG_BOX("ReturnPool Failed. : CEnemy_Biplane_Bullet");
			return;
		}
	}
}

void CEnemy_Biplane_Bullet::Enter_Scene()
{
	m_pParticle->Set_Dead(false);
	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, m_pParticle)))
	{
		MSG_BOX("Add Particle Failed. : CEnemy_Biplane_Bullet::Enter_Scene");
		return;
	}
}

void CEnemy_Biplane_Bullet::Return_Pool()
{
	m_pParticle->Set_Dead(true);
	Safe_AddRef(m_pParticle);
}


CEnemy_Biplane_Bullet* CEnemy_Biplane_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemy_Biplane_Bullet* pInstance = new CEnemy_Biplane_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEnemy_Biplane_Bullet");
		Safe_Release<CEnemy_Biplane_Bullet*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEnemy_Biplane_Bullet::Clone(void* pArg)
{
	CEnemy_Biplane_Bullet* pInstance = new CEnemy_Biplane_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEnemy_Biplane_Bullet");
		Safe_Release<CEnemy_Biplane_Bullet*>(pInstance);
	}

	return pInstance;
}

void CEnemy_Biplane_Bullet::Free()
{
	__super::Free();
	Safe_Release(m_pParticle);
}
