#include "stdafx.h"
#include "Spawner_Ice03.h"

#include "GameInstance.h"
#include "Monster.h"

CSpawner_Ice03::CSpawner_Ice03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CSpawner(pDevice, pContext, strObjectTag)
{
}

CSpawner_Ice03::CSpawner_Ice03(const CSpawner_Ice03& rhs)
	: CSpawner(rhs)
{
}

HRESULT CSpawner_Ice03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice03::Initialize(void* pArg)
{
	m_fSpawnTime = 30.f;

	m_iMaxThiefCount = 3;
	m_iMaxBearCount = 1;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f, 0.f, 0.f, 1.f });

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL)
	{
		if (FAILED(Spawn_Monster()))
			return E_FAIL;
	}

	return S_OK;
}

void CSpawner_Ice03::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL)
	{
		m_fCurTime += fTimeDelta;

		if (m_fCurTime >= m_fSpawnTime)
		{
			Spawn_Monster();

			m_fCurTime = m_fSpawnTime - m_fCurTime;
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
}

void CSpawner_Ice03::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CSpawner_Ice03::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpawner_Ice03::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice03::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Ice03::Spawn_Monster()
{
	_uint iCurLevel = GI->Get_CurrentLevel();

	_float4 vSpawnerPos = {};
	XMStoreFloat4(&vSpawnerPos, m_pTransformCom->Get_Position());

	OBJECT_INIT_DESC tInfo = {};

	for (_int i = m_vecThief.size(); i < m_iMaxThiefCount; ++i)
	{
		tInfo.vStartPosition.x = vSpawnerPos.x + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.y = vSpawnerPos.y;
		tInfo.vStartPosition.z = vSpawnerPos.z + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_Shadow_Thief"), _uint(LAYER_MONSTER), &tInfo);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurLevel, (_uint)LAYER_MONSTER, pObj);
		m_vecThief.push_back(pObj);
	}

	for (_int i = m_vecBear.size(); i < m_iMaxBearCount; ++i)
	{
		tInfo.vStartPosition.x = vSpawnerPos.x + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.y = vSpawnerPos.y;
		tInfo.vStartPosition.z = vSpawnerPos.z + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_IceBearMan"), _uint(LAYER_MONSTER), &tInfo);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurLevel, (_uint)LAYER_MONSTER, pObj);
		m_vecBear.push_back(pObj);
	}

	return S_OK;
}

CSpawner_Ice03* CSpawner_Ice03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSpawner_Ice03* pInstance = new CSpawner_Ice03(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSpawner_Ice03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawner_Ice03::Clone(void* pArg)
{
	CSpawner_Ice03* pInstance = new CSpawner_Ice03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSpawner_Ice03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawner_Ice03::Free()
{
	__super::Free();

	m_vecThief.clear();
	m_vecBear.clear();
}
