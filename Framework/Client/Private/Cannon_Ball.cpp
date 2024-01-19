#include "stdafx.h"
#include "GameInstance.h"
#include "Cannon_Ball.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character.h"

CCannon_Ball::CCannon_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter_Projectile(pDevice, pContext, L"Cannon_Tower_Projectile")
{

}

CCannon_Ball::CCannon_Ball(const CCannon_Ball& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CCannon_Ball::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCannon_Ball::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::BOUND, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	return S_OK;
}

void CCannon_Ball::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.1f), fTimeDelta, TEXT("Particle_Smoke"), this);
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
}

void CCannon_Ball::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCannon_Ball::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CCannon_Ball::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Cannon_Ball"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
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

void CCannon_Ball::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS)
		&& tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
	}
}


CCannon_Ball* CCannon_Ball::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCannon_Ball* pInstance = new CCannon_Ball(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CCannon_Ball");
		Safe_Release<CCannon_Ball*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCannon_Ball::Clone(void* pArg)
{
	CCannon_Ball* pInstance = new CCannon_Ball(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CCannon_Ball");
		Safe_Release<CCannon_Ball*>(pInstance);
	}

	return pInstance;
}

void CCannon_Ball::Free()
{
	__super::Free();
}
