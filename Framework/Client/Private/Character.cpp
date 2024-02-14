#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "HierarchyNode.h"
#include "Key_Manager.h"
#include "Part.h"
#include "Sword.h"
#include "Trail.h"
#include "Monster.h"
#include "Decal.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "UIDamage_Manager.h"
#include "Utils.h"
#include "Weapon.h"
#include "Animals.h"
#include <future>

#include "Game_Manager.h"
#include "Player.h"
#include "UI_Manager.h"
#include "UI_World_NameTag.h"
#include "UI_Minimap_Icon.h"
#include "MonsterProjectile.h"
#include "Camera_Follow.h"
#include "Kuu.h"
#include "CurlingGame_Prop.h"
#include "CurlingGame_Stone.h"
#include "Particle.h"
#include "Riding_Manager.h"

USING(Client)
CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
	, m_eCharacterType(eCharacterType)
{

}

CCharacter::CCharacter(const CCharacter& rhs)
	: CGameObject(rhs)
	, m_eCharacterType(rhs.m_eCharacterType)
{	

}

HRESULT CCharacter::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		m_tStat = *((CHARACTER_STAT*)pArg);

	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = nullptr;

	CGameObject* pNameTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_World_NameTag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pNameTag)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NameTag*>(pNameTag))
		return E_FAIL;
	m_pName = dynamic_cast<CUI_World_NameTag*>(pNameTag);
	m_pName->Set_Owner(this);

	if (LEVELID::LEVEL_TOOL != (LEVELID)g_eStartLevel)
	{
		//m_pCameraIcon
		CGameObject* pIcon = nullptr;
		pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minimap_Icon"), LAYER_TYPE::LAYER_UI);
		if (nullptr == pIcon)
			return E_FAIL;
		if (nullptr == dynamic_cast<CUI_Minimap_Icon*>(pIcon))
			return E_FAIL;
		m_pCameraIcon = dynamic_cast<CUI_Minimap_Icon*>(pIcon);
		m_pCameraIcon->Set_Owner(this, true);

		//m_pMinimapIcon
		pIcon = nullptr;
		pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minimap_Icon"), LAYER_TYPE::LAYER_UI);
		if (nullptr == pIcon)
			return E_FAIL;
		if (nullptr == dynamic_cast<CUI_Minimap_Icon*>(pIcon))
			return E_FAIL;
		m_pMinimapIcon = dynamic_cast<CUI_Minimap_Icon*>(pIcon);
		m_pMinimapIcon->Set_Owner(this);
	}

	// CEffect_Manager::GetInstance()->Generate_Decal_To_Position(L"Decal_Target")

	return S_OK;
}

void CCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);	

	Vec4 vPosition = m_pTransformCom->Get_Position();
	if (nullptr != m_pTarget)
		Tick_Target(fTimeDelta);

	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::G))
	{
		if (m_tStat.fSpeedWeight >= 10.f)
			m_tStat.fSpeedWeight = 1.f;
		else
			m_tStat.fSpeedWeight = 10.f;
	}

	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::END_KEY))
	{
		//Set_InitialPosition(Vec4(111.f, -0.785f, 8.f, 1.f));
		Set_InitialPosition(Vec4(161.5f, 2.311f, 147.5f, 1.f));
	}

	//if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::INSERT))
	//{
	//	m_pStateCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
	//}

	if (true == m_bInfinite)
	{
		m_fAccInfinite += fTimeDelta;
		if (m_fAccInfinite >= m_fInfiniteTime)
		{
			m_fAccInfinite = 0.f;
			m_bInfinite = false;
		}
	}

	if (true == m_bSuperArmor)
	{
		m_fAccSuperArmor += fTimeDelta;
		if (m_fAccSuperArmor >= m_fSuperArmorTime)
		{
			m_fAccSuperArmor = 0.f;
			m_bSuperArmor = false;
		}
	}

	//if (KEY_TAP(KEY::TAB))
	//{
	//	switch (m_eCharacterType)
	//	{
	//	case CHARACTER_TYPE::SWORD_MAN:
	//		CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::ENGINEER);
	//		break;

	//	case CHARACTER_TYPE::ENGINEER:
	//		CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::DESTROYER);
	//		break;

	//	case CHARACTER_TYPE::DESTawwwwwwwROYER:
	//		CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::SWORD_MAN);
	//		break;
	//	}
	//	
	//}

	if (nullptr != m_pName)
		m_pName->Tick(fTimeDelta);

	if (nullptr != m_pWeapon)
		m_pWeapon->Tick(fTimeDelta);

	if(true == m_bMotionTrail)
		Tick_MotionTrail(fTimeDelta);

	// For Minimap
	// 미니맵을 위한 뷰 행렬을 렌더러에 던진다.
	_matrix CamMatrix = XMMatrixIdentity();

	_float4 vPlayerPos;
	XMStoreFloat4(&vPlayerPos, m_pTransformCom->Get_Position());
	_float vCamPosY = vPlayerPos.y + 150.f;

	_float3 vCamPos = _float3(vPlayerPos.x, vCamPosY, vPlayerPos.z);
	_float3 vLook = _float3(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);
	_float3 vUp = _float3(0.f, 0.f, 1.f);

	m_vCamPosition = _float4(vCamPos.x, vCamPos.y, vCamPos.z, 1.f);
	CamMatrix = XMMatrixLookAtLH(XMLoadFloat3(&vCamPos), XMLoadFloat3(&vLook), XMLoadFloat3(&vUp));
	XMStoreFloat4x4(&m_ViewMatrix, CamMatrix);
	m_pRendererCom->Set_MinimapView(m_ViewMatrix);

	if (nullptr != m_pMinimapIcon)
	{
		if (true == CUI_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
			m_pMinimapIcon->Set_Active(true);
		else
			m_pMinimapIcon->Set_Active(false);
	}

	if (LEVELID::LEVEL_TOOL != (LEVELID)g_eStartLevel)
	{
		if (nullptr != m_pCameraIcon)
			m_pCameraIcon->Tick(fTimeDelta);

		if (nullptr != m_pMinimapIcon)
			m_pMinimapIcon->Tick(fTimeDelta);
	}
	

#pragma region Deprecated.

	//if (m_bInfinite)
	//{
	//	m_fAccInfinite += fTimeDelta;
	//	if (m_fAccInfinite >= m_fInfiniteTime)
	//	{
	//		m_bInfinite = false;
	//		m_fAccInfinite = 0.f;

	//		Set_ActiveColliders(CCollider::DETECTION_TYPE::HEAD, true);
	//		Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, true);
	//	}
	//}
	//

	//for(_uint i = 0; i < SOCKET_END; ++i)
	//{
	//	if (nullptr == m_pTrails[i])
	//		continue;

	//	_matrix		WorldMatrix = m_Sockets[i]->Get_CombinedTransformation() * m_pModelCom->Get_PivotMatrix();

	//	WorldMatrix.r[CTransform::STATE_RIGHT] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_RIGHT]);
	//	WorldMatrix.r[CTransform::STATE_UP] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_UP]);
	//	WorldMatrix.r[CTransform::STATE_LOOK] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_LOOK]);

	//	m_pTrails[i]->Set_TransformMatrix(WorldMatrix * m_pTransformCom->Get_WorldMatrix());
	//	m_pTrails[i]->Tick(fTimeDelta);
	//}
#pragma endregion

	/* 보류된 레벨업 UI */
	if (true == m_bReserveUI&&
		false == CUI_Manager::GetInstance()->Is_QuestRewardsOn())
	{
		m_bReserveUI = false;
		CUI_Manager::GetInstance()->OnOff_LevelUp(true, m_tStat.iLevel);
	}
}





