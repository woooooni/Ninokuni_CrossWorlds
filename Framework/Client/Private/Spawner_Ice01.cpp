#include "stdafx.h"
#include "Spawner_Ice01.h"

#include "GameInstance.h"
#include "Monster.h"

CSpawner_Ice01::CSpawner_Ice01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CSpawner(pDevice, pContext, strObjectTag)
{
}

CSpawner_Ice01::CSpawner_Ice01(const CSpawner_Ice01& rhs)
	: CSpawner(rhs)
{
}

HRESULT CSpawner_Ice01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice01::Initialize(void* pArg)
{
	m_fSpawnTime = 15.f;

	m_iMaxBaobamCount = 2;
	m_iMaxThiefCount = 3;
	m_iMaxBearCount = 1;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f, 0.f, 0.f, 1.f });

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Spawn_Monster()))
		return E_FAIL;


	return S_OK;
}

void CSpawner_Ice01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurTime += fTimeDelta;

	if (m_fCurTime >= m_fSpawnTime)
	{
		Spawn_Monster();

		m_fCurTime = m_fSpawnTime - m_fCurTime;
	}

	for (_int i = 0; i < m_vecBaobam.size();)
	{
		if (m_vecBaobam[i]->Is_Dead())
			m_vecBaobam.erase(m_vecBaobam.begin() + i);
		else
			++i;
	}

	for (_int i = 0; i < m_vecThief.size();)
	{
		if (m_vecThief[i]->Is_Dead())
			m_vecThief.erase(m_vecThief.begin() + i);
		else
			++i;
	}

	for (_int i = 0; i < m_vecBear.size();)
	{
		if (m_vecBear[i]->Is_Dead())
			m_vecBear.erase(m_vecBear.begin() + i);
		else
			++i;
	}
}

void CSpawner_Ice01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CSpawner_Ice01::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpawner_Ice01::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice01::Spawn_Monster()
{
	_float4 vSpawnerPos = {}; 
	XMStoreFloat4(&vSpawnerPos,  m_pTransformCom->Get_Position());

	_uint iCurr = GI->Get_CurrentLevel();

	for (_int i = m_vecBaobam.size(); i < m_iMaxBaobamCount; ++i)
	{
		_float4 vSpawnPos = {};
		vSpawnPos.x = vSpawnerPos.x + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.y = vSpawnerPos.y;
		vSpawnPos.z = vSpawnerPos.z + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_Baobam_Water"), _uint(LAYER_MONSTER), &vSpawnPos);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurr, (_uint)LAYER_MONSTER, pObj);
		m_vecBaobam.push_back(pObj);
	}

	for (_int i = m_vecThief.size(); i < m_iMaxThiefCount; ++i)
	{
		_float4 vSpawnPos = {};
		vSpawnPos.x = vSpawnerPos.x + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.y = vSpawnerPos.y;
		vSpawnPos.z = vSpawnerPos.z + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_Shadow_Thief"), _uint(LAYER_MONSTER), &vSpawnPos);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurr, (_uint)LAYER_MONSTER, pObj);
		m_vecThief.push_back(pObj);
	}

	for (_int i = m_vecBear.size(); i < m_iMaxBearCount; ++i)
	{
		_float4 vSpawnPos = {};
		vSpawnPos.x = vSpawnerPos.x + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.y = vSpawnerPos.y;
		vSpawnPos.z = vSpawnerPos.z + GI->RandomFloat(-2.5f, 2.5f);
		vSpawnPos.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_IceBearMan"), _uint(LAYER_MONSTER), &vSpawnPos);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurr, (_uint)LAYER_MONSTER, pObj);
		m_vecBear.push_back(pObj);
	}

	return S_OK;
}

CSpawner_Ice01* CSpawner_Ice01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSpawner_Ice01* pInstance = new CSpawner_Ice01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSpawner_Ice01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawner_Ice01::Clone(void* pArg)
{
	CSpawner_Ice01* pInstance = new CSpawner_Ice01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSpawner_Ice01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawner_Ice01::Free()
{
	__super::Free();

	m_vecBaobam.clear();
	m_vecBaobam.clear();
	m_vecBear.clear();
}
