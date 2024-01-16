#include "stdafx.h"
#include "Spawner_Witch01.h"

#include "GameInstance.h"
#include "Monster.h"

CSpawner_Witch01::CSpawner_Witch01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CSpawner(pDevice, pContext, strObjectTag)
{
}

CSpawner_Witch01::CSpawner_Witch01(const CSpawner_Witch01& rhs)
	: CSpawner(rhs)
{
}

HRESULT CSpawner_Witch01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Witch01::Initialize(void* pArg)
{
	m_fSpawnTime = 30.f;
	m_fCurTime = 30.f;

	m_iMaxPumpkinCount = 2;
	m_iMaxClownCount = 2;
	m_iMaxWizardCount = 2;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CSpawner_Witch01::Tick(_float fTimeDelta)
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

		for (_int i = 0; i < m_vecPumpkin.size();)
		{
			if (m_vecPumpkin[i]->Is_Dead())
				m_vecPumpkin.erase(m_vecPumpkin.begin() + i);
			else
				++i;
		}

		for (_int i = 0; i < m_vecClown.size();)
		{
			if (m_vecClown[i]->Is_Dead())
				m_vecClown.erase(m_vecClown.begin() + i);
			else
				++i;
		}

		for (_int i = 0; i < m_vecWizard.size();)
		{
			if (m_vecWizard[i]->Is_Dead())
				m_vecWizard.erase(m_vecWizard.begin() + i);
			else
				++i;
		}
	}
}

void CSpawner_Witch01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CSpawner_Witch01::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpawner_Witch01::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Witch01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner_Witch01::Spawn_Monster()
{
	_uint iCurLevel = GI->Get_CurrentLevel();

	_float4 vSpawnerPos = {};
	XMStoreFloat4(&vSpawnerPos, m_pTransformCom->Get_Position());

	OBJECT_INIT_DESC tInfo = {};

	for (_int i = m_vecPumpkin.size(); i < m_iMaxPumpkinCount; ++i)
	{
		tInfo.vStartPosition.x = vSpawnerPos.x + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.y = vSpawnerPos.y;
		tInfo.vStartPosition.z = vSpawnerPos.z + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.w = 1.f;
	
		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_PumpkinCandle"), _uint(LAYER_MONSTER), &tInfo);
	
		if (pObj == nullptr)
			return E_FAIL;
	
		GI->Add_GameObject(iCurLevel, (_uint)LAYER_MONSTER, pObj);
		m_vecPumpkin.push_back(pObj);
	}

	for (_int i = m_vecClown.size(); i < m_iMaxClownCount; ++i)
	{
		tInfo.vStartPosition.x = vSpawnerPos.x + GI->RandomFloat(-3.f, 3.f);
		tInfo.vStartPosition.y = vSpawnerPos.y + 1.f;
		tInfo.vStartPosition.z = vSpawnerPos.z + GI->RandomFloat(-3.f, 3.f);
		tInfo.vStartPosition.w = 1.f;

		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_Clown"), _uint(LAYER_MONSTER), &tInfo);

		if (pObj == nullptr)
			return E_FAIL;

		GI->Add_GameObject(iCurLevel, (_uint)LAYER_MONSTER, pObj);
		m_vecClown.push_back(pObj);
	}

	for (_int i = m_vecWizard.size(); i < m_iMaxWizardCount; ++i)
	{
		tInfo.vStartPosition.x = vSpawnerPos.x + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.y = vSpawnerPos.y;
		tInfo.vStartPosition.z = vSpawnerPos.z + GI->RandomFloat(-5.f, 5.f);
		tInfo.vStartPosition.w = 1.f;
	
		CGameObject* pObj = GI->Clone_GameObject(TEXT("Prorotype_GameObject_Clown_Wizard"), _uint(LAYER_MONSTER), &tInfo);
	
		if (pObj == nullptr)
			return E_FAIL;
	
		GI->Add_GameObject(iCurLevel, (_uint)LAYER_MONSTER, pObj);
		m_vecWizard.push_back(pObj);
	}

	return S_OK;
}

CSpawner_Witch01* CSpawner_Witch01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSpawner_Witch01* pInstance = new CSpawner_Witch01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSpawner_Witch01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawner_Witch01::Clone(void* pArg)
{
	CSpawner_Witch01* pInstance = new CSpawner_Witch01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSpawner_Witch01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawner_Witch01::Free()
{
	__super::Free();

	m_vecPumpkin.clear();
	m_vecClown.clear();
	m_vecWizard.clear();
}
