#include "stdafx.h"
#include "GameInstance.h"
#include "Riding_Manager.h"

#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Character.h"

#include "Vehicle_Udadak.h"
#include "Vehicle_Flying_Biplane.h"
#include "Vehicle_Flying_EnemyBiplane.h" 
#include "Vehicle_Flying_EnemyBoto.h"

#include "Grandprix_Engineer.h"

IMPLEMENT_SINGLETON(CRiding_Manager)

CRiding_Manager::CRiding_Manager()
{

}

HRESULT CRiding_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CRiding_Manager::Tick(_float fTimeDelta)
{
}

void CRiding_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CRiding_Manager::Ready_Vehicle_GameObject(LEVELID eID)
{
	CVehicle::VEHICLE_DESC UdadakDesc = {};
	UdadakDesc.fSpeed = 10.f;

	CGameObject* pUdadak = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Udadak"), &UdadakDesc, &pUdadak)))
		return E_FAIL;
	if (nullptr == pUdadak)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Udadak*>(pUdadak))
		return E_FAIL;
	m_pUdadak = dynamic_cast<CVehicle_Udadak*>(pUdadak);
	Safe_AddRef(m_pUdadak);

	CGameObject* pBiplane = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Vehicle_Biplane"), &UdadakDesc, &pBiplane)))
		return E_FAIL;
	if (nullptr == pBiplane)
		return E_FAIL;
	if (nullptr == dynamic_cast<CVehicle_Flying_Biplane*>(pBiplane))
		return E_FAIL;
	m_pBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(pBiplane);
	Safe_AddRef(m_pBiplane);
	
	// 모든 캐릭터를 로드한 경우에만 엔지니어를 만들 수 있다.
	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
	{
		CGameObject* pEnemyplane = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBiplane"), &UdadakDesc, &pEnemyplane)))
			return E_FAIL;
		if (nullptr == pEnemyplane)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBiplane*>(pEnemyplane))
			return E_FAIL;
		m_pEnemyPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(pEnemyplane);
		Safe_AddRef(m_pEnemyPlane);

		CGameObject* pEnemyEngineer = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Grandprix_Engineer"), nullptr, &pEnemyEngineer)))
			return E_FAIL;
		if (nullptr == pEnemyEngineer)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Engineer*>(pEnemyEngineer))
			return E_FAIL;
		m_pEngineer = dynamic_cast<CGrandprix_Engineer*>(pEnemyEngineer);
		Safe_AddRef(m_pEngineer);
	

		m_Botos.reserve(9);
		m_Enemies.reserve(9);

		CVehicle::VEHICLE_DESC BotoDesc = {};
		BotoDesc.fSpeed = 12.f;

		CGameObject* pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto); 

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(0);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(1);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(2);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(3);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(4);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(0);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER , TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Using_TextureComponent(true);
		dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto)->Set_TextureIndex(1);

		CGameObject* pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		pEnemy = nullptr;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
			return E_FAIL;
		if (nullptr == pEnemy)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
			return E_FAIL;
		m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
		Safe_AddRef(pEnemy);

		Ready_Grandprix();
	}

	return S_OK;
}

HRESULT CRiding_Manager::Ready_Vehicle_GameObjectToLayer(LEVELID eID)
{
	if (nullptr == m_pUdadak)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pUdadak)))
		return E_FAIL;
	Safe_AddRef(m_pUdadak);

	if (eID == LEVEL_EVERMORE)
	{
		if (nullptr == m_pBiplane)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pBiplane)))
			return E_FAIL;
		Safe_AddRef(m_pBiplane);

		if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
		{
			if (nullptr == m_pEnemyPlane)
				return E_FAIL;
			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, m_pEnemyPlane)))
				return E_FAIL;
			Safe_AddRef(m_pEnemyPlane);

			if (nullptr == m_pEngineer)
				return E_FAIL;
			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pEngineer)))
				return E_FAIL;
			Safe_AddRef(m_pEngineer);
		
			for (auto& iter : m_Botos)
			{
				if (nullptr == iter)
					return E_FAIL;

				if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, iter)))
					return E_FAIL;
				Safe_AddRef(iter);
			}

			for (auto& iter : m_Enemies)
			{
				if (nullptr == iter)
					return E_FAIL;

				if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, iter)))
					return E_FAIL;
				Safe_AddRef(iter);
			}
		}
	}

	return S_OK;
}