void CCharacter::Tick_MotionTrail(_float fTimeDelta)
{
	m_MotionTrailDesc.fAccMotionTrail += fTimeDelta;
	if (m_MotionTrailDesc.fAccMotionTrail >= m_MotionTrailDesc.fMotionTrailTime)
	{
		m_MotionTrailDesc.fAccMotionTrail = 0.f;

		MOTION_TRAIL_DESC TrailDesc;
		TrailDesc.WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		TrailDesc.fAlphaSpeed = m_MotionTrailDesc.fAlphaSpeed;
		TrailDesc.pModel = m_pModelCom;
		TrailDesc.TweenDesc = m_pModelCom->Get_TweenDesc();
		TrailDesc.vBloomPower = m_MotionTrailDesc.vBloomPower;
		TrailDesc.vRimColor = m_MotionTrailDesc.vRimColor;
		TrailDesc.fBlurPower = m_MotionTrailDesc.fBlurPower;


		TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::HEAD];
		if (nullptr != TrailDesc.pRenderModel)
		{
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
			{
				MSG_BOX("MotionTrail_Failed");
			}
		}
		

		TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::HAIR];
		if (nullptr != TrailDesc.pRenderModel)
		{
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
			{
				MSG_BOX("MotionTrail_Failed");
			}
		}
		

		TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::FACE];
		if (nullptr != TrailDesc.pRenderModel)
		{
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
			{
				MSG_BOX("MotionTrail_Failed");
			}
		}

		TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::BODY];
		if (nullptr != TrailDesc.pRenderModel)
		{
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
			{
				MSG_BOX("MotionTrail_Failed");
			}
		}
	}
}

void CCharacter::Tick_Target(_float fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;


	if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
		return;
	}
		
	CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr != pTargetTransform)
	{
		Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
		if (vDir.Length() > 6.f)
		{
			Safe_Release(m_pTarget);
			m_pTarget = nullptr;
		}
	}
}



void CCharacter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;

	if(nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pName)
		m_pName->LateTick(fTimeDelta);

	if (nullptr != m_pWeapon)
		m_pWeapon->LateTick(fTimeDelta);


	if (LEVELID::LEVEL_TOOL != (LEVELID)g_eStartLevel)
	{
		if (nullptr != m_pCameraIcon)
			m_pCameraIcon->LateTick(fTimeDelta);

		if (nullptr != m_pMinimapIcon)
			m_pMinimapIcon->LateTick(fTimeDelta);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_MINIMAP, this);



#ifdef _DEBUG
	m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}

#endif
}

HRESULT CCharacter::Render()
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

	Matrix worldInvTranspose = m_pTransformCom->Get_WorldMatrixInverse();
	worldInvTranspose.Transpose();

	Matrix worldInvTransposeView = worldInvTranspose * GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShaderCom->Bind_Matrix("WorldInvTransposeView", &worldInvTransposeView)))
		return E_FAIL;

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
	if (m_bSuperArmor)
	{
		vRimColor = { 1.f, 1.f, 0.f, 1.f };
	}

	if (m_bInfinite)
	{
		vRimColor = { 0.f, 0.5f, 1.f, 1.f };
	}		

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	
	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CCharacter::Render_ShadowDepth()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;


	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 10)))
				return E_FAIL;
		}
	}


	return S_OK;
}


HRESULT CCharacter::Render_Cascade_Depth(const Matrix lightViewMatrix, const Matrix LightOrthoMatrix)
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &lightViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &LightOrthoMatrix)))
		return E_FAIL;


	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;



	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 12)))
				return E_FAIL;
		}
	}


	return S_OK;
}

//HRESULT CCharacter::Render_Minimap()
//{
//	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
//		return E_FAIL;
//
//	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pRendererCom->Get_MinimapProj())))
//		return E_FAIL;
//	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
//		return E_FAIL;
//
//	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
//	{
//		if (nullptr == m_pCharacterPartModels[i])
//			continue;
//
//		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();
//
//		for (_uint j = 0; j < iNumMeshes; ++j)
//		{
//			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j)
//				,aiTextureType_DIFFUSE, "g_DiffuseTexture")))
//				return E_FAIL;
//
//			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j, 9)))
//				return E_FAIL;
//		}
//	}
//
//	return S_OK;
//}

void CCharacter::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
	{
		if ((tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
			&& (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK))
		{
			On_Damaged(tInfo);
		}
	}

	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS )
		&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		wstring strSoundKey = L"";
		switch (m_eCharacterType)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			strSoundKey = L"Hit_PC_Combo_Slash_Flesh_" + to_wstring(GI->RandomInt(1, 4)) + L".mp3";
			GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
			break;

		case CHARACTER_TYPE::ENGINEER:
			strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
			GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
			break;

		case CHARACTER_TYPE::DESTROYER:
			strSoundKey = L"Hit_PC_Combo_Smack_Flesh_" + to_wstring(GI->RandomInt(1, 4)) + L".mp3";
			GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
			break;

		}
	}
}

