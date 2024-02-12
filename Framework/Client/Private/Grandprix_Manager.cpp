#include "stdafx.h"
#include "Grandprix_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "UIMinigame_Manager.h"

#include "Vehicle_Flying_EnemyBiplane.h" 
#include "Vehicle_Flying_EnemyBoto.h"
#include "Grandprix_ItemBox.h"
#include "Grandprix_Engineer.h"
#include "Grandprix_ItemBox.h"
#include "Grandprix_Goal.h"
#include "Riding_Manager.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

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

	fill(begin(m_bItem), end(m_bItem), false);
	fill(begin(m_fTimeAcc), end(m_fTimeAcc), 0.f);

	return S_OK;
}

void CGrandprix_Manager::Tick(_float fTimeDelta)
{
	// 그랑프리가 시작되었는지 확인한다. false면 return;
	if (KEY_TAP(KEY::P))
		Show_GoalObject();

	// 그랑프리가 정상적으로 끝났는지 확인한다.
	if (false == m_bReset && true == m_bGrandPrixEnd)
	{
		if (true == CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(false, 16.f, 0.1f);
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f, true);
			m_bReset = true;
		}
	}

	if (true == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP])
	{
		m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP] += fTimeDelta;

		// 10초가 지나면 복구
		if (10.f < m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP])
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP] = false;
			m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP] = 0.f;

			CRiding_Manager::GetInstance()->Set_Character_BiplaneSpeed(10.f);
		}
	}

	if (true == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP])
	{
		m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP] += fTimeDelta;

		// 10초가 지나면 사이즈 복구
		if (10.f < m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP])
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP] = false;
			m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP] = 0.f;

			for (auto& pVehicle : m_Botos)
			{
				if (nullptr != pVehicle &&
					(false == pVehicle->Is_ReserveDead() && false == pVehicle->Is_Dead()))
				{
					pVehicle->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(Vec3(1.f));

					for (auto& pCollider : pVehicle->Get_Collider(CCollider::DETECTION_TYPE::BODY))
						pCollider->Set_Radius(2.f);
				}
			}
			for (auto& pGhost : m_Enemies)
			{
				if (nullptr != pGhost &&
					(false == pGhost->Is_ReserveDead() && false == pGhost->Is_Dead()))
				{
					pGhost->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(Vec3(1.f));
				}
			}
		}
	}

	if (true == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB])
	{
		m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB] += fTimeDelta;

		if (10.f < m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB])
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB] = false;
			m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB] = 0.f;

			CUIMinigame_Manager::GetInstance()->OnOff_RaderIcons(true);
		}
	}

	if (true == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW])
	{
		m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW] += fTimeDelta;

		if (0.1f < m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW])
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW] = false;
			m_fTimeAcc[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW] = 0.f;
			// 5초가 지나면 복구 -> 알아서 복구됨 제어할 필요 없음.
			// Bool변수만 제어
		}
	}
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

	if (nullptr != m_pGoal)
	{
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_ETC, m_pGoal)))
			return E_FAIL;
		Safe_AddRef(m_pGoal);
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

	m_Items.reserve(10);
	CGameObject* pItem;

	for (_uint i = 0; i < 10; ++i)
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

		_float fX = GI->RandomFloat(-200.f, 185.f);
		_float fY = GI->RandomFloat(70.f, 130.f);
		_float fZ = GI->RandomFloat(-210.f, 200.f);

		pItem->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, Vec4(fX, fY, fZ, 1.f));
		dynamic_cast<CGrandprix_ItemBox*>(pItem)->Set_MinMaxPosition(Vec4(fX, fY, fZ, 1.f));
	}

	CGameObject* pGoal = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_ETC, TEXT("Prorotype_GameObject_Grandprix_Goal"), nullptr, &pGoal)))
		return E_FAIL;
	if (nullptr == pGoal)
		return E_FAIL;
	if (nullptr == dynamic_cast<CGrandprix_Goal*>(pGoal))
		return E_FAIL;
	m_pGoal = dynamic_cast<CGrandprix_Goal*>(pGoal);
	Safe_AddRef(m_pGoal);

	return S_OK;
}

void CGrandprix_Manager::Start_Grandprix()
{
//	CUIMinigame_Manager::GetInstance()->Intro_Grandprix();
}

void CGrandprix_Manager::Finish_Grandprix()
{
}

void CGrandprix_Manager::End_Grandprix()
{
	// 남아 있는 적이 있다면, Set_Dead 처리한다.
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

	if (nullptr != m_pGoal)
	{
		m_pGoal->Set_Dead(true);
		Safe_Release(m_pGoal);
	}

	m_bGrandPrixEnd = true;
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(true, 16.f, 0.1f);
	GI->Set_Slow(TIMER_TYPE::GAME_PLAY, 3.f, 0.2f, true);
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 0.2f, true);
}

void CGrandprix_Manager::Add_ItemBox(_uint iType)
{
	if (iType == CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_END)
		return;

	switch (iType)
	{
	case CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP:
		if (false == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP])
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP] = true; // 5초
			// 플레이어 탈 것의 스피드를 제어한다.
			CRiding_Manager::GetInstance()->Set_Character_BiplaneSpeed((CRiding_Manager::GetInstance()->Get_Character_BiplaneSpeed()) * 3.f);
			// UI 연결
			CUIMinigame_Manager::GetInstance()->On_GrandprixPopup(CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SPEEDUP);
		}
		break;

	case CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP:
		if (false == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP]) // 10초
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP] = true;
			// 적과 적의 탈 것의 사이즈를 키운다.
			for (auto& pVehicle : m_Botos)
			{
				if (nullptr != pVehicle && (false == pVehicle->Is_ReserveDead() && false == pVehicle->Is_Dead()))
				{
					pVehicle->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(Vec3(5.f));
					for (auto& pCollider : pVehicle->Get_Collider(CCollider::DETECTION_TYPE::BODY))
					{
						pCollider->Set_Radius(5.f);
					}
				}
			}
			for (auto& pGhost : m_Enemies)
			{
				if (nullptr != pGhost &&
					(false == pGhost->Is_ReserveDead() && false == pGhost->Is_Dead()))
				{
					pGhost->Get_Component<CTransform>(L"Com_Transform")->Set_Scale(Vec3(5.f));
				}
			}
			// UI 연결
			CUIMinigame_Manager::GetInstance()->On_GrandprixPopup(CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SIZEUP);
		}
		break;

	case CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB:
		if (false == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB]) // 10초간 Error
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB] = true;
			CUIMinigame_Manager::GetInstance()->OnOff_RaderIcons(false);
			// 플레이어 탈 것을 멀리 밀어낸다.
			// UI 연결
			CUIMinigame_Manager::GetInstance()->On_GrandprixPopup(CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_BOMB);
		}
		break;

	case CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW:
		if (false == m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW]) // 5초
		{
			m_bItem[CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW] = true;
			// 시간이 느리게 가도록 세팅한다.
			GI->Set_Slow(TIMER_TYPE::GAME_PLAY, 5.f, 0.3f, true);
			// UI 연결
			CUIMinigame_Manager::GetInstance()->On_GrandprixPopup(CGrandprix_ItemBox::ITEMBOX_TYPE::ITEMBOX_SLOW);
		}
		break;
	}
}

void CGrandprix_Manager::Show_GoalObject()
{
	if (nullptr == m_pGoal)
		return;

	m_pGoal->Set_Active(true);
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
					m_Enemies[i]->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION,
						Vec4(0.1f, 60.f, 105.f, 1.f));
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
	Safe_Release(m_pGoal);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
