#include "stdafx.h"
#include "GameInstance.h"
#include "DefenceInvasion_Portal.h"
#include "Level_Loading.h"
#include "Effect_Manager.h"
#include "Vfx.h"

#include "Game_Manager.h"
#include "Player.h"


CDefenceInvasion_Portal::CDefenceInvasion_Portal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"DefenceInvasion_Portal", OBJ_TYPE::OBJ_PORTAL)
{
}

CDefenceInvasion_Portal::CDefenceInvasion_Portal(const CDefenceInvasion_Portal& rhs)
	: CGameObject(rhs)
{
}


HRESULT CDefenceInvasion_Portal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDefenceInvasion_Portal::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	m_strPrototypeMonsterTags.push_back(L"Prorotype_GameObject_PumpkinCandle");
	m_strPrototypeMonsterTags.push_back(L"Prorotype_GameObject_Clown");
	m_strPrototypeMonsterTags.push_back(L"Prorotype_GameObject_Clown_Wizard");

	return S_OK;
}

void CDefenceInvasion_Portal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	auto iter = m_Monsters.begin();
	for (; iter != m_Monsters.end();)
	{
		if ((*iter)->Is_ReserveDead() || (*iter)->Is_Dead())
		{
			iter = m_Monsters.erase(iter);
			continue;
		}
		iter++;
	}
}

void CDefenceInvasion_Portal::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pModelCom->LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDefenceInvasion_Portal::Render()
{
	__super::Render();
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

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

void CDefenceInvasion_Portal::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CDefenceInvasion_Portal::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CDefenceInvasion_Portal::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CDefenceInvasion_Portal::Start_Invasion()
{
	for (auto& pMonster : m_Monsters)
		pMonster->Reserve_Dead(true);
	m_Monsters.clear();

	for (_int i = -4; i <= 4; ++i)
	{
		OBJECT_INIT_DESC InitDesc;
		InitDesc.vStartPosition = m_pTransformCom->Get_Position() + XMVectorSet(i + GI->RandomFloat(-1.f, 1.f), 0.f, i + GI->RandomFloat(-1.f, 1.f), 0.f);

		CGameObject* pMonster = nullptr;
		
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, m_strPrototypeMonsterTags[GI->RandomInt(SPAWN_MONSTER_TYPE::PUMPKIN_CANDLE, SPAWN_MONSTER_TYPE::CLOWN_WIZARD)], &InitDesc, &pMonster)))
		{
			MSG_BOX("AddMonster Failed. : CDefenceInvasion_Portal::Start_Invasion");
			return;
		}

		Safe_AddRef(pMonster);
		m_Monsters.push_back(pMonster);
	}
}

void CDefenceInvasion_Portal::Stop_Invasion()
{
	for (auto& pMonster : m_Monsters)
	{
		pMonster->Reserve_Dead(true);
		Safe_Release(pMonster);
	}

	m_Monsters.clear();
}

HRESULT CDefenceInvasion_Portal::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_InvasionPortal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}



CDefenceInvasion_Portal* CDefenceInvasion_Portal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefenceInvasion_Portal* pInstance = new CDefenceInvasion_Portal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDefenceInvasion_Portal");
		Safe_Release<CDefenceInvasion_Portal*>(pInstance);
	}

	return pInstance;
}

CGameObject* CDefenceInvasion_Portal::Clone(void* pArg)
{
	CDefenceInvasion_Portal* pInstance = new CDefenceInvasion_Portal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDefenceInvasion_Portal");
		Safe_Release<CDefenceInvasion_Portal*>(pInstance);
	}

	return pInstance;
}

void CDefenceInvasion_Portal::Free()
{
	__super::Free();
	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	for (auto& pMonster : m_Monsters)
		Safe_Release(pMonster);

	m_Monsters.clear();
}
