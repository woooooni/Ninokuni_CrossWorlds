#include "stdafx.h"
#include "GameInstance.h"
#include "Character_Biplane_Bullet.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"
#include "Pool.h"
#include "Particle.h"

CCharacter_Biplane_Bullet::CCharacter_Biplane_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Character_Biplane_Bullet", OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
{

}

CCharacter_Biplane_Bullet::CCharacter_Biplane_Bullet(const CCharacter_Biplane_Bullet& rhs)
	: CVehicleFlying_Projectile(rhs)
{
	
}


HRESULT CCharacter_Biplane_Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter_Biplane_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3(5.f, 5.f, 5.f));

	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	m_fDeletionTime = 1.5f;

	m_fMoveSpeed = 60.f;
	m_vBloomPower = Vec3(1.f, 1.f, 0.f);

	m_pParticle = dynamic_cast<CParticle*>(GI->Clone_GameObject(L"Prototype_Particle_Character_Bullet", LAYER_TYPE::LAYER_EFFECT));

	if (nullptr == m_pParticle)
	{
		MSG_BOX("Clone Particle Failed. : CCharacter_Biplane_Bullet::Initialize");
		return E_FAIL;
	}

	m_pParticle->Set_Owner(this);
	return S_OK;
}

void CCharacter_Biplane_Bullet::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

	if (true == m_bDead)
	{
		CPool<CCharacter_Biplane_Bullet>::Return_Obj(this);
		return;
	}
}

void CCharacter_Biplane_Bullet::LateTick(_float fTimeDelta)
{
	__super::LateUpdate_Collider(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_PROJECTILE, this);
#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CCharacter_Biplane_Bullet::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CCharacter_Biplane_Bullet::Ready_Components()
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
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CCharacter_Biplane_Bullet::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_ENEMY) && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		/*wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);*/

		Set_Dead(true);
		if (false == CPool<CCharacter_Biplane_Bullet>::Return_Obj(this))
		{
			MSG_BOX("ReturnPool Failed. : CCharacter_Biplane_Bullet");
			return;
		}
	}
}

void CCharacter_Biplane_Bullet::Enter_Scene()
{
	m_pParticle->Set_Dead(false);
	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, m_pParticle)))
	{
		MSG_BOX("Add Particle Failed. : CCharacter_Biplane_Bullet::Enter_Scene");
		return;
	}
		
}

void CCharacter_Biplane_Bullet::Return_Pool()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pParticle->Set_Dead(true);
	Safe_AddRef(m_pParticle);
}


CCharacter_Biplane_Bullet* CCharacter_Biplane_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharacter_Biplane_Bullet* pInstance = new CCharacter_Biplane_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CCharacter_Biplane_Bullet");
		Safe_Release<CCharacter_Biplane_Bullet*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCharacter_Biplane_Bullet::Clone(void* pArg)
{
	CCharacter_Biplane_Bullet* pInstance = new CCharacter_Biplane_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CCharacter_Biplane_Bullet");
		Safe_Release<CCharacter_Biplane_Bullet*>(pInstance);
	}

	return pInstance;
}

void CCharacter_Biplane_Bullet::Free()
{
	__super::Free();
	Safe_Release(m_pParticle);
}
