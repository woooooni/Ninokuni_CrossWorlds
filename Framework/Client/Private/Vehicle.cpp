#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "..\Public\Vehicle.h"

CVehicle::CVehicle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, OBJ_TYPE eObjType)
	: CGameObject(pDevice, pContext, strObjectTag, eObjType)
{
}

CVehicle::CVehicle(const CVehicle& rhs)
	: CGameObject(rhs)
{

}

void CVehicle::Set_Aboard(_bool bAboard)
{
	m_bOnBoard = bAboard;
	m_pRider = nullptr;
}

HRESULT CVehicle::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_VehicleDesc = *((VEHICLE_DESC*)(pArg));
	}



	return S_OK;
}

void CVehicle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pStateCom)
		m_pStateCom->Tick_State(fTimeDelta);

//	Update_Rider(fTimeDelta);
}

void CVehicle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	if(nullptr != m_pTransformCom)
		Compute_CamZ(m_pTransformCom->Get_Position());

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CVehicle::Render()
{
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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CVehicle::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom || nullptr == m_pModelCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}

	return S_OK;
}

void CVehicle::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle::Ride(CGameObject* pRider)
{
	if (nullptr == pRider)
	{
		MSG_BOX("Rider is Null :  CVehicle::Ride");
		return;
	}

	m_pRiderTransform = pRider->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == m_pRiderTransform)
	{
		MSG_BOX("Get Rider Transform Failed. :  CVehicle::Ride");
		return;
	}

	m_pRider = pRider;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pRiderTransform->Get_Right());
	m_pTransformCom->Set_State(CTransform::STATE_UP, m_pRiderTransform->Get_Up());
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pRiderTransform->Get_Look());
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pRiderTransform->Get_Position());

	m_pControllerCom->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
}

void CVehicle::Update_Rider(_float fTimeDelta)
{
	if (nullptr == m_pRider || nullptr == m_pRiderTransform || m_pRider->Is_Dead() || m_pRider->Is_ReserveDead())
	{
		m_pRider = nullptr;
		m_pRiderTransform = nullptr;
		return;
	}
	
	if (true == m_bUseBone)
	{
		Matrix SitBoneMatrix = m_pModelCom->Get_SocketLocalMatrix(0);

		if (nullptr != m_pRiderTransform)
		{
			m_pRiderTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
			_matrix MatSitWorld = SitBoneMatrix * m_pTransformCom->Get_WorldMatrix();
			m_pRiderTransform->Set_State(CTransform::STATE_POSITION, MatSitWorld.r[CTransform::STATE_POSITION] + XMVectorSet(0.f, m_fOffsetY, 0.f, 0.f));
		}
	}
	else
	{
		if (nullptr != m_pRiderTransform)
		{
			m_pRiderTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		}
	}
}

HRESULT CVehicle::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	return S_OK;
}

void CVehicle::Free()
{
	__super::Free();

	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
