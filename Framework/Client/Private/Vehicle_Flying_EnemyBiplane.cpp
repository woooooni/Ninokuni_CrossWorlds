#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying_EnemyBiplane.h"

#include "State_Enemy_VehicleFlying_Enter.h"
#include "State_Enemy_VehicleFlying_Stand.h"
#include "State_Enemy_VehicleFlying_Run.h"

#include "State_EnemyBiplane_Stand.h"
#include "State_EnemyBiplane_Finish_Attack.h"
#include "State_EnemyBiplane_Skill_0.h"
#include "State_EnemyBiplane_Skill_1.h"
#include "State_EnemyBiplane_Skill_2.h"
#include "State_EnemyBiplane_Dead.h"

#include "Grandprix_Enemy.h"
#include "Grandprix_Manager.h"

#include "UIMinigame_Manager.h"
#include "UI_Minigame_WorldHP.h"
#include "UI_Minigame_Aim.h"
#include "UI_Grandprix_RaderIcon.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "Grandprix_Engineer.h"

#include "Trail.h"



CVehicle_Flying_EnemyBiplane::CVehicle_Flying_EnemyBiplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVehicle_Flying(pDevice, pContext, L"Vehicle_Flying_EnemyBiplane", OBJ_GRANDPRIX_ENEMY)
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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_Trails()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	
	
	m_bUseBone = false; 

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

//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(0.12f, -0.09, 30.2f, 1.f));
	pTemp = nullptr;
	pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Enemy_Aim"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTemp)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_Aim*>(pTemp))
		return E_FAIL;
	m_pAim = dynamic_cast<CUI_Minigame_Aim*>(pTemp);
	m_pAim->Set_Owner(this);

	CGameObject* pIcon = nullptr;
	pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_RaderIcon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon))
		return E_FAIL;
	m_pRaderIcon = dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon);
	m_pRaderIcon->Set_Owner(this);

	m_pRigidBodyCom->Set_Use_Gravity(false);

	return S_OK;
}

