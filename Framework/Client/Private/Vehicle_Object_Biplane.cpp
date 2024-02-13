#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Object_Biplane.h"


CVehicle_Object_Biplane::CVehicle_Object_Biplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVehicle_Flying(pDevice, pContext, L"Vehicle_Object_Biplane", OBJ_PROP)
{
}

CVehicle_Object_Biplane::CVehicle_Object_Biplane(const CVehicle_Object_Biplane& rhs)
	: CVehicle_Flying(rhs)
{
}

HRESULT CVehicle_Object_Biplane::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVehicle_Object_Biplane::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_bUseBone = false; 
	m_pRigidBodyCom->Set_Use_Gravity(false);

	m_pModelCom->Set_Animation(9);
	m_pTransformCom->Set_Position(XMVectorSet(-74.f, -7.6f, -50.f, 1.f));
	if (nullptr != m_pControllerCom)
		m_pControllerCom->Set_EnterLevel_Position(XMVectorSet(-74.f, -7.6f, -50.f, 1.f));
	m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	m_bOnBoard = true;

	return S_OK;
}

void CVehicle_Object_Biplane::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::Tick(fTimeDelta);

//		if (nullptr != m_pRigidBodyCom)
//			m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

		if (nullptr != m_pControllerCom)
			m_pControllerCom->Tick_Controller(fTimeDelta);
	}
}

void CVehicle_Object_Biplane::LateTick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::LateTick(fTimeDelta);

		Update_Rider(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
}

HRESULT CVehicle_Object_Biplane::Render()
{
	if (true == m_bOnBoard)
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

		_float4 vRimColor = true == m_bInfinite ? Vec4(0.f, 0.f, 1.f, 1.f) : Vec4(0.f, 0.f, 0.f, 0.f);

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;

		const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		_uint iPassIndex = 0;
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
				return E_FAIL;
		}

		return S_OK;

	}

	return S_OK;
}

HRESULT CVehicle_Object_Biplane::Render_ShadowDepth()
{
	if (true == m_bOnBoard)
	{
		__super::Render_ShadowDepth();
	}

	return S_OK;
}

void CVehicle_Object_Biplane::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Object_Biplane::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Object_Biplane::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Object_Biplane::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Biplane"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Object_Biplane::Ready_States()
{

	return S_OK;
}

HRESULT CVehicle_Object_Biplane::Ready_Colliders()
{

	return S_OK;
}

CVehicle_Object_Biplane* CVehicle_Object_Biplane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVehicle_Object_Biplane* pInstance = new CVehicle_Object_Biplane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CVehicle_Object_Biplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVehicle_Object_Biplane::Clone(void* pArg)
{
	CVehicle_Object_Biplane* pInstance = new CVehicle_Object_Biplane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CVehicle_Object_Biplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVehicle_Object_Biplane::Free()
{
	__super::Free();
}
