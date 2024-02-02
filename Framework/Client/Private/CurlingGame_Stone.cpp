#include "stdafx.h"
#include "..\Public\CurlingGame_Stone.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "CurlingGame_Wall.h"
#include "CurlingGame_Manager.h"

CCurlingGame_Stone::CCurlingGame_Stone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_Stone::CCurlingGame_Stone(const CCurlingGame_Stone& rhs)
	: CCurlingGame_Prop(rhs)
	, m_tElasticColDesc(rhs.m_tElasticColDesc)
	, m_tPosLerpDesc(rhs.m_tPosLerpDesc)
	, m_iNumCol(rhs.m_iNumCol)
	, m_bLaunched(rhs.m_bLaunched)
	, m_eStoneType(rhs.m_eStoneType)
{
}

HRESULT CCurlingGame_Stone::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Stone::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	/* Stone Type */
	STONE_INIT_DESC* pDesc = static_cast<STONE_INIT_DESC*>(pArg);
	m_eStoneType = pDesc->eStoneType;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Prop Type */
	m_eCGType = CG_TYPE::CG_STONE;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
	{
		if (STONE_TYPE::BARREL == m_eStoneType)
			m_pTransformCom->Set_Scale(Vec3(1.1f));
		else if (STONE_TYPE::POT == m_eStoneType)
			m_pTransformCom->Set_Scale(Vec3(0.7f));
		
	}

	/* Model */
	if (nullptr != m_pModelCom)
	{
		m_pModelCom->Set_Animation(1);
		m_pModelCom->Set_Stop_Animation(true);
	}
	
	/* Rigidbody*/
	if (nullptr != m_pRigidBodyCom)
	{
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}


	return S_OK;
}

void CCurlingGame_Stone::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	/* 이전 프레임에서 충돌 세팅이 되어있다면 반영한다. */
	if (m_tElasticColDesc.bSet)
	{
		m_pRigidBodyCom->Set_Velocity(m_tElasticColDesc.vVelocity);
		m_tElasticColDesc.Clear();
	}

	/* 플레이어가 손에서 내려놓을 때 포지션을 세팅해주기 위한 보간 */
	if (m_tPosLerpDesc.bActive)
	{
		m_tPosLerpDesc.Update_Lerp(fTimeDelta);

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, m_tPosLerpDesc.vCurVec.OneW());

		if (!m_tPosLerpDesc.bActive)
			m_bPutted = true;
	}

	__super::Tick(fTimeDelta);

	/* 리지드바디 체크 */
	if (!m_pRigidBodyCom->Is_Sleep())
	{
		m_bMoving = !m_pRigidBodyCom->Check_Sleep();

		/* 회전 적용 */
		 const _float fRotateSpeed = Vec3(m_pRigidBodyCom->Get_Velocity()).Length() * m_fRotateSpeed * fTimeDelta;
		 m_pTransformCom->Rotation(Vec3::Up, fRotateSpeed);
	}
}

void CCurlingGame_Stone::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCurlingGame_Stone::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Stone::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Stone::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

HRESULT CCurlingGame_Stone::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

HRESULT CCurlingGame_Stone::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}

HRESULT CCurlingGame_Stone::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}

void CCurlingGame_Stone::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == tInfo.pOther->Get_ObjectType())
	{	
		CCurlingGame_Prop* pProp = dynamic_cast<CCurlingGame_Prop*>(tInfo.pOther);
		if (nullptr == pProp)
			return;

		if (CG_TYPE::CG_STONE == pProp->Get_CGType())
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
		else if (CG_TYPE::CG_DEADZONE == pProp->Get_CGType())
		{
			m_bOutted = true;
			m_pRigidBodyCom->Set_Use_Gravity(true);
		}
	}
}

void CCurlingGame_Stone::Launch(Vec4 vDir, const _float& fPower)
{
	if (nullptr == m_pRigidBodyCom)
		return;

	m_pRigidBodyCom->Add_Velocity(vDir.Normalized(), fPower, false);

	m_pRigidBodyCom->Set_Sleep(false);

	m_bLaunched = m_bMoving = true;
}

void CCurlingGame_Stone::PutDown()
{
	CTransform* pTargetTransform = CCurlingGame_Manager::GetInstance()->Get_CurParticipant()->Get_Component_Transform();
	
	if (nullptr == pTargetTransform)
		return;
	
	Vec4 vTargetPos = (Vec4)pTargetTransform->Get_Position() + pTargetTransform->Get_RelativeOffset(Vec4{ 0.f, 0.f, 1.f, 1.f });

	m_tPosLerpDesc.Start(m_pTransformCom->Get_Position(), vTargetPos.OneW(), 0.5f, LERP_MODE::EASE_IN);
}

HRESULT CCurlingGame_Stone::Ready_Components()
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
		if (STONE_TYPE::BARREL == m_eStoneType)
		{
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Prop_Barrel"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
				return E_FAIL;
		}
		else if (STONE_TYPE::POT == m_eStoneType)
		{
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Prop_Pot"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
				return E_FAIL;
		}
		else
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCurlingGame_Stone::Ready_Colliders()
{
	/* Sphere Collider */
	{
		BoundingSphere Sphere;
		ZeroMemory(&Sphere, sizeof(BoundingSphere));
		{
			if (STONE_TYPE::BARREL == m_eStoneType)
				Sphere.Radius = 0.5f;
			else if (STONE_TYPE::POT == m_eStoneType)
				Sphere.Radius = 0.6f;

			Sphere.Center = Vec3::Zero;
		}

		CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
		ZeroMemory(&SphereDesc, sizeof(SphereDesc));
		{
			SphereDesc.tSphere = Sphere;
			SphereDesc.pNode = nullptr;
			SphereDesc.pOwnerTransform = m_pTransformCom;
			SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
			SphereDesc.vOffsetPosition = Vec3{ 0.f, 30.f, 0.f };
		}

		if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, m_iColliderDetectionType, &SphereDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCurlingGame_Stone::Calculate_ElasticCollision(CGameObject* pOther)
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

	m_pRigidBodyCom->Set_Sleep(false);

	return S_OK;
}

HRESULT CCurlingGame_Stone::Calculate_ActionAndReaction(class CCurlingGame_Wall* pWall)
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
		
		const _float fFrictionCoefficient = 0.25f;
		
		_float fPower = Vec3(m_pRigidBodyCom->Get_Velocity()).Length() * (1.f - fFrictionCoefficient);
		if (fPower < 0.f)   
			fPower = 0.f;
		
		vNewVelocity = vReflect * fPower;
	}

	m_tElasticColDesc.Set(vNewVelocity);

	return S_OK;
}

CCurlingGame_Stone* CCurlingGame_Stone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCurlingGame_Stone* pInstance = new CCurlingGame_Stone(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCurlingGame_Stone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCurlingGame_Stone::Clone(void* pArg)
{
	CCurlingGame_Stone* pInstance = new CCurlingGame_Stone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCurlingGame_Stone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurlingGame_Stone::Free()
{
	__super::Free();
}
