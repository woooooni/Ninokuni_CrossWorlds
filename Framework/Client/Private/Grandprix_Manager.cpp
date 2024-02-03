#include "stdafx.h"
#include "Grandprix_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "UIMinigame_Manager.h"

#include "Vehicle_Flying_EnemyBiplane.h" 
#include "Vehicle_Flying_EnemyBoto.h"
#include "Grandprix_ItemBox.h"
#include "Grandprix_Engineer.h"

IMPLEMENT_SINGLETON(CGrandprix_Manager)

CGrandprix_Manager::CGrandprix_Manager()
{

}

HRESULT CGrandprix_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CGrandprix_Manager::Tick(_float fTimeDelta)
{

}

void CGrandprix_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CGrandprix_Manager::Ready_Grandprix_GameObjectToLayer(LEVELID eID)
{
	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
	{
		if (nullptr != m_pEnemyPlane)
		{
			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, m_pEnemyPlane)))
				return E_FAIL;
			Safe_AddRef(m_pEnemyPlane);
		}

		if (nullptr != m_pEngineer)
		{
			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_CHARACTER, m_pEngineer)))
				return E_FAIL;
			Safe_AddRef(m_pEngineer);
		}

		for (auto& iter : m_Botos)
		{
			if (nullptr != iter)
			{
				if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_MONSTER, iter)))
					return E_FAIL;
				Safe_AddRef(iter);
			}
		}

		for (auto& iter : m_Enemies)
		{
			if (nullptr != iter)
			{
				if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, iter)))
					return E_FAIL;
				Safe_AddRef(iter);
			}
		}

		for (auto& iter : m_Items)
		{
			if (nullptr != iter)
			{
				if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, iter)))
					return E_FAIL;
				Safe_AddRef(iter);
			}
		}
	}

	return S_OK;
}

void CGrandprix_Manager::Ready_Grandprix_EnemyInfo()
{
	Ready_Riders(); // 여기서 Active true함.

	CUIMinigame_Manager::GetInstance()->Set_HPOwner(m_pEnemyPlane, CUIMinigame_Manager::GRANDPRIX_ENEMY::ENGINEER);

	for (size_t i = 0; i < m_Botos.size(); ++i)
	{
		if (nullptr != m_Botos[i])
		{
			CUIMinigame_Manager::GetInstance()->Set_HPOwner(m_Botos[i], i + 1);
		}
	}
}

HRESULT CGrandprix_Manager::Prepare_Grandprix()
{
	// 모든 캐릭터를 로드한 경우에만 엔지니어를 만들 수 있다.

	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
	{
		CVehicle::VEHICLE_DESC DefaultDesc = {};
		DefaultDesc.fSpeed = 10.f;

		CGameObject* pEnemyplane = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBiplane"), &DefaultDesc, &pEnemyplane)))
			return E_FAIL;
		if (nullptr == pEnemyplane)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBiplane*>(pEnemyplane))
			return E_FAIL;
		m_pEnemyPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(pEnemyplane);
		Safe_AddRef(m_pEnemyPlane);

		CGameObject* pEnemyEngineer = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Grandprix_Engineer"), nullptr, &pEnemyEngineer)))
			return E_FAIL;
		if (nullptr == pEnemyEngineer)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_Engineer*>(pEnemyEngineer))
			return E_FAIL;
		m_pEngineer = dynamic_cast<CGrandprix_Engineer*>(pEnemyEngineer);
		Safe_AddRef(m_pEngineer);


		m_Botos.reserve(8);
		m_Enemies.reserve(8);

		CVehicle::VEHICLE_DESC BotoDesc = {};
		BotoDesc.fSpeed = 12.f;

		CGameObject* pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
			return E_FAIL;
		if (nullptr == pEnemyBoto)
			return E_FAIL;
		if (nullptr == dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto))
			return E_FAIL;
		m_Botos.push_back(dynamic_cast<CVehicle_Flying_EnemyBoto*>(pEnemyBoto));
		Safe_AddRef(pEnemyBoto);

		pEnemyBoto = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_MONSTER, TEXT("Prototype_GameObject_Vehicle_EnemyBoto"), &BotoDesc, &pEnemyBoto)))
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

		for (_uint i = 0; i < 8; ++i)
		{
			pEnemy = nullptr;
			if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Enemy_Ghost2"), nullptr, &pEnemy)))
				return E_FAIL;
			if (nullptr == pEnemy)
				return E_FAIL;
			if (nullptr == dynamic_cast<CGrandprix_Enemy*>(pEnemy))
				return E_FAIL;
			m_Enemies.push_back(dynamic_cast<CGrandprix_Enemy*>(pEnemy));
			Safe_AddRef(pEnemy);
		}
	}

	m_Items.reserve(8);
	CGameObject* pItem;

	for (_uint i = 0; i < 8; ++i)
	{
		pItem = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_ETC, TEXT("Prototype_GameObject_Grandprix_ItemBox"), nullptr, &pItem)))
			return E_FAIL;
		if (nullptr == pItem)
			return E_FAIL;
		if (nullptr == dynamic_cast<CGrandprix_ItemBox*>(pItem))
			return E_FAIL;
		m_Items.push_back(dynamic_cast<CGrandprix_ItemBox*>(pItem));
		Safe_AddRef(pItem);

		_float fX = GI->RandomFloat(-200.f, 175.f);
		_float fY = GI->RandomFloat(70.f, 120.f);
		_float fZ = GI->RandomFloat(-200.f, 200.f);

		pItem->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, Vec4(fX, fY, fZ, 1.f));
		dynamic_cast<CGrandprix_ItemBox*>(pItem)->Set_MinMaxPosition(Vec4(fX, fY, fZ, 1.f));
	}

	return S_OK;
}