void CCharacter::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
	if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CURLINGGAME_PROP)
		{
			if (m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_ENTER
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_IDLE
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_WALK
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_RUN
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_THROW
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_LARGE_FINISH
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_ENTER
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_IDLE
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_WALK
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_RUN
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_THROW
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::NEUTRAL_PICK_SMALL_FINISH
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::TAG_IN
				|| m_pStateCom->Get_CurrState() == CCharacter::STATE::TAG_OUT)
			{
				if (nullptr != m_pEffectTargetDecal)
				{
					m_pEffectTargetDecal->Set_Dead(true);
					Safe_Release(m_pEffectTargetDecal);
					m_pEffectTargetDecal = nullptr;
				}
				return;
			}
			Decide_Target(tInfo);
		}
	}
}

void CCharacter::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCharacter::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);

	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
	
}

void CCharacter::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CCharacter::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
 	m_pRigidBodyCom->Set_Use_Gravity(true);
}





void CCharacter::Create_MotionTrail(const MOTION_TRAIL_DESC& MotionTrailDesc)
{
	MOTION_TRAIL_DESC TrailDesc;
	TrailDesc.WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	TrailDesc.fAlphaSpeed = MotionTrailDesc.fAlphaSpeed;
	TrailDesc.pModel = m_pModelCom;
	TrailDesc.TweenDesc = m_pModelCom->Get_TweenDesc();
	TrailDesc.vBloomPower = MotionTrailDesc.vBloomPower;
	TrailDesc.vRimColor = MotionTrailDesc.vRimColor;
	TrailDesc.fBlurPower = MotionTrailDesc.fBlurPower;


	TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::HEAD];
	if (nullptr != TrailDesc.pRenderModel)
	{
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
		{
			MSG_BOX("MotionTrail_Failed");
		}
	}


	TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::HAIR];
	if (nullptr != TrailDesc.pRenderModel)
	{
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
		{
			MSG_BOX("MotionTrail_Failed");
		}
	}


	TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::FACE];
	if (nullptr != TrailDesc.pRenderModel)
	{
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
		{
			MSG_BOX("MotionTrail_Failed");
		}
	}

	TrailDesc.pRenderModel = m_pCharacterPartModels[PART_TYPE::BODY];
	if (nullptr != TrailDesc.pRenderModel)
	{
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_MotionTrail", &TrailDesc)))
		{
			MSG_BOX("MotionTrail_Failed");
		}
	}
}

void CCharacter::Generate_MotionTrail(const MOTION_TRAIL_DESC& MotionTrailDesc)
{
	m_bMotionTrail = true;
	m_MotionTrailDesc = MotionTrailDesc;
}

void CCharacter::Stop_MotionTrail()
{
	m_bMotionTrail = false;
}



void CCharacter::Decide_Target(COLLISION_INFO tInfo)
{
	/* 이미 세팅된 스톤이라면 리턴*/
	if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == tInfo.pOther->Get_ObjectType())
	{
		CCurlingGame_Stone* pStone = dynamic_cast<CCurlingGame_Stone*>(tInfo.pOther);
		if (nullptr != pStone && pStone->Is_Putted())
			return;
	}

	if (nullptr == m_pTarget)
	{
		Set_Target(tInfo.pOther);

		if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == tInfo.pOther->Get_ObjectType()) // 컬링 게임 오브젝트들은 데칼 사용 X
			return;

		if (nullptr != m_pEffectTargetDecal)
		{
			m_pEffectTargetDecal->Set_Dead(true);
			Safe_Release(m_pEffectTargetDecal);
			m_pEffectTargetDecal = nullptr;
		}

		CEffect_Manager::GetInstance()->Generate_Decal_To_Position(L"Decal_Target", XMMatrixIdentity(), Vec3(0.f, 0.1f, 0.f), Vec3(2.f, 2.f, 2.f), Vec3(0.f, 0.f, 0.f), m_pTarget, &m_pEffectTargetDecal, false);
		Safe_AddRef(m_pEffectTargetDecal);
	}
	else
	{
		if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
		{
			Safe_Release(m_pTarget);
			m_pTarget = nullptr;

			if (nullptr != m_pEffectTargetDecal)
			{
				m_pEffectTargetDecal->Set_Dead(true);
				Safe_Release(m_pEffectTargetDecal);
				m_pEffectTargetDecal = nullptr;
			}
			return;
		}

		CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
		CTransform* pNewTargetTransform = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform");

		if (nullptr != pTargetTransform && nullptr != pNewTargetTransform)
		{
			Vec3 vTargetDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
			Vec3 vNewTargetDir = pNewTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
			if (vNewTargetDir.Length() < vTargetDir.Length())
			{
				Set_Target(tInfo.pOther);

				if (nullptr != m_pEffectTargetDecal)
				{
					m_pEffectTargetDecal->Set_Dead(true);
					Safe_Release(m_pEffectTargetDecal);
					m_pEffectTargetDecal = nullptr;
				}

				if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == tInfo.pOther->Get_ObjectType()
					|| OBJ_TYPE::OBJ_ANIMAL == tInfo.pOther->Get_ObjectType()) // 컬링 게임
					return;

				CEffect_Manager::GetInstance()->Generate_Decal(L"Decal_Target", pNewTargetTransform->Get_WorldMatrix(), Vec3(0.f, 1.f, 0.f), Vec3(2.f, 2.f, 2.f), Vec3(0.f, 0.f, 0.f), m_pTarget, &m_pEffectTargetDecal, false);
				Safe_AddRef(m_pEffectTargetDecal);
			}
		}
	}
}

