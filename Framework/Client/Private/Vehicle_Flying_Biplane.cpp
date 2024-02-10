#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying_Biplane.h"

#include "State_VehicleFlying_Stand.h"
#include "State_VehicleFlying_Run.h"
#include "State_VehicleFlying_Rush.h"
#include "State_VehicleFlying_Damaged.h"
#include "State_VehicleFlying_Backflip.h"

#include "Character.h"

#include "UIMinigame_Manager.h"
#include "Trail.h"
#include "UI_Grandprix_RaderIcon.h"
#include "Grandprix_ItemBox.h"

#include "Pool.h"
#include "Character_Biplane_Bullet.h"
#include "VehicleFlying_Projectile.h"

#include "Camera_Follow.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "State_VehicleFlying_BossStage_Stand.h"

CVehicle_Flying_Biplane::CVehicle_Flying_Biplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVehicle_Flying(pDevice, pContext, L"Vehicle_Flying_PlayerBiplane", OBJ_GRANDPRIX_CHARACTER)
{
}

CVehicle_Flying_Biplane::CVehicle_Flying_Biplane(const CVehicle_Flying_Biplane& rhs)
	: CVehicle_Flying(rhs)
{
}

HRESULT CVehicle_Flying_Biplane::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying_Biplane::Initialize(void* pArg)
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
//	m_fOffsetY = -0.5f;

	if (nullptr != m_pRider)
	{
		if (OBJ_TYPE::OBJ_PLAYER == m_pRider->Get_ObjectType())
			m_bIsPlayers = true;
	}

	

	CGameObject* pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_RaderIcon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon))
		return E_FAIL;
	m_pCameraIcon = dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon);
	m_pCameraIcon->Set_Owner(this, true);

	pIcon = nullptr;
	pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_RaderIcon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon))
		return E_FAIL;
	m_pRaderIcon = dynamic_cast<CUI_Grandprix_RaderIcon*>(pIcon);
	m_pRaderIcon->Set_Owner(this);

	m_pCameraFollow = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr == m_pCameraFollow)
		return E_FAIL;

	return S_OK;
}

void CVehicle_Flying_Biplane::Tick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		Tick_Target();

		__super::Tick(fTimeDelta);
		GI->Add_CollisionGroup(COLLISION_GROUP::PLANE_BODY, this);

		// Update_RiderState();


		if (nullptr != m_pRigidBodyCom)
			m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

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

		if (nullptr != m_pCameraIcon)
		{
			if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
 				m_pCameraIcon->Tick(fTimeDelta);
		}

		if (nullptr != m_pRaderIcon)
		{
			if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
				m_pRaderIcon->Set_Active(true);
			else
				m_pRaderIcon->Set_Active(false);

			m_pRaderIcon->Tick(fTimeDelta);
		}
	}
}

void CVehicle_Flying_Biplane::LateTick(_float fTimeDelta)
{
	if (true == m_bOnBoard)
	{
		__super::LateTick(fTimeDelta);

		for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
		{
			if (nullptr != m_pTrails[i])
				m_pTrails[i]->LateTick(fTimeDelta);
		}

		Update_Rider(fTimeDelta);

		if (nullptr != m_pCameraIcon)
		{
			if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
				m_pCameraIcon->LateTick(fTimeDelta);
		}

		if (nullptr != m_pRaderIcon)
		{
			if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
				m_pRaderIcon->LateTick(fTimeDelta);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}

#ifdef _DEBUG
//	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
//	{
//		for (auto& pCollider : m_Colliders[i])
//			m_pRendererCom->Add_Debug(pCollider);
//	}
#endif
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

		if (true == m_bInfinite)
			vRimColor = { 0.f, 1.f, 1.f, 1.f };

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
	if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (OBJ_TYPE::OBJ_GRANDPRIX_ENEMY_PROJECTILE == tInfo.pOther->Get_ObjectType())
		{
			On_Damaged(tInfo);
		}

		if (tInfo.pOther->Get_ObjectTag() == TEXT("Grandprix_ItemBox"))
		{
			if (nullptr == dynamic_cast<CGrandprix_ItemBox*>(tInfo.pOther))
				return;

			if (dynamic_cast<CGrandprix_ItemBox*>(tInfo.pOther)->Get_ItemType()
				!= CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB)
				return;

			Vec4 vItemPos = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
			Vec4 vMyPos = m_pTransformCom->Get_Position();

			m_pRigidBodyCom->Add_Velocity(XMVector4Normalize(vMyPos - vItemPos), 40.f, true);
			m_pStateCom->Change_State(VEHICLE_BACKFLIP);
		}
	}

}