void CGrandprix_Manager::Start_Grandprix()
{

}

void CGrandprix_Manager::Finish_Grandprix()
{
}

void CGrandprix_Manager::End_Grandprix()
{
	// 남아 있는 적이 있다면, Set_Dead 처리한다.

	// Engineer
//	if (false == m_pEnemyPlane->Is_Dead() || false == m_pEnemyPlane->Is_ReserveDead())
	if (nullptr != m_pEnemyPlane)
	{
		m_pEnemyPlane->Set_Dead(true);
		Safe_Release(m_pEnemyPlane);
	}
	if (nullptr != m_pEngineer)
	{
		m_pEngineer->Set_Dead(true);
		Safe_Release(m_pEngineer);
	}

	// Enemy
	for (auto& iter : m_Botos)
	{
		if (nullptr != iter)
		{
			iter->Set_Dead(true);
			Safe_Release(iter);
		}
	}
	m_Botos.clear();

	for (auto& iter : m_Enemies)
	{
		if (nullptr != iter)
		{
			iter->Set_Dead(true);
			Safe_Release(iter);
		}
	}
	m_Enemies.clear();

	for (auto& iter : m_Items)
	{
		if (nullptr != iter)
		{
			iter->Set_Dead(true);
			Safe_Release(iter);
		}
	}
	m_Items.clear();
}


void CGrandprix_Manager::Ready_Riders()
{
	if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
	{
		// 적, 아이템을 Active한다.
		if (nullptr != m_pEngineer)
			m_pEngineer->Set_Active(true);
		for (auto& iter : m_Enemies)
		{
			if (nullptr != iter)
			{
				iter->Set_Active(true);
			}
		}

		if (nullptr != m_pEnemyPlane)
		{
			m_pEnemyPlane->Set_Aboard(true);
			m_pEnemyPlane->Ride(m_pEngineer);
			m_pEnemyPlane->Set_Routes();
		}

		for (size_t i = 0; i < m_Botos.size(); ++i)
		{
			if (nullptr != m_Botos[i])
			{
				m_Botos[i]->Set_Aboard(true);

				if (i < m_Enemies.size() && nullptr != m_Enemies[i])
				{
//					m_Enemies[i]->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION,
//						Vec4(-15.f + 10.f * i, 1.f, 30.2f, 1.f));
					m_Botos[i]->Ride(m_Enemies[i]);
					//m_Botos[i]->Set_OriginPosition(m_Botos[i]->Get_Component<CTransform>(L"Com_Transform")->Get_Position());

					m_Botos[i]->Set_Routes();
				}
			}
		}
	}

	for (auto& iter : m_Items)
	{
		if (nullptr != iter)
		{
			iter->Set_Active(true);
		}
	}
}

void CGrandprix_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_Botos)
		Safe_Release(iter);
	m_Botos.clear();
	for (auto& iter : m_Enemies)
		Safe_Release(iter);
	m_Enemies.clear();
	Safe_Release(m_pEngineer);
	Safe_Release(m_pEnemyPlane);
	for (auto& iter : m_Items)
		Safe_Release(iter);
	m_Items.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