void CCharacter::LevelUp()
{
	m_tStat.iExp = max(0, m_tStat.iExp - m_tStat.iMaxExp);
	m_tStat.iMaxExp += (m_tStat.iMaxExp) * 0.5f;	
	m_tStat.iLevel++;

	m_tStat.iMaxHp += m_tStat.iLevel * 100;
	m_tStat.iHp = m_tStat.iMaxHp;
	m_tStat.iAtt += m_tStat.iLevel * 100;
	if (false == CUI_Manager::GetInstance()->Is_QuestRewardsOn())
	{
		m_bReserveUI = false;
		CUI_Manager::GetInstance()->OnOff_LevelUp(true, m_tStat.iLevel);
	}
	else
	{
		// 퀘스트 보상창이 켜져있다면 보류해둔다.
		m_bReserveUI = true;
	}

	if (m_tStat.iExp >= m_tStat.iMaxExp)	
		LevelUp();
}



void CCharacter::On_Damaged(const COLLISION_INFO& tInfo)
{
	if (true == m_bInfinite)
		return;

	if (m_pStateCom->Get_CurrState() == CCharacter::STATE::DEAD || m_pStateCom->Get_CurrState() == CCharacter::STATE::REVIVE)
		return;
	

	CMonster* pMonster = nullptr;
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
	{
		CMonsterProjectile* pProjectile = dynamic_cast<CMonsterProjectile*>(tInfo.pOther);
		if (nullptr == pProjectile)
		{
			MSG_BOX("CMonsterProjectile Cast Failed.");
			return;
		}
		pMonster = pProjectile->Get_Owner();
	}
	else
	{
		pMonster = dynamic_cast<CMonster*>(tInfo.pOther);
	}
	
	if (nullptr == pMonster)
		return;


	CTransform* pOtherTransform = pMonster->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr != pOtherTransform)
		m_pTransformCom->LookAt_ForLandObject(pOtherTransform->Get_Position());


	_float fElemental = CGame_Manager::GetInstance()->Calculate_Elemental(tInfo.pOtherCollider->Get_ElementalType(), m_eDamagedElemental);
	_int iDamage = max(0, pMonster->Get_Stat().iAtk - (m_tStat.iDef * 0.2f) * fElemental);

	

	if (m_eDamagedElemental != ELEMENTAL_TYPE::BASIC)
	{
		m_eDamagedElemental = ELEMENTAL_TYPE::BASIC;
	}
	else
	{
		m_eDamagedElemental = tInfo.pOtherCollider->Get_ElementalType();
	}

	if (m_pStateCom->Get_CurrState() == CCharacter::BATTLE_GUARD)
	{
		iDamage *= 0.2f;
		m_pRigidBodyCom->Add_Velocity(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 2.f, true);
	}
		


	CUIDamage_Manager::GetInstance()->Create_PlayerDamageNumber(m_pTransformCom, iDamage);
	if (true == Decrease_HP(iDamage))
		return;
	

	if (true == m_bSuperArmor)	
		return;

	if (m_pStateCom->Get_CurrState() == CCharacter::BATTLE_GUARD)
		return;

	

	if (CCollider::ATTACK_TYPE::AIR_BORNE == tInfo.pOtherCollider->Get_AttackType())
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)), 7.f, true);
		m_pStateCom->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
	}

	else if (CCollider::ATTACK_TYPE::STUN == tInfo.pOtherCollider->Get_AttackType())
	{
		m_pStateCom->Change_State(CCharacter::ABNORMALITY_STUN);
	}

	else if (CCollider::ATTACK_TYPE::BLOW == tInfo.pOtherCollider->Get_AttackType())
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		m_pRigidBodyCom->Add_Velocity(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 7.f, true);
		m_pStateCom->Change_State(CCharacter::DAMAGED_IMPACT);
	}

	else if (CCollider::ATTACK_TYPE::BOUND == tInfo.pOtherCollider->Get_AttackType())
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)), 2.f, true);
		m_pStateCom->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
	}

	else if (CCollider::ATTACK_TYPE::STRONG == tInfo.pOtherCollider->Get_AttackType())
	{
		m_pRigidBodyCom->Add_Velocity(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 2.f, true);
		m_pStateCom->Change_State(CCharacter::DAMAGED_STRONG);
	}

	else if (CCollider::ATTACK_TYPE::WEAK == tInfo.pOtherCollider->Get_AttackType())
	{
		m_pStateCom->Change_State(CCharacter::DAMAGED_WEAK);
	}

	Create_HitEffect(tInfo.pOtherCollider->Get_AttackType(), pMonster);
}