void CVehicle_Flying_Biplane::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
	if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		if (OBJ_TYPE::OBJ_GRANDPRIX_ENEMY == tInfo.pOther->Get_ObjectType())
		{
			if(m_pStateCom->Get_CurrState() != CVehicle::VEHICLE_STATE::VEHICLE_DAMAGED)
				Decide_Target();
		}
	}
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

void CVehicle_Flying_Biplane::Start_Trail(BIPLANE_TRAIL eTrailType)
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

void CVehicle_Flying_Biplane::Generate_Trail(const wstring& strDiffuseTextureName, const wstring& strAlphaTextureName, const wstring& strDistortionTextureName, const _float4& vColor, _uint iVertexCount, BIPLANE_TRAIL eTrailType)
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

void CVehicle_Flying_Biplane::Stop_Trail(BIPLANE_TRAIL eTrailType)
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
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_RUSH, CState_VehicleFlying_Rush::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_DAMAGED, CState_VehicleFlying_Damaged::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_BACKFLIP, CState_VehicleFlying_Backflip::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Run");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE, CState_VehicleFlying_BossStage_Stand::Create(m_pStateCom, strAnimationNames));


	/*strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Biplane.ao|Biplane_Stand");
	m_pStateCom->Add_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE, CState_VehicleFlying_Stand::Create(m_pStateCom, strAnimationNames));*/

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

HRESULT CVehicle_Flying_Biplane::Ready_Trails()
{
	CTrail::TRAIL_DESC TrailDesc = {};
	TrailDesc.bTrail = false;
	TrailDesc.bUV_Cut = false;
	TrailDesc.fAccGenTrail = 0.f;
	TrailDesc.fGenTrailTime = 0.1f;
	TrailDesc.vDiffuseColor = { 1.f, 1.f, 1.f, 0.2f };
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

		if(i <= BIPLANE_TRAIL::RIGHT_WING)
			m_pTrails[i]->SetUp_Position(XMVectorSet(-0.05f, 0.f, 0.f, 1.f), XMVectorSet(0.05f, 0.f, 0.f, 1.f));
		else
			m_pTrails[i]->SetUp_Position(XMVectorSet(0.f, -0.05f, 0.f, 1.f), XMVectorSet(0.f, 0.05f, 0.f, 1.f));

		m_pTrails[i]->Stop_Trail();
	}


	return S_OK;
}

void CVehicle_Flying_Biplane::Look_For_Target()
{
	if (nullptr == m_pTarget)
		return;

	CTransform* pTargetTransform = m_pTarget->Get_Component_Transform();
	Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() - m_pTransformCom->Get_Position());

	m_pTransformCom->Rotation_Look(vDir);
}

void CVehicle_Flying_Biplane::Decide_Target()
{
	list<CGameObject*>& Targets = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);
	CGameObject* pDecidedTarget = nullptr;
	_float fMinDistance = 50.f;

	for (auto& pTarget : Targets)
	{
		if (pTarget->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_ENEMY)
			continue;

		if (true == pTarget->Is_ReserveDead() || true == pTarget->Is_Dead())
			continue;

		CTransform* pTargetTransform = pTarget->Get_Component_Transform();
		if (nullptr == pTargetTransform)
			continue;
		
		Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
		_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(m_pTransformCom->Get_Look())));
		if (fMinDistance > vDir.Length() && 0.f <= fDot)
		{
			pDecidedTarget = pTarget;
			fMinDistance = vDir.Length();
		}
	}

	if (nullptr == pDecidedTarget)
		return;

	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
	}

	/*CCamera_Follow* pCameraFollow = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pCameraFollow)
	{
		if (nullptr == pCameraFollow->Get_LookAtObj() 
			|| pCameraFollow->Get_LookAtObj()->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE)
		{
			pCameraFollow->Start_LockOn(pDecidedTarget, Vec4(0.f, 0.f, -15.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f));
		}
	}*/
		

	m_pTarget = pDecidedTarget;
	Safe_AddRef(m_pTarget);

	if (nullptr != dynamic_cast<CVehicle_Flying*>(m_pTarget))
		CUIMinigame_Manager::GetInstance()->Set_TargetUI(dynamic_cast<CVehicle_Flying*>(m_pTarget));
}

