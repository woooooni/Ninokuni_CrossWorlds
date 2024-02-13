#include "stdafx.h"
#include "GameInstance.h"
#include "Cannon_Ball.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
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
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_fDeletionTime = 5.f;

	

	return S_OK;
}

void CCannon_Ball::Tick(_float fTimeDelta)
{

	if (false == m_bInitLook)
	{
		m_bInitLook = true;
		m_vInitLook = XMVector3Normalize(m_pTransformCom->Get_Look());

		m_bInitDestPosition = Find_Dest_Position(fTimeDelta);
		m_vDestPosition;
		if (true == m_bInitDestPosition)
		{
			m_vStartPosition = m_pTransformCom->Get_Position();
			m_pRigidBodyCom->Set_Ground(true);
			m_pRigidBodyCom->Set_Use_Gravity(false);
		}
		else
		{
			m_pRigidBodyCom->Add_Velocity(XMVectorSet(0.f, 1.f, 0.f, 0.f), 10.f, true);
		}
	}

	if (true == m_bInitDestPosition)
	{
		//포물선 공식을 적용한다.
		m_fAccMove += fTimeDelta;
		m_pTransformCom->ParabolicFormula(m_vStartPosition.xyz(), m_vDestPosition.xyz(), m_fAccMove, max(m_vStartPosition.y, m_vDestPosition.y) + 5.f, 20.f);
	}
	else
	{
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
		m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Right()), XMConvertToRadians(180.f) * fTimeDelta);
		m_pTransformCom->Move(XMVector3Normalize(m_vInitLook), 10.f, fTimeDelta);
	}

	
	
	m_pControllerCom->Tick_Controller(fTimeDelta);
	__super::Tick(fTimeDelta);
	
	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.01f, 0.01f), fTimeDelta, TEXT("Particle_Smoke"), this);

}

void CCannon_Ball::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pControllerCom->LateTick_Controller(fTimeDelta);
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

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;


	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 0.8f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
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
		if (true == m_bReserveDead || true == m_bDead)
			return;

		Reserve_Dead(true);
		m_fAccDeletionTime = 0.f;
		m_fDeletionTime = 0.1f;

		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default_Attack();

		// TODO :: Explosion Effect
	}
}

void CCannon_Ball::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);

	if (true == m_bReserveDead || true == m_bDead)
		return;

	Reserve_Dead(true);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::STRONG, 0.f, 0.f, 0.f, false);

	for (auto& pAttackCollider : Get_Collider(CCollider::DETECTION_TYPE::ATTACK))
		pAttackCollider->Set_Radius(5.f);

	m_fAccDeletionTime = 0.f;
	m_fDeletionTime = 0.1f;

	// TODO :: Explosion Effect
}

_bool CCannon_Ball::Find_Dest_Position(_float fTimeDelta)
{
	list<CGameObject*>& TargetObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);

	_bool bFind = false;
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
			m_vDestPosition = pTargetTransform->Get_Position();
			bFind = true;
		}
	}
	return bFind;
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
	Safe_Release(m_pControllerCom);
	__super::Free();
}