void CCharacter::Create_HitEffect(_int iType, CMonster* pMonster)
{
	_float fRandomXOffset = CUtils::Random_Float(-0.5f, 0.5f);
	_float fRandomYOffset = CUtils::Random_Float(-0.5f, 0.5f);

	CTransform* pMonsterTransform = pMonster->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pMonsterTransform)
		return;

	Vec4 vPlayerPos = pMonsterTransform->Get_Position();
	Vec4 vThisPos = m_pTransformCom->Get_Position();
	Vec4 vLook = XMVector4Normalize(vPlayerPos - vThisPos);

	_vector vLookPosition = m_pTransformCom->Get_Position();
	vLookPosition += vLook * 0.5f;

	_matrix ThisWorldMat = m_pTransformCom->Get_WorldMatrix();
	ThisWorldMat.r[CTransform::STATE_POSITION] = vLookPosition;



	CParticle* pParticle = nullptr;

	// 어택 타입에 따른 피격 이펙트 생성
	if (CCollider::ATTACK_TYPE::WEAK == iType) // 살짝
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(L"Particle_Player_Hit_Small", ThisWorldMat, _float3(fRandomXOffset, 1.f + fRandomYOffset, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pParticle);
	}

	else if (CCollider::ATTACK_TYPE::STRONG == iType || CCollider::ATTACK_TYPE::STUN == iType) // 세게
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(L"Particle_Player_Hit_Maiddle", ThisWorldMat, _float3(fRandomXOffset, 1.f + fRandomYOffset, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pParticle);
	}

	else if (CCollider::ATTACK_TYPE::BOUND == iType || CCollider::ATTACK_TYPE::AIR_BORNE == iType || // 더세게 또는 날아감
		CCollider::ATTACK_TYPE::BLOW == iType || CCollider::ATTACK_TYPE::IF_DEAD_BLOW == iType)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(L"Particle_Player_Hit_High", ThisWorldMat, _float3(fRandomXOffset, 1.f + fRandomYOffset, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pParticle);
	}

	if (nullptr == pParticle)
		return;

	// 기본 파티클
	CParticle* pCircleParticle = nullptr;

	CTransform* pMainParticleTransform = pParticle->Get_Component<CTransform>(L"Com_Transform");
	GET_INSTANCE(CParticle_Manager)->Generate_Particle(L"Particle_Player_Hit_Circles", pMainParticleTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pCircleParticle);
	if (nullptr == pCircleParticle)
		return;

	// 몬스터의 속성에 따른 색상 변경
	ELEMENTAL_TYPE eMonsterType = pMonster->Get_Stat().eElementType;
	_float3 fColor = _float3(1.f, 1.f, 1.f);

	switch (eMonsterType)
	{
	case FIRE: // _float3(1.f, 0.51f, 0.311f)
		fColor = _float3(CUtils::Random_Float(0.75f, 1.f), CUtils::Random_Float(0.36f, 0.66f), CUtils::Random_Float(0.161f, 0.461f));
		break;

	case WATER: // _float3(0.418f, 0.865f, 0.952f)
		fColor = _float3(CUtils::Random_Float(0.118f, 0.718f), CUtils::Random_Float(0.565f, 1.f), CUtils::Random_Float(0.652f, 1.f));
		break;

	case WOOD: // _float3(0.567f, 1.f, 0.461f)
		fColor = _float3(CUtils::Random_Float(0.417f, 0.717f), CUtils::Random_Float(0.75f, 1.f), CUtils::Random_Float(0.311f, 0.611f));
		break;
	}

	pParticle->Set_Color(fColor);
	pCircleParticle->Set_Color(_float3(min(fColor.x + 0.3f, 1.f), min(fColor.y + 0.3f, 1.f), min(fColor.z + 0.3f, 1.f)));
}