HRESULT CRiding_Manager::Ride_ForCharacter(VEHICLE_TYPE eType, _bool bOnOff)
{
	CCharacter* pCharacter = CUI_Manager::GetInstance()->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	switch (eType)
	{
	case UDADAK:
		if (nullptr == m_pUdadak)
			return E_FAIL;
		if (true == bOnOff)
		{
			if (nullptr == m_pUdadak->Get_Rider())
			{
				m_pUdadak->Set_Aboard(true);
				m_pUdadak->Ride(pCharacter);
			}
		}
		else
		{
			if (pCharacter == m_pUdadak->Get_Rider())
			{
				m_pUdadak->Set_Aboard(false);
			}
		}
		break;

	case BIPLANE:
		if (LEVELID::LEVEL_EVERMORE == GI->Get_CurrentLevel())
		{
			if (nullptr == m_pBiplane)
				return E_FAIL;

			if (true == bOnOff)
			{
				if (nullptr == m_pBiplane->Get_Rider())
				{
					m_pBiplane->Set_Aboard(true);
					m_pBiplane->Ride(pCharacter);
					m_pBiplane->Set_Players(true);
				}
			}
			else
			{
				if (pCharacter == m_pBiplane->Get_Rider())
				{
					m_pBiplane->Set_Aboard(false);
				}
			}
		}
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CRiding_Manager::Ready_Grandprix()
{
	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
	{
		if (nullptr != m_pEnemyPlane)
		{
			m_pEnemyPlane->Set_Aboard(true);
			m_pEnemyPlane->Ride(m_pEngineer);
		}

		for (size_t i = 0; i < m_Botos.size(); ++i)
		{
			if (nullptr != m_Botos[i])
			{
				m_Botos[i]->Set_Aboard(true);
				
				if (i < m_Enemies.size() && nullptr != m_Enemies[i])
				{
					m_Enemies[i]->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION,
						Vec4(-15.f + 10.f * i, 1.f, 30.2f, 1.f));
					m_Botos[i]->Ride(m_Enemies[i]);
					//m_Botos[i]->Set_OriginPosition(m_Botos[i]->Get_Component<CTransform>(L"Com_Transform")->Get_Position());
					
					m_Botos[i]->Set_Routes();
				}
			}
		}
	}

	return S_OK;
}

HRESULT CRiding_Manager::Ready_Grandprix_EnemyInfo()
{
	CUIMinigame_Manager::GetInstance()->Set_HPOwner(m_pEnemyPlane, CUIMinigame_Manager::GRANDPRIX_ENEMY::ENGINEER);

	for (size_t i = 0; i < m_Botos.size(); ++i)
	{
		if (nullptr != m_Botos[i])
		{
			CUIMinigame_Manager::GetInstance()->Set_HPOwner(m_Botos[i], i + 1);
		}
	}

	return S_OK;
}

void CRiding_Manager::Tick_Grandprix(_float fTimeDelta)
{
}

void CRiding_Manager::LateTick_Grandprix(_float fTimeDelta)
{
}

void CRiding_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pUdadak);
	Safe_Release(m_pBiplane);

	for (auto& iter : m_Botos)
		Safe_Release(iter);
	m_Botos.clear();
	for (auto& iter : m_Enemies)
		Safe_Release(iter);
	m_Enemies.clear();
	Safe_Release(m_pEngineer);
	Safe_Release(m_pEnemyPlane);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}




