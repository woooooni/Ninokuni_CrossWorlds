#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying_EnemyBoto.h"

#include "Particle_Manager.h"
#include "Utils.h"

#include "State_Enemy_VehicleFlying_Stand.h"
#include "State_Enemy_VehicleFlying_Run.h"
#include "State_Enemy_VehicleFlying_Trace.h"
#include "State_Enemy_VehicleFlying_Attack.h"
#include "State_Enemy_VehicleFlying_Dead.h"

#include "Grandprix_Enemy.h"

#include "UIMinigame_Manager.h"
#include "UI_Minigame_WorldHP.h"
#include "UI_Minigame_Aim.h"
#include "UI_Grandprix_RaderIcon.h"

CVehicle_Flying_EnemyBoto::CVehicle_Flying_EnemyBoto(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVehicle_Flying(pDevice, pContext, L"Vehicle_Flying_EnemyBoto", OBJ_TYPE::OBJ_GRANDPRIX_ENEMY)
{
}

CVehicle_Flying_EnemyBoto::CVehicle_Flying_EnemyBoto(const CVehicle_Flying_EnemyBoto& rhs)
	: CVehicle_Flying(rhs)
{
}

void CVehicle_Flying_EnemyBoto::Set_BiggerSize(_bool bSize)
{
	if (true == bSize)
	{
		if (nullptr != m_pHP)
		{
			m_pHP->Update_Offset(10.5f);

			_float fCX = 3.75f;
			_float fCY = 1.f;
			m_pHP->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(XMVectorSet(fCX, fCY, 1.f, 0.f));
		}
	}
	else
	{
		if (nullptr != m_pHP)
		{
			m_pHP->Update_Offset(2.4f);

			_float fCX = 0.75f;
			_float fCY = 0.2f;
			m_pHP->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(XMVectorSet(fCX, fCY, 1.f, 0.f));
			
		}
	}

	m_bSize = bSize;
}

HRESULT CVehicle_Flying_EnemyBoto::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBoto::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;
	 
	if (FAILED(Ready_States()))
		return E_FAIL;
	
	m_bUseBone = true; 

	m_tStat.bIsEnemy = true;
	m_tStat.fMaxHP = 50000.f;
	m_tStat.fCurHP = 50000.f;

	CGameObject* pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Enemy_WorldHP"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTemp)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_WorldHP*>(pTemp))
		return E_FAIL;

	m_pHP = dynamic_cast<CUI_Minigame_WorldHP*>(pTemp);
	m_pHP->Set_VehicleInformation(this);

	pTemp = nullptr;
	pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Enemy_Aim"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTemp)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_Aim*>(pTemp))
		return E_FAIL;
	m_pAim = dynamic_cast<CUI_Minigame_Aim*>(pTemp);
	m_pAim->Set_Owner(this);

	CGameObject* pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_RaderIcon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon))
		return E_FAIL;
	m_pRaderIcon = dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon);
	m_pRaderIcon->Set_Owner(this);

	m_pRigidBodyCom->Set_Ground(true);
	m_pRigidBodyCom->Set_Use_Gravity(false);

	return S_OK;
}

void CVehicle_Flying_EnemyBoto::Tick(_float fTimeDelta)
{
	if (true == m_bReserveDead)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
		m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_Dead(true);
			return;
		}
	}

	if (nullptr == m_pRider)
		return;

	if (true == m_bOnBoard)
	{
		__super::Tick(fTimeDelta);

		GI->Add_CollisionGroup(COLLISION_GROUP::PLANEENEMY_BODY, this);

		if (nullptr != m_pHP)
			m_pHP->Tick(fTimeDelta);

		if (nullptr != m_pAim)
			m_pAim->Tick(fTimeDelta);
		
		Update_RiderState();

		if (VEHICLE_STATE::VEHICLE_RUN == m_pStateCom->Get_CurrState() ||
			VEHICLE_STATE::VEHICLE_IDLE == m_pStateCom->Get_CurrState() ||
			VEHICLE_STATE::VEHICLE_TRACE == m_pStateCom->Get_CurrState())
		{
			GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect,
				CUtils::Random_Float(0.8f, 0.8f), fTimeDelta, TEXT("Particle_Smoke"), this, _float3(0.f, 1.f, -1.6f));
		}

		if (nullptr != m_pRaderIcon)
		{
			if (true == CUI_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
				m_pRaderIcon->Set_Active(true);
			else
				m_pRaderIcon->Set_Active(false);

			m_pRaderIcon->Tick(fTimeDelta);
		}
	}

	if (nullptr != m_pControllerCom)
		m_pControllerCom->Tick_Controller(fTimeDelta);

}

