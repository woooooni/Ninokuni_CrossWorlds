#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying_Biplane.h"

#include "State_VehicleFlying_Stand.h"
#include "State_VehicleFlying_Run.h"
#include "State_VehicleFlying_Rush.h"

#include "Character.h"

#include "UIMinigame_Manager.h"

CVehicle_Flying_Biplane::CVehicle_Flying_Biplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVehicle_Flying(pDevice, pContext, strObjectTag)
{
}

CVehicle_Flying_Biplane::CVehicle_Flying_Biplane(const CVehicle_Flying_Biplane& rhs)
	: CVehicle_Flying(rhs)
{
}

HRESULT CVehicle_Flying_Biplane::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVehicle_Flying_Biplane::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;
	
	m_bUseBone = false; 
//	m_fOffsetY = -0.5f;

	if (nullptr != m_pRider)
	{
		if (OBJ_TYPE::OBJ_PLAYER == m_pRider->Get_ObjectType())
			m_bIsPlayers = true;
	}

	return S_OK;
}

void CVehicle_Flying_Biplane::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::Tick(fTimeDelta);
		
		Update_RiderState();

		if (false == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
		{
			if (false == m_bUseRigidbody)
				m_bUseRigidbody = true;
		}
		else
			m_bUseRigidbody = false;

		if (true == m_bUseRigidbody)
		{
			if (nullptr != m_pRigidBodyCom)
				m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
		}

		if (nullptr != m_pControllerCom)
			m_pControllerCom->Tick_Controller(fTimeDelta);
	}
}

void CVehicle_Flying_Biplane::LateTick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_BODY, this);
	
		__super::LateTick(fTimeDelta);

		Update_Rider(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
}

HRESULT CVehicle_Flying_Biplane::Render()
{
	if (true == m_bOnBoard)
	{
		//__super::Render();
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

		if (false == m_bIsPlayers)
		{
			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
					return E_FAIL;
			}
		}
		else
		{
			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CVehicle_Flying_Biplane::Render_ShadowDepth()
{
	if (true == m_bOnBoard)
	{
		//__super::Render_ShadowDepth();
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

		if (false == m_bIsPlayers)
		{
			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
					return E_FAIL;
			}
		}
		else
		{
			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

void CVehicle_Flying_Biplane::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle_Flying_Biplane::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Flying_Biplane::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Flying_Biplane::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Flying_Biplane::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Flying_Biplane::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Flying_Biplane::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Biplane"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For Texture Component
	if (FAILED(__super::Add_Component(LEVEL_EVERMORE,
		TEXT("Prototype_Component_Texture_Vehicle_Minigame_Grandprix_Biplane_ColorBlue"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying_Biplane::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUSH, CState_VehicleFlying_Rush::Create(m_pStateCom, strAnimationNames));

	//VEHICLE_RUSH
//	strAnimationNames.clear();
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Run");
//	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_Vehicle_Run::Create(m_pStateCom, strAnimationNames));
//
//	strAnimationNames.clear();
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_Sprint");
//	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_SPRINT, CState_Vehicle_Sprint::Create(m_pStateCom, strAnimationNames));
//
//	strAnimationNames.clear();
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpStart");
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpUp");
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpDown");
//	strAnimationNames.push_back(L"SKM_Udadak.ao|Udadak_JumpEnd");
//	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP, CState_Vehicle_Jump::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);

	return S_OK;
}

void CVehicle_Flying_Biplane::Update_RiderState()
{
	if (nullptr != m_pRider)
	{
		CStateMachine* pStateCom = m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
		if (nullptr == pStateCom)
			return;

		if (CVehicle::VEHICLE_STATE::VEHICLE_RUN == m_pStateCom->Get_CurrState() &&
			CCharacter::STATE::FLYING_RUNSTART != pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::FLYING_RUN != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::FLYING_RUN);
		}

		if (CVehicle::VEHICLE_STATE::VEHICLE_IDLE == m_pStateCom->Get_CurrState())
		{
			if (CCharacter::STATE::FLYING_STAND != pStateCom->Get_CurrState() &&
				CCharacter::STATE::FLYING_RUNSTART != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::FLYING_STAND);
		}
	}
}

void CVehicle_Flying_Biplane::On_Damaged(const COLLISION_INFO& tInfo)
{
	// 데미지 처리.
}

HRESULT CVehicle_Flying_Biplane::Ready_Colliders()
{

	return S_OK;
}

CVehicle_Flying_Biplane* CVehicle_Flying_Biplane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVehicle_Flying_Biplane* pInstance = new CVehicle_Flying_Biplane(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CVehicle_Flying_Biplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVehicle_Flying_Biplane::Clone(void* pArg)
{
	CVehicle_Flying_Biplane* pInstance = new CVehicle_Flying_Biplane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CVehicle_Flying_Biplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVehicle_Flying_Biplane::Free()
{
	__super::Free();
}