void CVehicle_Flying_EnemyBiplane::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		if (false == m_bInfinite && KEY_TAP(KEY::Z))
		{
			m_tStat.fCurHP -= 10000;
			m_tStat.fCurHP = max(m_tStat.fCurHP, 0);
			if (0 >= m_tStat.fCurHP)
			{
				m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_FINISH_ATTACK);
			}
		}

		if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::Z))
		{
			for (auto& pObject : GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER))
			{
				if (pObject->Get_ObjectTag() == L"Vehicle_Flying_EnemyBiplane")
					continue;

				pObject->Set_Dead(true);
			}
		}

		Check_Use_Skill();

		__super::Tick(fTimeDelta);


		if (true == m_bInfinite)
		{
			m_fAccInfinite += fTimeDelta;
			if (m_fAccInfinite >= m_fInfiniteTime)
			{
				m_bInfinite = false;
				m_fAccInfinite = 0.f;
			}
		}

		if (true == m_bReserveDead)
		{
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
			if (m_fDissolveWeight >= m_fDissolveTotal)
			{
				Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
				Set_Dead(true);
			}
		}


		m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

		if (nullptr != m_pHP)
			m_pHP->Tick(fTimeDelta);

		if (nullptr != m_pAim)
			m_pAim->Tick(fTimeDelta);
		
		Update_RiderState();

		if (nullptr != m_pRaderIcon)
		{
			if (true == CUI_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
				m_pRaderIcon->Set_Active(true);
			else
				m_pRaderIcon->Set_Active(false);

			m_pRaderIcon->Tick(fTimeDelta);
		}

		for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
		{
			if (nullptr != m_pTrails[i])
			{
				m_pTrails[i]->Set_TransformMatrix(m_pModelCom->Get_SocketLocalMatrix(i + 2) * m_pTransformCom->Get_WorldMatrix());
				m_pTrails[i]->Tick(fTimeDelta);
			}

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

HRESULT CVehicle_Flying_EnemyBiplane::Render()
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

		// Dissolve -----------------
		if (FAILED(m_pDissolveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
			return E_FAIL;
		// ----------------- Dissolve

		if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
			return E_FAIL;

		const _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		_uint iPassIndex = 0;
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (m_bReserveDead)
				iPassIndex = 2;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
				return E_FAIL;
		}

		return S_OK;


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
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
	{
		if (CCollider::DETECTION_TYPE::ATTACK == tInfo.pOtherCollider->Get_DetectionType())
		{
			On_Damaged(tInfo);
		}
	}
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

void CVehicle_Flying_EnemyBiplane::Start_Trail(BIPLANE_TRAIL eTrailType)
{
	if (eTrailType >= BIPLANE_TRAIL::BIPLANE_TRAIL_END)
	{
		for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
		{
			m_pTrails[i]->Start_Trail(m_pModelCom->Get_SocketLocalMatrix(i + 2) * m_pTransformCom->Get_WorldMatrix());
		}
	}
	else
	{
		m_pTrails[eTrailType]->Start_Trail(m_pModelCom->Get_SocketLocalMatrix(eTrailType + 2) * m_pTransformCom->Get_WorldMatrix());
	}
}

void CVehicle_Flying_EnemyBiplane::Generate_Trail(const wstring& strDiffuseTextureName, const wstring& strAlphaTextureName, const wstring& strDistortionTextureName, const _float4& vColor, _uint iVertexCount, BIPLANE_TRAIL eTrailType)
{
	if (eTrailType >= BIPLANE_TRAIL::BIPLANE_TRAIL_END)
	{
		for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
		{
			m_pTrails[i]->Set_DiffuseTexture_Index(strDiffuseTextureName);
			m_pTrails[i]->Set_AlphaTexture_Index(strAlphaTextureName);
			m_pTrails[i]->Set_DistortionTexture_Index(strDistortionTextureName);
			m_pTrails[i]->Set_Color(vColor);
			m_pTrails[i]->Start_Trail(m_pModelCom->Get_SocketLocalMatrix(i + 2) * m_pTransformCom->Get_WorldMatrix());
		}

	}
	else
	{
		m_pTrails[eTrailType]->Set_DiffuseTexture_Index(strDiffuseTextureName);
		m_pTrails[eTrailType]->Set_AlphaTexture_Index(strAlphaTextureName);
		m_pTrails[eTrailType]->Set_DistortionTexture_Index(strDistortionTextureName);
		m_pTrails[eTrailType]->Set_Color(vColor);
		m_pTrails[eTrailType]->Start_Trail(m_pModelCom->Get_SocketLocalMatrix(eTrailType + 2) * m_pTransformCom->Get_WorldMatrix());
	}
}

void CVehicle_Flying_EnemyBiplane::Stop_Trail(BIPLANE_TRAIL eTrailType)
{
	if (eTrailType >= BIPLANE_TRAIL::BIPLANE_TRAIL_END)
	{
		for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
			m_pTrails[i]->Stop_Trail();
	}
	else
	{
		m_pTrails[eTrailType]->Stop_Trail();
	}
}

void CVehicle_Flying_EnemyBiplane::Set_Infinite(_bool bInfinite, _float fInfiniteTime)
{
	m_fInfiniteTime = fInfiniteTime;
	m_fAccInfinite = 0.f;
	m_bInfinite = bInfinite;

	CGrandprix_Engineer* pEngineer = dynamic_cast<CGrandprix_Engineer*>(m_pRider);

	if (nullptr != pEngineer)	
		pEngineer->Set_Infinite(bInfinite, fInfiniteTime);	

}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Biplane"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	m_pDissolveTexture = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));

	if (m_pDissolveTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENTER, CState_Enemy_VehicleFlying_Enter::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_Enemy_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN, CState_Enemy_VehicleFlying_Run::Create(m_pStateCom, strAnimationNames));

	
	/*"State_EnemyBiplane_Skill_0.h"
	"State_EnemyBiplane_Skill_1.h"
	"State_EnemyBiplane_Skill_2.h"*/

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND, CState_EnemyBiplane_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_FINISH_ATTACK, CState_EnemyBiplane_Finish_Attack::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_0, CState_EnemyBiplane_Skill_0::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_1, CState_EnemyBiplane_Skill_1::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_2, CState_EnemyBiplane_Skill_2::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_DEAD, CState_EnemyBiplane_Dead::Create(m_pStateCom, strAnimationNames));


	m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENTER);

	return S_OK;
}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_Trails()
{
	CTrail::TRAIL_DESC TrailDesc = {};
	TrailDesc.bTrail = false;
	TrailDesc.bUV_Cut = false;
	TrailDesc.fAccGenTrail = 0.f;
	TrailDesc.fGenTrailTime = 0.1f;
	TrailDesc.vDiffuseColor = { 1.f, 0.3f, 0.f, 0.2f };
	TrailDesc.vDistortion = { 0.1f, 0.1f };
	TrailDesc.vUVAcc = { 0.f, 0.f };
	TrailDesc.vUV_FlowSpeed = { 0.f, 0.f };
	TrailDesc.fAlphaDiscard = 0.1f;
	TrailDesc.vBlackDiscard = { -1.f, -1.f, -1.f };

	m_pTrails[BIPLANE_TRAIL::LEFT_WING] = CTrail::Create(m_pDevice, m_pContext, L"Prototype_GameObject_Trail", TrailDesc);
	m_pTrails[BIPLANE_TRAIL::RIGHT_WING] = CTrail::Create(m_pDevice, m_pContext, L"Prototype_GameObject_Trail", TrailDesc);
	m_pTrails[BIPLANE_TRAIL::TAIL] = CTrail::Create(m_pDevice, m_pContext, L"Prototype_GameObject_Trail", TrailDesc);

	if (nullptr == m_pTrails[BIPLANE_TRAIL::LEFT_WING] || nullptr == m_pTrails[BIPLANE_TRAIL::RIGHT_WING] || nullptr == m_pTrails[BIPLANE_TRAIL::TAIL])
		return E_FAIL;

	for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
	{
		if (FAILED(m_pTrails[i]->Initialize(nullptr)))
			return E_FAIL;

		if (i <= BIPLANE_TRAIL::RIGHT_WING)
			m_pTrails[i]->SetUp_Position(XMVectorSet(-0.05f, 0.f, 0.f, 1.f), XMVectorSet(0.05f, 0.f, 0.f, 1.f));
		else
			m_pTrails[i]->SetUp_Position(XMVectorSet(0.f, -0.05f, 0.f, 1.f), XMVectorSet(0.f, 0.05f, 0.f, 1.f));

		m_pTrails[i]->Stop_Trail();
	}

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

		switch (m_pStateCom->Get_CurrState())
		{
		case CVehicle::VEHICLE_STATE::VEHICLE_ENTER:
		case CVehicle::VEHICLE_STATE::VEHICLE_IDLE:
			if (CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND != pStateCom->Get_CurrState())
			{
				pStateCom->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND);
			}
			break;

		case CVehicle::VEHICLE_STATE::VEHICLE_RUN:
		case CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND:
		case CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_FINISH_ATTACK:
		case CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_0:
		case CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_1:
		case CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_2:
		case CVehicle::VEHICLE_STATE::VEHICLE_DEAD:
			if (CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN != pStateCom->Get_CurrState())
			{
				m_pStateCom->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN);
			}
			break;

		}
	}
}