void CCharacter::Add_Exp(_int iExp)
{
	m_tStat.iExp += iExp;
	if (m_tStat.iExp >= m_tStat.iMaxExp)
	{
		LevelUp();
	}
}

_bool CCharacter::Decrease_HP(_int iDecrease)
{
	m_tStat.iHp = max(0, m_tStat.iHp - iDecrease);
	if (0 == m_tStat.iHp)
	{
		// 확인 필요
		CRiding_Manager::GetInstance()->Ride_ForCharacter(CRiding_Manager::VEHICLE_TYPE::UDADAK, false);

		m_pStateCom->Change_State(CCharacter::STATE::DEAD);
		return true;
	}
	return false;
}

CCharacter::STATE CCharacter::Get_CurrentState()
{
	if (nullptr == m_pStateCom)
		return CCharacter::STATE::STATE_END;

	return CCharacter::STATE(m_pStateCom->Get_CurrState());
}

Vec4 CCharacter::Get_CharacterPosition()
{
	return m_pTransformCom->Get_Position();
}

void CCharacter::Set_EnterLevelPosition(Vec4 vPosition, Vec3* pRotation)
{
	m_pStateCom->Change_State(CCharacter::STATE::NEUTRAL_DOOR_ENTER);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vPosition, 1.f));
	vPosition.z += 1.f;
	m_pTransformCom->LookAt_ForLandObject(XMVectorSetW(vPosition, 1.f));
	m_pControllerCom->Set_EnterLevel_Position(vPosition);

	if (nullptr != pRotation)
		m_pTransformCom->FixRotation((*pRotation).x, (*pRotation).y, (*pRotation).z);
}

void CCharacter::Set_InitialPosition(Vec4 vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vPosition, 1.f));
	m_pControllerCom->Set_EnterLevel_Position(XMVectorSetW(vPosition, 1.f));
}

void CCharacter::Set_Target(CGameObject* pTarget)
{
	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
	}

	m_pTarget = pTarget;
	Safe_AddRef(m_pTarget);
}

HRESULT CCharacter::Disappear_Weapon()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->Disappear_Weapon();

	return S_OK;
}

HRESULT CCharacter::Appear_Weapon()
{
	if (nullptr != m_pWeapon)
		m_pWeapon->Appear_Weapon();

	return S_OK;
}

void CCharacter::PickUp_Target()
{
	if (nullptr == m_pTarget)
		return;

	if (m_pTarget->Is_Dead() || m_pTarget->Is_ReserveDead())
		return;

	CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTargetTransform)
		return;
	
	Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();

	if (OBJ_TYPE::OBJ_ANIMAL == m_pTarget->Get_ObjectType())
	{
		CAnimals* pAnimal = dynamic_cast<CAnimals*>(m_pTarget);

		if (nullptr == pAnimal)
		{
			MSG_BOX("Animal Cast Failed.");
			return;
		}

		pAnimal->Set_Lift(true);
		m_pStateCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_ENTER);
		return;
	}

	if (OBJ_TYPE::OBJ_CURLINGGAME_PROP == m_pTarget->Get_ObjectType())
	{
		CCurlingGame_Prop* pProp = dynamic_cast<CCurlingGame_Prop*>(m_pTarget);

		if (nullptr == pProp)
		{
			MSG_BOX("Prop Cast Failed.");
			return;
		}

		m_pStateCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_ENTER);
		return;

	}
}

