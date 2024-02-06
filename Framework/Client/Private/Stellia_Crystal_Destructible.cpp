#include "stdafx.h"
#include "Stellia_Crystal_Destructible.h"

#include "GameInstance.h"
#include "Animation.h"

#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"

#include "Particle_Manager.h"

#include "Stellia.h"

#include "Stellia_Crystal_Controller.h"
#include "Stellia_Crystal_Explosion.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CStellia_Crystal_Destructible::CStellia_Crystal_Destructible(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia_Crystal_Destructible::CStellia_Crystal_Destructible(const CStellia_Crystal_Destructible& rhs)
	: CMonster(rhs)
{
}

HRESULT CStellia_Crystal_Destructible::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Initialize(void* pArg)
{
	// 부유 Value
	m_fFlyDuration = 1.5f;
	m_fCrystalMaxY = 1.f;
	m_fCrystalMinY = 0.2f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		Vec4 vPos = *(Vec4*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransformCom->Set_Scale(Vec3(4.f, 4.f, 4.f));
		if (vPos.y > m_fCrystalMinY)
			m_bIsUp = false;
		else
			m_bIsUp = true;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(1);

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType, 2.5f);

	m_vBloomPower = _float3(1.f, 1.f, 1.f);

	// 받아온 y값에 따른 bool setting
	if (m_bIsUp)
		m_tFlyDesc.Start(m_fCrystalMinY, m_fCrystalMaxY, m_fFlyDuration, LERP_MODE::SMOOTHER_STEP);
	else
		m_tFlyDesc.Start(m_fCrystalMaxY, m_fCrystalMinY, m_fFlyDuration, LERP_MODE::SMOOTHER_STEP);
	
	// 턴 Value
	m_fDeSpeedDuration = 3.f;
	m_tTurnDesc.fCurValue = 2.f;
	m_tTurnDesc.Start(m_tTurnDesc.fCurValue, 0.f, m_fDeSpeedDuration, LERP_MODE::SMOOTHER_STEP);

	// Start Dissolve
	m_bStartDissolve  = true;
	m_fDissolveWeight = 10.f;

	return S_OK;
}

void CStellia_Crystal_Destructible::Tick(_float fTimeDelta)
{
	// Start Dissolve
	if (true == m_bStartDissolve)
	{
		switch (m_iSelfType)
		{
		case 0: // CRYSTAL_AURA
			m_vDissolveColor = _float4(0.212f, 0.620f, 1.f, 1.f);
			break;
		case 1: // CRYSTAL_SKY
			m_vDissolveColor = _float4(0.715f, 0.995f, 1.f, 1.f);
			break;
		case 2: // CRYSTAL_GOLD
			m_vDissolveColor = _float4(1.f, 0.96f, 0.466f, 1.f);
			break;
		}

		m_fDissolveWeight -= m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight < 0.f)
		{
			m_fDissolveWeight = 0.f;
			m_bStartDissolve  = false;
		}
	}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_GachaCommonCrystal03.ao|GachaCommonCrystalStart") && m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pModelCom->Set_Animation(TEXT("SKM_GachaCommonCrystal03.ao|GachaCommonCrystalLoop"));
	}

	if (false == m_bReserveDead && m_tStat.fHp <= 0.f)
	{
		if (m_iSelfType != m_iBingoType)
		{
			// Set_Dead 하기 전에 폭발 객체 생성 및 스텔리아 페일 카운트 추가
			GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Stellia_Crystal_Explosion"), this);

			// 굳이 늘리지 말자. 어차피 제한 시간 못 맞추면 늘어난다.
			// m_pStellia->Set_CrystalFailCount(1);
		}
		else if (m_iSelfType == m_iBingoType)
		{
			m_pStellia->Set_CrystalBingoCount(1);

			// 스텔리아에게 전달하여 다시 턴 시키기
			m_pStellia->Set_CrystalTurnData();
		}

		m_bReserveDead = true;
	}
	else if (m_bReserveDead) // Dissolve
	{
		if (!m_bDissolveEffect)
		{
			m_bDissolveEffect = true;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Monster_Dissolve"), m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pDissolveObject);
		}
		else if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
			return;
		}
		else
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
	}

	Fly_Crystal(fTimeDelta);

	// 어색해서 뺌. 빙글빙글 도는것과 로밍하는 것
	//if (m_bIsTurn)
	//	Turn_Crystal(fTimeDelta);
	//else
		//Crystal_Roaming(fTimeDelta);

	//////////////////////////

	if (nullptr != m_pHPBar)
		m_pHPBar->Tick(fTimeDelta);

	if (m_bIsRimUse) // RimLight
	{
		m_vRimLightColor = { 1.f, 0.f, 0.f, 1.f };
		Tick_RimLight(fTimeDelta);
	}
	else
	{
		m_vRimLightColor = { 0.f, 0.f, 0.f, 0.f };
	}

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);
}

void CStellia_Crystal_Destructible::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

	if (nullptr != m_pHPBar)
		m_pHPBar->LateTick(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);


#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // _DEBUG
}