_bool CVehicle_Flying_EnemyBiplane::Check_Use_Skill()
{
	_float fHpRatio = m_tStat.fCurHP / m_tStat.fMaxHP;
	if (false == m_bLaunch_Pattern1 && fHpRatio <= 0.7f)
	{
		m_bLaunch_Pattern1 = true;
		m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_0);
		return true;
	}

	if (false == m_bLaunch_Pattern2 && fHpRatio <= 0.5f)
	{
		m_bLaunch_Pattern2 = true;
		m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_1);
		return true;
	}

	if (false == m_bLaunch_Pattern3 && fHpRatio <= 0.3f)
	{
		m_bLaunch_Pattern3 = true;
		m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_SKILL_2);
		return true;
	}

	return false;
}

void CVehicle_Flying_EnemyBiplane::On_Damaged(const COLLISION_INFO& tInfo)
{
	if (true == m_bInfinite)
		return;

	wstring strAttackerName = tInfo.pOther->Get_ObjectTag();

	_int iDamage = 300;
	if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Character_Biplane_Bullet"))
	{
		iDamage = iDamage * 0.1f + GI->RandomInt(-30, 30);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.05f, 17.f, 0.3f);
	}
	else if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Biplane_Thunder_Cloud"))
	{
		iDamage = iDamage * 0.7f + GI->RandomInt(-30, 30);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.1f, 17.f, 0.3f);
	}
	else if (wstring::npos != tInfo.pOther->Get_ObjectTag().find(L"Biplane_GuidedMissile"))
	{
		iDamage = iDamage * 0.3f + GI->RandomInt(-30, 30);
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.3f, 17.f, 0.3f);
	}

	m_tStat.fCurHP = max(0, m_tStat.fCurHP - iDamage);

	if (0.f >= m_tStat.fCurHP)
	{
		if (m_pStateCom->Get_CurrState() != CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_DEAD)
		{
			m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_DEAD);
			return;
		}
	}
}

HRESULT CVehicle_Flying_EnemyBiplane::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	SphereDesc.tSphere = tSphere;
	SphereDesc.tSphere.Radius = 1.25f;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 150.f, 0.f);

	
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CVehicle_Flying_EnemyBiplane* CVehicle_Flying_EnemyBiplane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVehicle_Flying_EnemyBiplane* pInstance = new CVehicle_Flying_EnemyBiplane(pDevice, pContext);

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

	Safe_Release(m_pRaderIcon);
	Safe_Release(m_pAim);
	Safe_Release(m_pHP);

	for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)	
		Safe_Release(m_pTrails[i]);

	Safe_Release(m_pDissolveTexture);
}