void CVehicle_Flying_Biplane::Tick_Target()
{
	if (nullptr == m_pTarget)
		return;

	if (true == m_pTarget->Is_Dead() || true == m_pTarget->Is_ReserveDead())
	{
		
		//if (nullptr != m_pCameraFollow)
		//{
		//	m_pCameraFollow->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//	m_pCameraFollow->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//	m_pCameraFollow->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//}
			

		Safe_Release(m_pTarget);
		m_pTarget = nullptr;

		return;
	}

	CTransform* pTargetTransform = m_pTarget->Get_Component_Transform();
	if (nullptr == pTargetTransform)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
		return;
	}

	Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
	
	_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(m_pTransformCom->Get_Look())));
	if (vDir.Length() > 50.f || 0 > fDot)
	{
		//if (nullptr != m_pCameraFollow)
		//{
		//	m_pCameraFollow->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//	m_pCameraFollow->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//	m_pCameraFollow->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		//}
			

		Safe_Release(m_pTarget);
		m_pTarget = nullptr;

		return;
	}
}

void CVehicle_Flying_Biplane::Update_RiderState()
{
	if (nullptr != m_pRider)
	{
		CStateMachine* pStateCom = m_pRider->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
		if (nullptr == pStateCom)
			return;

		switch (m_pStateCom->Get_CurrState())
		{
		case CVehicle::VEHICLE_STATE::VEHICLE_RUN :
		case CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE:
			if (CCharacter::STATE::FLYING_RUN != pStateCom->Get_CurrState())
			{
				pStateCom->Change_State(CCharacter::STATE::FLYING_RUN);
			}
			break;

		case CVehicle::VEHICLE_STATE::VEHICLE_IDLE:
		case CVehicle::VEHICLE_STATE::VEHICLE_RUSH:
		case CVehicle::VEHICLE_STATE::VEHICLE_DAMAGED:
		case CVehicle::VEHICLE_STATE::VEHICLE_BACKFLIP:
			if (CCharacter::STATE::FLYING_STAND != pStateCom->Get_CurrState())
			{
				pStateCom->Change_State(CCharacter::STATE::FLYING_STAND);
			}
			break;


		}

	}
}

void CVehicle_Flying_Biplane::On_Damaged(const COLLISION_INFO& tInfo)
{
	if (true == m_bInfinite)
		return;

	wstring strAttackerName = tInfo.pOther->Get_PrototypeTag();

	if (wstring::npos != strAttackerName.find(L"Enemy_Biplane_Bullet"))
	{
		CTransform* pOtherTransform = tInfo.pOther->Get_Component_Transform();
		Vec3 vDir = m_pTransformCom->Get_Position() - pOtherTransform->Get_Position();
		vDir = XMVector3Normalize(vDir);


		// 데미지 공식
		m_tStat.fCurHP = max(0.f, m_tStat.fCurHP - 1500.f);

		if (m_pStateCom->Get_CurrState() != CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE)
		{
			m_pRigidBodyCom->Set_FrictionScale(2.f);
			m_pRigidBodyCom->Add_Velocity(vDir, 20.f, true);
		}
		
	}

	else if (wstring::npos != strAttackerName.find(L"Enemy_GuidedMissile"))
	{
		CTransform* pOtherTransform = tInfo.pOther->Get_Component_Transform();
		Vec3 vDir = m_pTransformCom->Get_Position() - pOtherTransform->Get_Position();
		vDir = XMVector3Normalize(vDir);

		// 데미지 공식
		m_tStat.fCurHP = max(0.f, m_tStat.fCurHP - 3000.f);

		if (m_pStateCom->Get_CurrState() != CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE)
		{
			m_pRigidBodyCom->Set_FrictionScale(2.f);
			m_pRigidBodyCom->Add_Velocity(vDir, 30.f, true);
		}
	}

	CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.3f, 19.f, 0.3f);

	if(m_pStateCom->Get_CurrState() != CVehicle::VEHICLE_STATE::VEHICLE_BOSS_IDLE)
		m_pStateCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_DAMAGED);
}

HRESULT CVehicle_Flying_Biplane::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 50.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 150.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;



	SphereDesc.tSphere.Radius = 1.25f;
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	

	return S_OK;
}

CVehicle_Flying_Biplane* CVehicle_Flying_Biplane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVehicle_Flying_Biplane* pInstance = new CVehicle_Flying_Biplane(pDevice, pContext);

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
	Safe_Release(m_pCameraIcon);
	Safe_Release(m_pRaderIcon);

	for (_uint i = 0; i < BIPLANE_TRAIL::BIPLANE_TRAIL_END; ++i)
		Safe_Release(m_pTrails[i]);
}
