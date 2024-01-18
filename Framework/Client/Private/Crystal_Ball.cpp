#include "stdafx.h"
#include "GameInstance.h"
#include "Crystal_Ball.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"

CCrystal_Ball::CCrystal_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter_Projectile(pDevice, pContext, L"Crystal_Tower_Projectile")
{

}

CCrystal_Ball::CCrystal_Ball(const CCrystal_Ball& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CCrystal_Ball::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrystal_Ball::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STUN, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	return S_OK;
}

void CCrystal_Ball::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.1f), fTimeDelta, TEXT("Particle_Smoke"), this);
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
}

void CCrystal_Ball::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCrystal_Ball::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CCrystal_Ball::Ready_Components()
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
	tSphere.Radius = 0.5f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CCrystal_Ball::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
}


CCrystal_Ball* CCrystal_Ball::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCrystal_Ball* pInstance = new CCrystal_Ball(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CCrystal_Ball");
		Safe_Release<CCrystal_Ball*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCrystal_Ball::Clone(void* pArg)
{
	CCrystal_Ball* pInstance = new CCrystal_Ball(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CCrystal_Ball");
		Safe_Release<CCrystal_Ball*>(pInstance);
	}

	return pInstance;
}

void CCrystal_Ball::Free()
{
	__super::Free();
}
