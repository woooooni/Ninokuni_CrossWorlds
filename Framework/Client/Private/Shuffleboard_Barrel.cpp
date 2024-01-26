#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "..\Public\Shuffleboard_Barrel.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CShuffleboard_Barrel::CShuffleboard_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CShuffleboard_Prop(pDevice, pContext, strObjectTag)
{
}

CShuffleboard_Barrel::CShuffleboard_Barrel(const CShuffleboard_Barrel& rhs)
	: CShuffleboard_Prop(rhs)
{
}

HRESULT CShuffleboard_Barrel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShuffleboard_Barrel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
	{
		m_pTransformCom->Set_Scale(Vec3(1.5f));
	}

	/* Model */
	if (nullptr != m_pModelCom)
	{
		m_pModelCom->Set_Animation(1);
	}

	/* Rigidbody */
	if (nullptr != m_pRigidBodyCom)
	{
		m_pRigidBodyCom->Set_Use_Gravity(false);

		Vec3 vPlayerLook = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Look();
		{
			vPlayerLook.y = 0.f;
			vPlayerLook.Normalize();
		}

		const _float fPower = 20.f;

		m_pRigidBodyCom->Add_Velocity(vPlayerLook, fPower, false);
	}
	return S_OK;
}

void CShuffleboard_Barrel::Tick(_float fTimeDelta)
{
	/* 이전 프레임에서 충돌 세팅이 되어있다면 반영한다. */
	if (m_tElasticColDesc.bSet)
	{
		m_pRigidBodyCom->Set_Velocity(m_tElasticColDesc.vVelocity);
		m_tElasticColDesc.Clear();
	}

	/* 플레이어가 손에서 내려놓을 때 높이를 세팅해주기 위한 보간 */
	m_tHeightLerpDesc.Update(fTimeDelta);
	if (m_tHeightLerpDesc.bActive)
	{
		Vec3 vCurPos = m_pTransformCom->Get_Position();

		vCurPos.y = m_tHeightLerpDesc.fCurValue;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCurPos);
	}

	__super::Tick(fTimeDelta);
}

void CShuffleboard_Barrel::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CShuffleboard_Barrel::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShuffleboard_Barrel::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

void CShuffleboard_Barrel::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (OBJ_TYPE::OBJ_SHUFFLEBOARD_PROP == tInfo.pOther->Get_ObjectType())
	{	
		Calculate_ElasticCollision(tInfo.pOther);
	}
}

void CShuffleboard_Barrel::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CShuffleboard_Barrel::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CShuffleboard_Barrel::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
}

void CShuffleboard_Barrel::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
}

void CShuffleboard_Barrel::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
}

void CShuffleboard_Barrel::PutDown()
{
	const _float fCurHeight = Vec3(m_pTransformCom->Get_Position()).y;

	const _float fTargetHeight = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).y;

	const _float fLerpTime = 0.5f;

	m_tHeightLerpDesc.Start(fCurHeight, fTargetHeight, fLerpTime, LERP_MODE::EASE_IN);
}

HRESULT CShuffleboard_Barrel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;
	}

	/* For.Com_RigidBody */
	{
		CRigidBody::RIGID_BODY_DESC RigidDesc;
		RigidDesc.pTransform = m_pTransformCom;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
			return E_FAIL;
	}


	/* For. Com_Model */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Prop_Barrel"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShuffleboard_Barrel::Ready_Colliders()
{
	/* Sphere Collider */
	{
		BoundingSphere Sphere;
		ZeroMemory(&Sphere, sizeof(BoundingSphere));
		{
			Sphere.Radius = 0.5f;
			Sphere.Center = Vec3::Zero;
		}

		CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
		ZeroMemory(&SphereDesc, sizeof(SphereDesc));
		{
			SphereDesc.tSphere = Sphere;
			SphereDesc.pNode = nullptr;
			SphereDesc.pOwnerTransform = m_pTransformCom;
			SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
			SphereDesc.vOffsetPosition = Vec3::Zero;;
		}

		if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShuffleboard_Barrel::Calculate_ElasticCollision(CGameObject* pOther)
{
	CRigidBody* pOtherRigidBody = nullptr;
	CTransform* pOtherTransform = nullptr;

	/* Check Exception */
	{
		if(nullptr == pOther)
			return E_FAIL;

		pOtherRigidBody = pOther->Get_Component<CRigidBody>(L"Com_RigidBody");
		if (nullptr == pOtherRigidBody)
			return E_FAIL;

		pOtherTransform = pOther->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pOtherTransform)
			return E_FAIL;
	}

	/* 충돌 후의 방향 계산 */
	Vec3 vColDir = pOtherTransform->Get_Position() - m_pTransformCom->Get_Position();
	{
		vColDir.y = 0.f;
		vColDir.Normalize();
	}

	/* 충돌 후의 속도 계산 */
	Vec3 vVelocity;
	{
		Vec3 vMyDir = m_pRigidBodyCom->Get_Velocity();

		// 충돌 후의 상대 속도 계산
		Vec3 vRelativeVelocity = vMyDir - Vec3(pOtherRigidBody->Get_Velocity());

		// 반발 계수 설정
		const _float fRestitutionCoefficient = 0.2f;

		// 충돌 후의 속도 갱신
		vVelocity = vMyDir - (1.0f + fRestitutionCoefficient) * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&vRelativeVelocity), XMLoadFloat3(&vColDir))) * vColDir;
	}

	m_tElasticColDesc.Set(vVelocity);
}

CShuffleboard_Barrel* CShuffleboard_Barrel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CShuffleboard_Barrel* pInstance = new CShuffleboard_Barrel(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CShuffleboard_Barrel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShuffleboard_Barrel::Clone(void* pArg)
{
	CShuffleboard_Barrel* pInstance = new CShuffleboard_Barrel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CShuffleboard_Barrel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShuffleboard_Barrel::Free()
{
	__super::Free();
}
