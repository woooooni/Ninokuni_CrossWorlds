#include "stdafx.h"
#include "Character_Dummy.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"




CCharacter_Dummy::CCharacter_Dummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
{
}

CCharacter_Dummy::CCharacter_Dummy(const CCharacter_Dummy& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCharacter_Dummy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCharacter_Dummy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	//if (FAILED(Ready_Parts()))
	//	return E_FAIL;

	//if (FAILED(Ready_States()))
	//	return E_FAIL;

	return S_OK;
}

void CCharacter_Dummy::Tick(_float fTimeDelta)
{
	

	_bool bKeyInput = false;
	if (KEY_HOLD(KEY::UP_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(-1.f * vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f) * fTimeDelta);	
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f) * fTimeDelta);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_bIsJumping = true;
		m_bFirstJump = true;
		m_pRigidBodyCom->Add_Velocity(XMVectorSet(0.f, 1.f, 0.f, 0.f), 5.f, false);
		m_pRigidBodyCom->Set_Use_Gravity(true);
		m_pRigidBodyCom->Set_Ground(false);
	}

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CCharacter_Dummy::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);
	m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
		{
			m_pRendererCom->Add_Debug(pCollider);
		}
	}
#endif
}

HRESULT CCharacter_Dummy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}




	return S_OK;
}

void CCharacter_Dummy::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CCharacter_Dummy::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
	
}

void CCharacter_Dummy::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCharacter_Dummy::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
	
}

void CCharacter_Dummy::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);

	Vec3 vNormal = tInfo.vNormal;
	vNormal.Normalize();

	Vec3 vUp = Vec3(0.f, 1.f, 0.f);
	vUp.Normalize();
	_float fRadian = vNormal.Dot(vUp);

	if (fRadian < XMConvertToRadians(90.f))
	{
		_vector vPosition = m_pTransformCom->Get_Position();
		vPosition = XMVectorSetY(vPosition, tInfo.vCollision_Position.y);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CCharacter_Dummy::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	Vec3 vVelocity = m_pRigidBodyCom->Get_Velocity();
	m_pRigidBodyCom->Set_Velocity(Vec3(vVelocity.x, 0.f, vVelocity.z));
	m_pRigidBodyCom->Set_Use_Gravity(true);
	m_pRigidBodyCom->Set_Ground(false);
}






HRESULT CCharacter_Dummy::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordMan_Body"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CRigidBody::RIGID_BODY_DESC RigidDesc;
	
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	/*m_pRigidBodyCom->Set_Velocity(_float3(0.f, 0.f, 0.f));

	m_pRigidBodyCom->Set_Use_Gravity(false);*/

	return S_OK;
}


#pragma region Ready_Colliders
HRESULT CCharacter_Dummy::Ready_Colliders()
{

	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 200.f, 50.f };




	OBBDesc.tBox = OBBBox;
	OBBDesc.pOwner = this;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);
	OBBDesc.bLockAngle_X = false;
	OBBDesc.bLockAngle_Y = false;
	OBBDesc.bLockAngle_Z = false;

	OBBDesc.fAngularDamping = 0.f;
	OBBDesc.fDensity = 1.f;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;



	
	//
	//ColliderDesc.PhysXInitDesc.vExtents = { 5.f, 5.f, 10.f };
	//
	//ColliderDesc.PhysXInitDesc.eColliderType = PHYSX_COLLIDER_TYPE::SPHERE;
	//ColliderDesc.PhysXInitDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;

	//ColliderDesc.PhysXInitDesc.bLockAngle_X = true;
	//ColliderDesc.PhysXInitDesc.bLockAngle_Y = false;
	//ColliderDesc.PhysXInitDesc.bLockAngle_Z = true;

	//ColliderDesc.PhysXInitDesc.bKinematic = true;
	//ColliderDesc.PhysXInitDesc.fAngularDamping = 30.f;
	//ColliderDesc.PhysXInitDesc.fDensity = 100.f;


	//ColliderDesc.PhysXInitDesc.fStaticFriction = 0.f;
	//ColliderDesc.PhysXInitDesc.fDynamicFriction = 0.f;
	//ColliderDesc.PhysXInitDesc.fRestitution = 0.f;

	//ColliderDesc.PhysXInitDesc.fMaxVelocity = 10.f;
	//ColliderDesc.PhysXInitDesc.pGameObject = this;

	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
	//	return E_FAIL;


	//ColliderDesc.tSphere.Radius = .2f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"C_TongueA_2");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);

	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::HEAD, &ColliderDesc)))
	//	return E_FAIL;

	//ColliderDesc.tSphere.Radius = .6f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"C_Spine_1");
	//ColliderDesc.vOffsetPosition = _float3(0.f, -0.25f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
	//	return E_FAIL;



	//ColliderDesc.tSphere.Radius = .1f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"L_Foot_End");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &ColliderDesc)))
	//	return E_FAIL;


	//ColliderDesc.tSphere.Radius = .1f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"R_Foot_End");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &ColliderDesc)))
	//	return E_FAIL;


	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	return S_OK;
}

#pragma endregion


CCharacter_Dummy* CCharacter_Dummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCharacter_Dummy* pInstance = new CCharacter_Dummy(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CCharacter_Dummy");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CCharacter_Dummy::Clone(void* pArg)
{
	CCharacter_Dummy* pInstance = new CCharacter_Dummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCharacter_Dummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacter_Dummy::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRigidBodyCom);
}