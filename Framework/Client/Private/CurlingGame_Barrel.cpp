#include "stdafx.h"
#include "..\Public\CurlingGame_Barrel.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "CurlingGame_Wall.h"

CCurlingGame_Barrel::CCurlingGame_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_Barrel::CCurlingGame_Barrel(const CCurlingGame_Barrel& rhs)
	: CCurlingGame_Prop(rhs)
	, m_tElasticColDesc(rhs.m_tElasticColDesc)
	, m_tHeightLerpDesc(rhs.m_tHeightLerpDesc)
	, m_iNumCol(rhs.m_iNumCol)
	, m_bLaunched(rhs.m_bLaunched)
	, m_eOwnerType(rhs.m_eOwnerType)
{
}

HRESULT CCurlingGame_Barrel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Barrel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Type */
	m_eCGType = CG_TYPE::CG_BARREL;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
		m_pTransformCom->Set_Scale(Vec3(1.3f));

	/* Model */
	if (nullptr != m_pModelCom)
	{
		m_pModelCom->Set_Animation(1);
		m_pModelCom->Set_Stop_Animation(true);
	}
	
	/* Rigidbody*/
	if(nullptr != m_pRigidBodyCom)
		m_pRigidBodyCom->Set_Use_Gravity(false);

	/* Active */
	m_bActive = false;

	return S_OK;
}

void CCurlingGame_Barrel::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

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

void CCurlingGame_Barrel::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

void CCurlingGame_Barrel::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == tInfo.pOther->Get_ObjectType())
	{	
		CCurlingGame_Prop* pProp = dynamic_cast<CCurlingGame_Prop*>(tInfo.pOther);
		if (nullptr == pProp)
			return;

		/* 통과 충돌 */
		if (CG_TYPE::CG_BARREL == pProp->Get_CGType())
		{
			Calculate_ElasticCollision(pProp);

			++m_iNumCol;
		}
		else if (CG_TYPE::CG_WALL == pProp->Get_CGType())
		{
			CCurlingGame_Wall* pWall = dynamic_cast<CCurlingGame_Wall*>(pProp);
			if(nullptr != pWall)
				Calculate_ActionAndReaction(pWall);
		}
	}
}

void CCurlingGame_Barrel::Launch(const Vec3& vDir, const _float& fPower)
{
	if (nullptr != m_pRigidBodyCom)
	{
		m_pRigidBodyCom->Add_Velocity(vDir, fPower, false);

		m_bLaunched = true;
	}
}

void CCurlingGame_Barrel::PutDown()
{
	const _float fCurHeight = Vec3(m_pTransformCom->Get_Position()).y;

	const _float fTargetHeight = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).y;

	const _float fLerpTime = 0.5f;

	m_tHeightLerpDesc.Start(fCurHeight, fTargetHeight, fLerpTime, LERP_MODE::EASE_IN);
}

HRESULT CCurlingGame_Barrel::Ready_Components()
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

HRESULT CCurlingGame_Barrel::Ready_Colliders()
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
			SphereDesc.vOffsetPosition = Vec3{ 0.f, 50.f, 0.f };
		}

		if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, m_iColliderDetectionType, &SphereDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCurlingGame_Barrel::Calculate_ElasticCollision(CGameObject* pOther)
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
	Vec3 vNewVelocity;
	{
		Vec3 vMyDir = m_pRigidBodyCom->Get_Velocity();

		// 충돌 후의 상대 속도 계산
		Vec3 vRelativeVelocity = vMyDir - Vec3(pOtherRigidBody->Get_Velocity());

		// 반발 계수 설정
		const _float fRestitutionCoefficient = 0.25f;

		// 충돌 후의 속도 갱신
		vNewVelocity = vMyDir - (1.0f + fRestitutionCoefficient) * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&vRelativeVelocity), XMLoadFloat3(&vColDir))) * vColDir;
	}

	m_tElasticColDesc.Set(vNewVelocity);
	return S_OK;
}

HRESULT CCurlingGame_Barrel::Calculate_ActionAndReaction(class CCurlingGame_Wall* pWall)
{
	CCollider*		pCollider = nullptr;
	CCollider_OBB*	pColliderObb = nullptr;

	/* Check Exception */
	{
		if (nullptr == pWall)
			return E_FAIL;

		pCollider = pWall->Get_Collider(m_iColliderDetectionType).front();
		if (nullptr != pCollider)
		{
			pColliderObb = dynamic_cast<CCollider_OBB*>(pCollider);
			if (nullptr == pColliderObb)
				return E_FAIL;
		}
	}

	/* Obb 박스 충돌시 현재로서 정확한 충돌 판단이 불가능 하기에 Look을 사용하여 제한적인 상황에서의 충돌만을 판단한다. */
	Vec3 vNewVelocity;
	{
		const Vec3 vNormal = pWall->Get_Normal().ZeroY().Normalized();
		const Vec3 vReflect = Vec3::Reflect(m_pTransformCom->Get_Look(), vNormal).ZeroY().Normalized();
		
		const _float fFrictionCoefficient = 0.5f;
		
		_float fPower = Vec3(m_pRigidBodyCom->Get_Velocity()).Length() * (1.f - fFrictionCoefficient);
		if (fPower < 0.f)
			fPower = 0.f;
		
		vNewVelocity = vReflect * fPower;
	}

	m_tElasticColDesc.Set(vNewVelocity);

	return S_OK;
}

CCurlingGame_Barrel* CCurlingGame_Barrel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCurlingGame_Barrel* pInstance = new CCurlingGame_Barrel(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCurlingGame_Barrel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCurlingGame_Barrel::Clone(void* pArg)
{
	CCurlingGame_Barrel* pInstance = new CCurlingGame_Barrel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCurlingGame_Barrel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurlingGame_Barrel::Free()
{
	__super::Free();
}
