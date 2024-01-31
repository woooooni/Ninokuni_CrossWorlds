#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying_EnemyBiplane.h"

#include "State_Enemy_VehicleFlying_Stand.h"
#include "State_Enemy_VehicleFlying_Run.h"
#include "State_Enemy_VehicleFlying_Trace.h"
#include "State_Enemy_VehicleFlying_Attack.h"
#include "State_Enemy_VehicleFlying_Dead.h"

#include "Grandprix_Enemy.h"

#include "UIMinigame_Manager.h"
#include "UI_Minigame_WorldHP.h"

CVehicle_Flying_EnemyBiplane::CVehicle_Flying_EnemyBiplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVehicle_Flying(pDevice, pContext, strObjectTag)
{
}

CVehicle_Flying_EnemyBiplane::CVehicle_Flying_EnemyBiplane(const CVehicle_Flying_EnemyBiplane& rhs)
	: CVehicle_Flying(rhs)
{
}

HRESULT CVehicle_Flying_EnemyBiplane::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBiplane::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;
	
	m_bUseBone = false; 

	m_eStat.bIsEnemy = true;
	m_eStat.fMaxHP = 100000.f;
	m_eStat.fCurHP = 100000.f;

	CGameObject* pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Enemy_WorldHP"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTemp)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_WorldHP*>(pTemp))
		return E_FAIL;

	m_pHP = dynamic_cast<CUI_Minigame_WorldHP*>(pTemp);
	m_pHP->Set_VehicleInformation(this);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(0.12f, -0.09, 30.2f, 1.f));

	return S_OK;
}

void CVehicle_Flying_EnemyBiplane::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::Tick(fTimeDelta);

		if (nullptr != m_pHP)
			m_pHP->Tick(fTimeDelta);
		
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

void CVehicle_Flying_EnemyBiplane::LateTick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		GI->Add_CollisionGroup(COLLISION_GROUP::PLANEENEMY_BODY, this);

		__super::LateTick(fTimeDelta);

		if (nullptr != m_pHP)
			m_pHP->LateTick(fTimeDelta);

		Update_Rider(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
}

HRESULT CVehicle_Flying_EnemyBiplane::Render()
{
	if (true == m_bOnBoard)
	{
		__super::Render();

	}

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBiplane::Render_ShadowDepth()
{
	if (true == m_bOnBoard)
	{
		__super::Render_ShadowDepth();
	
	}

	return S_OK;
}

void CVehicle_Flying_EnemyBiplane::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle_Flying_EnemyBiplane::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Flying_EnemyBiplane::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Flying_EnemyBiplane::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Flying_EnemyBiplane::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Flying_EnemyBiplane::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Biplane"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_Enemy_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_TRACE, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ATTACK, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_DEAD, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);

	return S_OK;
}

void CVehicle_Flying_EnemyBiplane::Update_RiderState()
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

HRESULT CVehicle_Flying_EnemyBiplane::Ready_Colliders()
{

	return S_OK;
}

CVehicle_Flying_EnemyBiplane* CVehicle_Flying_EnemyBiplane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVehicle_Flying_EnemyBiplane* pInstance = new CVehicle_Flying_EnemyBiplane(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CVehicle_Flying_EnemyBiplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVehicle_Flying_EnemyBiplane::Clone(void* pArg)
{
	CVehicle_Flying_EnemyBiplane* pInstance = new CVehicle_Flying_EnemyBiplane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CVehicle_Flying_EnemyBiplane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVehicle_Flying_EnemyBiplane::Free()
{
	__super::Free();

	Safe_Release(m_pHP);
}
