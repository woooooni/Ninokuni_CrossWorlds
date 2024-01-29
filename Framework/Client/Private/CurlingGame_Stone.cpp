#include "stdafx.h"
#include "..\Public\CurlingGame_Stone.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "CurlingGame_Wall.h"

CCurlingGame_Stone::CCurlingGame_Stone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_Stone::CCurlingGame_Stone(const CCurlingGame_Stone& rhs)
	: CCurlingGame_Prop(rhs)
	, m_tElasticColDesc(rhs.m_tElasticColDesc)
	, m_tHeightLerpDesc(rhs.m_tHeightLerpDesc)
	, m_iNumCol(rhs.m_iNumCol)
	, m_bLaunched(rhs.m_bLaunched)
	, m_eOwnerType(rhs.m_eOwnerType)
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

	/* Active */
	m_bActive = false;

	/* Prop Type */
	m_eCGType = CG_TYPE::CG_STONE;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
	{
		if (STONE_TYPE::BARREL == m_eStoneType)
			m_pTransformCom->Set_Scale(Vec3(1.3f));
		else if (STONE_TYPE::POT == m_eStoneType)
			m_pTransformCom->Set_Scale(Vec3(0.9f));
		
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

	/* ���� �����ӿ��� �浹 ������ �Ǿ��ִٸ� �ݿ��Ѵ�. */
	if (m_tElasticColDesc.bSet)
	{
		m_pRigidBodyCom->Set_Velocity(m_tElasticColDesc.vVelocity);
		m_tElasticColDesc.Clear();
	}

	/* �÷��̾ �տ��� �������� �� ���̸� �������ֱ� ���� ���� */
	m_tHeightLerpDesc.Update(fTimeDelta);
	if (m_tHeightLerpDesc.bActive)
	{
		Vec3 vCurPos = m_pTransformCom->Get_Position();

		vCurPos.y = m_tHeightLerpDesc.fCurValue;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, XMVectorSetW(vCurPos, 1.f));
	}

	__super::Tick(fTimeDelta);

	/* ������ٵ� üũ */
	if (!m_pRigidBodyCom->Is_Sleep())
	{
		m_bMoving = m_pRigidBodyCom->Check_Sleep();
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

		/* ��� �浹 */
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
	}
}

void CCurlingGame_Stone::Launch(const _float& fPower)
{
	/* Transform */
	{
		Vec4 vPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
		Vec4 vDir = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
		Vec4 vLookAt = vPos + (vDir * 5.f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos.OneW());
		m_pTransformCom->LookAt_ForLandObject(vLookAt.OneW());
	}

	/* Rigidbody */
	if (nullptr != m_pRigidBodyCom)
	{
		const Vec3 vDir = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Look()).ZeroY().Normalized();
		
		m_pRigidBodyCom->Add_Velocity(vDir, fPower, false);

		m_bLaunched = true;

		m_bActive = true;
	}
}

void CCurlingGame_Stone::PutDown()
{
	const _float fCurHeight = Vec3(m_pTransformCom->Get_Position()).y;

	const _float fTargetHeight = Vec3(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).y;

	const _float fLerpTime = 0.5f;

	m_tHeightLerpDesc.Start(fCurHeight, fTargetHeight, fLerpTime, LERP_MODE::EASE_IN);
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
			SphereDesc.vOffsetPosition = Vec3{ 0.f, 50.f, 0.f };
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

	/* �浹 ���� ���� ��� */
	Vec3 vColDir = pOtherTransform->Get_Position() - m_pTransformCom->Get_Position();
	{
		vColDir.y = 0.f;
		vColDir.Normalize();
	}

	/* �浹 ���� �ӵ� ��� */
	Vec3 vNewVelocity;
	{
		Vec3 vMyDir = m_pRigidBodyCom->Get_Velocity();

		// �浹 ���� ��� �ӵ� ���
		Vec3 vRelativeVelocity = vMyDir - Vec3(pOtherRigidBody->Get_Velocity());

		// �ݹ� ��� ����
		const _float fRestitutionCoefficient = 0.25f;

		// �浹 ���� �ӵ� ����
		vNewVelocity = vMyDir - (1.0f + fRestitutionCoefficient) * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&vRelativeVelocity), XMLoadFloat3(&vColDir))) * vColDir;
	}

	m_tElasticColDesc.Set(vNewVelocity);
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

	/* Obb �ڽ� �浹�� ����μ� ��Ȯ�� �浹 �Ǵ��� �Ұ��� �ϱ⿡ Look�� ����Ͽ� �������� ��Ȳ������ �浹���� �Ǵ��Ѵ�. */
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