HRESULT CStellia_Crystal_Destructible::Render()
{
	__super::Render();

	if (nullptr != m_pHPBar)
		m_pHPBar->Render();

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

	_float4 vRimColor = { 1.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	// Dissolve --------------------------------------------------------------------
	if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 51)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveDuration", &m_fDissolveDuration, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	// -----------------------------------------------------------------------------


	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (i != 2)
		{
			if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iSelfType)))
				return E_FAIL;
		}

		if (m_bStartDissolve || m_bReserveDead)
			iPassIndex = 2;
		else if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CStellia_Crystal_Destructible::Collision_Enter(const COLLISION_INFO& tInfo)
{
	/* 피격 */
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
		{
			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_ElementalType() == ELEMENTAL_TYPE::FIRE &&
				m_iSelfType == (_int)CStellia_Crystal_Controller::CRYSTAL_GOLD)
			{
				On_Damaged(tInfo);
			}

			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_ElementalType() == ELEMENTAL_TYPE::WOOD &&
				m_iSelfType == (_int)CStellia_Crystal_Controller::CRYSTAL_SKY)
			{
				On_Damaged(tInfo);
			}

			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_ElementalType() == ELEMENTAL_TYPE::WATER &&
				m_iSelfType == (_int)CStellia_Crystal_Controller::CRYSTAL_AURA)
			{
				On_Damaged(tInfo);
			}
		}
	}
}

void CStellia_Crystal_Destructible::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStellia_Crystal_Destructible::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CStellia_Crystal_Destructible::Set_CrystalData(STELLIA_CRYSTAL_DESC* pDesc)
{
	m_iSelfType = pDesc->iSelfType;
	m_iBingoType = pDesc->iBingoType;
	m_pStellia = dynamic_cast<CStellia*>(pDesc->pStellia);
}

void CStellia_Crystal_Destructible::Set_CrystalTurnData()
{
	m_tTurnDesc.fCurValue = 10.f;
	m_tTurnDesc.Start(m_tTurnDesc.fCurValue, 0.f, m_fDeSpeedDuration, LERP_MODE::SMOOTHER_STEP);
	m_bIsTurn = true;
}

void CStellia_Crystal_Destructible::Fly_Crystal(_float fTimeDelta)
{
	if (m_tFlyDesc.bActive)
	{
		m_fCrystalY = m_tFlyDesc.Update(fTimeDelta);
	}
	else
	{
		m_bIsUp = !m_bIsUp;
		if (m_bIsUp)
		{
			m_tFlyDesc.Start(m_fCrystalMinY, m_fCrystalMaxY, m_fFlyDuration, LERP_MODE::SMOOTHER_STEP);
		}
		else
		{
			m_tFlyDesc.Start(m_fCrystalMaxY, m_fCrystalMinY, m_fFlyDuration, LERP_MODE::SMOOTHER_STEP);
		}
	}

	Vec4 vCrystalPos = m_pTransformCom->Get_Position();
	vCrystalPos.y = m_fCrystalY;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCrystalPos);
}

void CStellia_Crystal_Destructible::Turn_Crystal(_float fTimeDelta)
{
	if (m_tTurnDesc.bActive)
		m_tTurnDesc.fCurValue = m_tTurnDesc.Update(fTimeDelta);
	else
	{
		m_bIsTurn = false;

		m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (FAILED(__super::Ready_RoamingPoint()))
			MSG_BOX("Fail Set : Crystal RoamingArea.");
	}

	m_pTransformCom->RevolutionRotation(m_pStellia->Get_OriginPos().xyz(), Vec3::Up, m_tTurnDesc.fCurValue * fTimeDelta);
}

HRESULT CStellia_Crystal_Destructible::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Stellia_Crystals"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stellia_Crystal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Ready_States()
{
	strKorName = TEXT("악몽 크리스탈");
	strSubName = TEXT("마녀의 숲");
	m_tStat.eElementType = ELEMENTAL_TYPE::FIRE;

	m_tStat.iLv = 15;
	m_tStat.fMaxHp = 5000;
	m_tStat.fHp = 5000;
	m_tStat.iAtk = 250;
	m_tStat.iDef = 0;

	return S_OK;
}

HRESULT CStellia_Crystal_Destructible::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 30.f, 50.f, 30.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

void CStellia_Crystal_Destructible::Crystal_Roaming(_float fTimeDelta)
{
	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_pTransformCom->Get_Position());
	XMStoreFloat4(&vDestPos, m_vecRoamingArea[m_iRoamingIndex]);

	m_pTransformCom->LookAt_ForLandObject(m_vecRoamingArea[m_iRoamingIndex]);
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), 2.f, fTimeDelta);

	if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
		vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
	{
		m_iRoamingIndex += 1;
		if (m_iRoamingIndex == m_vecRoamingArea.size())
			m_iRoamingIndex = 0;
	}
}

CStellia_Crystal_Destructible* CStellia_Crystal_Destructible::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia_Crystal_Destructible* pInstance = new CStellia_Crystal_Destructible(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CStellia_Crystal_Destructible");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStellia_Crystal_Destructible::Clone(void* pArg)
{
	CStellia_Crystal_Destructible* pInstance = new CStellia_Crystal_Destructible(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStellia_Crystal_Destructible");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia_Crystal_Destructible::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pHPBar);
}