void CCharacter::PickDown_Target()
{
	if (nullptr == m_pTarget)
		return;

	if (CCharacter::STATE::NEUTRAL_PICK_SMALL_IDLE == m_pStateCom->Get_CurrState())
	{
		m_pStateCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_FINISH);
		return;
	}
	else if (CCharacter::STATE::NEUTRAL_PICK_LARGE_IDLE == m_pStateCom->Get_CurrState())
	{
		m_pStateCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_FINISH);
		return;
	}

}

void CCharacter::Look_For_Target(_bool bEnemy)
{
	if (nullptr == m_pTarget)
		return;

	if (true == bEnemy)
	{
		if (m_pTarget->Get_ObjectType() != OBJ_TYPE::OBJ_MONSTER && m_pTarget->Get_ObjectType() != OBJ_TYPE::OBJ_BOSS)
			return;
	}

	CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTargetTransform)
		return;

	Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
	if (m_eCharacterType != CHARACTER_TYPE::ENGINEER)
	{
		if (vDir.Length() <= 3.f)
		{
			m_pTransformCom->LookAt_ForLandObject(pTargetTransform->Get_Position());
		}
	}
	else
	{
		if (vDir.Length() <= 5.f)
		{
			m_pTransformCom->LookAt_ForLandObject(pTargetTransform->Get_Position());
		}
	}
	

	m_pTransformCom->LookAt_ForLandObject(XMVectorSetW(pTargetTransform->Get_Position(), 1.f));
}

HRESULT CCharacter::Enter_Character()
{
	if (nullptr != m_pControllerCom)
		m_pControllerCom->Set_Active(true);

	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
	}

	m_bControllCharacter = true;
	return S_OK;
}

HRESULT CCharacter::Exit_Character()
{
	if (nullptr != m_pControllerCom)
		m_pControllerCom->Set_Active(true);

	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
	}
	
	m_bControllCharacter = false;

	return S_OK;
}

HRESULT CCharacter::Tag_In(Vec4 vInitializePosition)
{
	m_pStateCom->Change_State(CCharacter::STATE::TAG_IN);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vInitializePosition, 1.f));
	m_pControllerCom->Set_Active(true);
	m_pControllerCom->Set_EnterLevel_Position(XMVectorSetW(vInitializePosition, 1.f));

	if(LEVEL_TOOL != GI->Get_CurrentLevel())
		CGame_Manager::GetInstance()->Get_Kuu()->Set_KuuTarget_Player();

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_TargetObj(this);
			if(!pFollowCam->Is_LockOn())
				pFollowCam->Set_LookAtObj(this);
		}
	}

	if (LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		CUI_Manager::GetInstance()->Ready_CharacterTypeForUI(Get_CharacterType());
		CUI_Manager::GetInstance()->Ready_ElementalTypeForUI(Get_ElementalType());
	}

	m_bControllCharacter = true;

	return S_OK;
}

HRESULT CCharacter::Tag_Out()
{
	m_pStateCom->Change_State(CCharacter::STATE::TAG_OUT);

	m_pControllerCom->Set_Active(false);
	if (nullptr != m_pTarget)
	{
		Safe_Release(m_pTarget);
		m_pTarget = nullptr;
	}

	if (nullptr != m_pEffectTargetDecal)
	{
		m_pEffectTargetDecal->Set_Dead(true);
		Safe_Release(m_pEffectTargetDecal);
		m_pEffectTargetDecal = nullptr;
	}
	m_bControllCharacter = false;

	return S_OK;
}





void CCharacter::Free()
{
	__super::Free();

	/*for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		Safe_Release(m_pCharacterPartModels[i]);*/

	Safe_Release(m_pName);

	Safe_Release(m_pCameraIcon);
	Safe_Release(m_pMinimapIcon);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTarget);
	Safe_Release(m_pEffectTargetDecal);
}