void CVehicle_Flying_EnemyBoto::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRider)
		return;

	if (true == m_bOnBoard)
	{
		__super::LateTick(fTimeDelta);

		if (nullptr != m_pHP)
			m_pHP->LateTick(fTimeDelta);

		if (nullptr != m_pAim)
			m_pAim->LateTick(fTimeDelta);

		if (nullptr != m_pRaderIcon)
		{
			if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
				m_pRaderIcon->LateTick(fTimeDelta);
		}

		Update_Rider(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
}

HRESULT CVehicle_Flying_EnemyBoto::Render()
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

		_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
			return E_FAIL;

		if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;

		const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		if (false == m_bUseTextureCom)
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
			if (5 < m_iTextureIndex)
				m_iTextureIndex = 5;

			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBoto::Render_ShadowDepth()
{
	if (true == m_bOnBoard)
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

		if (false == m_bUseTextureCom)
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
			if (5 < m_iTextureIndex)
				m_iTextureIndex = 5;

			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
					return E_FAIL;
			}
		}

		return S_OK;
	}

	return S_OK;
}

void CVehicle_Flying_EnemyBoto::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
	{
		if (CCollider::DETECTION_TYPE::BODY == tInfo.pMyCollider->Get_DetectionType() && CCollider::DETECTION_TYPE::ATTACK == tInfo.pOtherCollider->Get_DetectionType())
		{
			On_Damaged(tInfo);
		}
	}
}

void CVehicle_Flying_EnemyBoto::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Flying_EnemyBoto::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Flying_EnemyBoto::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Flying_EnemyBoto::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Flying_EnemyBoto::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Flying_EnemyBoto::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boto"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// For Texture Component
	if (FAILED(__super::Add_Component(LEVEL_EVERMORE,
		TEXT("Prototype_Component_Texture_Vehicle_Minigame_Grandprix_Boto_Textures"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	m_pDissolveTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));
	if (m_pDissolveTextureCom == nullptr)
		return E_FAIL;

	

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBoto::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Stand");
//	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Idle01");
//	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Idle02");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_Enemy_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));

//	strAnimationNames.clear();
//	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Walk");
//	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_WALK, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Walk");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_TRACE, CState_Enemy_VehicleFlying_Trace::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ATTACK, CState_Enemy_VehicleFlying_Attack::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Boto.ao|Boto_Idle01");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_DEAD, CState_Enemy_VehicleFlying_Dead::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);

	return S_OK;
}

void CVehicle_Flying_EnemyBoto::Update_RiderState()
{
	if (nullptr != m_pRider)
	{
		if (m_pRider->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_ENEMY)
			return;

		CStateMachine* pStateCom = m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
		if (nullptr == pStateCom)
			return;

		if (CVehicle::VEHICLE_STATE::VEHICLE_RUN == m_pStateCom->Get_CurrState() &&
			CGrandprix_Enemy::ENEMY_STATE::FLYING_RUNSTART != pStateCom->Get_CurrState())
		{
			if (CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN);
		}

		if (CVehicle::VEHICLE_STATE::VEHICLE_IDLE == m_pStateCom->Get_CurrState())
		{
			if (CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND != pStateCom->Get_CurrState() &&
				CGrandprix_Enemy::ENEMY_STATE::FLYING_RUNSTART != pStateCom->Get_CurrState())
				m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND);
		}
	}
}
void CVehicle_Flying_EnemyBoto::On_Damaged(const COLLISION_INFO& tInfo)
{
	_int iDamage = 3000;
	if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Character_Biplane_Bullet"))
	{
		iDamage = iDamage * 0.3f + GI->RandomInt(-300, 300);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.05f, 17.f, 0.3f);
	}
	else if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Biplane_Thunder_Cloud"))
	{
		iDamage = iDamage * 0.7f + GI->RandomInt(-300, 300);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.1f, 17.f, 0.3f);
	}
	else if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Biplane_GuidedMissile"))
	{
		iDamage = iDamage * 0.5f + GI->RandomInt(-300, 300);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.3f, 17.f, 0.3f);
	}

	

	m_tStat.fCurHP = max(0, m_tStat.fCurHP - iDamage);
	if (0.f >= m_tStat.fCurHP)
	{
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.5f, 19.f, 0.5f);
		m_pRider->Set_Dead(true);
		m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_DEAD);
		Reserve_Dead(true);
		return;
	}
	/*else
	{
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.3f, 19.f, 0.3f);
	}*/
}


HRESULT CVehicle_Flying_EnemyBoto::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 2.f;

	SphereDesc.tSphere = tSphere;
	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CVehicle_Flying_EnemyBoto* CVehicle_Flying_EnemyBoto::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVehicle_Flying_EnemyBoto* pInstance = new CVehicle_Flying_EnemyBoto(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CVehicle_Flying_EnemyBoto");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVehicle_Flying_EnemyBoto::Clone(void* pArg)
{
	CVehicle_Flying_EnemyBoto* pInstance = new CVehicle_Flying_EnemyBoto(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CVehicle_Flying_EnemyBoto");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVehicle_Flying_EnemyBoto::Free()
{
	__super::Free();

	Safe_Release(m_pRaderIcon);
	Safe_Release(m_pAim);
	Safe_Release(m_pHP);
	Safe_Release(m_pDissolveTextureCom);
}
