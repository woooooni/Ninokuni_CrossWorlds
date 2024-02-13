#include "stdafx.h"
#include "UIMinigame_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "Riding_Manager.h"
#include "Grandprix_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

#include "UI_Manager.h"

#include "UI_Minigame_Basic.h"
#include "UI_Minigame_TowerSelect.h"
#include "UI_Minigame_Timer.h"

#include "UI_Minigame_EnemyInfo.h"
#include "UI_Minigame_ClassSkill.h"
#include "UI_Minigame_EnemyHP.h"
#include "UI_Minigame_GaugeBar.h"
#include "UI_Minigame_WorldHP.h"
#include "UI_Minigame_Aim.h"
#include "UI_Grandprix_IntroBackground.h"
#include "UI_Grandprix_IntroIcons.h"
#include "UI_Grandprix_RaderFrame.h"
#include "UI_Grandprix_Rader.h"
#include "UI_Grandprix_RaderIcon.h"
#include "UI_Grandprix_Vignette.h"
#include "UI_Grandprix_Popup.h"
#include "UI_Grandprix_Target.h"

#include "CurlingGame_Group.h"

#include "UI_Fade.h"

#include "Camera_Manager.h"
#include "Camera.h"
#include "Camera_Follow.h"

#include "UI_Minigame_PlayerInfo.h"
#include "UI_Grandprix_PlaneHP.h"

IMPLEMENT_SINGLETON(CUIMinigame_Manager)

CUIMinigame_Manager::CUIMinigame_Manager()
{

}

void CUIMinigame_Manager::Set_HPOwner(CGameObject* pOwner, _uint eEnemyID)
{
	if (GRANDPRIX_ENEMY::ENEMY_END == eEnemyID)
		return;
	
	if (m_EnemyHP[eEnemyID] == nullptr)
		return;

	if (nullptr != m_EnemyHP[eEnemyID]->Get_Owner())
		return;

	m_EnemyHP[eEnemyID]->Set_Owner(pOwner);
}

void CUIMinigame_Manager::Set_TargetUI(CVehicle_Flying* pTarget)
{
	if (false == Is_BiplaneFlying())
		return;

	if (nullptr == m_pTarget || nullptr == pTarget)
		return;
	
	m_pTarget->Set_Target(pTarget);
}

void CUIMinigame_Manager::Set_Flyable(_bool bFlyable)
{
	m_bFlying = bFlyable;
	CCamera_Follow* pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pFollowCamera)
	{
		pFollowCamera->Set_MinMaxLimitY(0.2f, 2.9f);
	}
}

CUI_Minigame_Curling_Base* CUIMinigame_Manager::Get_MiniGame_Curling_Ui(const _uint& iObjTag)
{
	if ((_uint)MG_CL_UI_TYPE::TYPEEND <= iObjTag)
		return nullptr;

	for (auto& pUi : m_CurlingGameUIs)
	{
		if (nullptr == pUi)
			continue;

		if (g_wstr_MG_Curling_Ui_ObjTags[iObjTag] == pUi->Get_ObjectTag())
			return pUi;
	}

	return nullptr;
}

HRESULT CUIMinigame_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

//	fill(begin(m_bIntro), end(m_bIntro), false);
	
	return S_OK;
}

void CUIMinigame_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_Prototypes(LEVELID eID)
{
	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		Ready_MinigameUI_Evermore();
		break;

	case LEVELID::LEVEL_KINGDOMHALL:
		break;

	case LEVELID::LEVEL_ICELAND:
		Ready_MinigameUI_IceLand();
		break;

	case LEVELID::LEVEL_WITCHFOREST:
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_GameObject(LEVELID eID)
{
	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		Ready_TowerDence();
		Ready_Granprix();
		break;

	case LEVELID::LEVEL_KINGDOMHALL:
		break;

	case LEVELID::LEVEL_ICELAND:
		Ready_Curling();
		break;

	case LEVELID::LEVEL_WITCHFOREST:
		break;

	default:
		break;
	}
	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_ToLayer(LEVELID eID)
{
	if (LEVELID::LEVEL_EVERMORE == eID)
	{
		if (nullptr == m_pMenu)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMenu)))
			return E_FAIL;
		Safe_AddRef(m_pMenu);

		for (auto& iter : m_TowerSelect)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		if (nullptr == m_pTimer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pTimer)))
			return E_FAIL;
		Safe_AddRef(m_pTimer);

		if (nullptr == m_pStartBtn)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pStartBtn)))
			return E_FAIL;
		Safe_AddRef(m_pStartBtn);

		if (nullptr == m_pGold)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pGold)))
			return E_FAIL;
		Safe_AddRef(m_pGold);

		if (nullptr == m_pCloud)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCloud)))
			return E_FAIL;
		Safe_AddRef(m_pCloud);

		for (auto& iter : m_EnemyHP)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_Skill)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_Counts)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_GaugeBack)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		if (nullptr == m_pGaugeBar)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pGaugeBar)))
			return E_FAIL;
		Safe_AddRef(m_pGaugeBar);

		if (nullptr == m_pSpace)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pSpace)))
			return E_FAIL;
		Safe_AddRef(m_pSpace);

		if (nullptr == m_pBiplaneIcon)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBiplaneIcon)))
			return E_FAIL;
		Safe_AddRef(m_pBiplaneIcon);

		if (nullptr == m_pPlayerStat)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pPlayerStat)))
			return E_FAIL;
		Safe_AddRef(m_pPlayerStat);

		if (nullptr == m_pIntroBackground)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pIntroBackground)))
			return E_FAIL;
		Safe_AddRef(m_pIntroBackground);

		for (auto& iter : m_IntroIcons)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_RaderFrame)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		if (nullptr == m_pRader)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pRader)))
			return E_FAIL;
		Safe_AddRef(m_pRader);

		for (auto& iter : m_Vignette)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_Popup)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		if (nullptr == m_pTarget)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pTarget)))
			return E_FAIL;
		Safe_AddRef(m_pTarget);

	}
	else if (LEVELID::LEVEL_ICELAND == eID)
	{
		for (auto& iter : m_CurlingGameUIs)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}
	}

	return S_OK;
}

void CUIMinigame_Manager::Tick_Minigame(LEVELID eID, _float fTimeDelta)
{
	// OnOff를 위한 임시

	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		Tick_Grandprix(fTimeDelta);
		break;

	case LEVELID::LEVEL_ICELAND:
		Tick_Curling(fTimeDelta);
		break;

	default:
		break;
	}
}

void CUIMinigame_Manager::LateTick_Minigame(LEVELID eID, _float fTimeDelta)
{
	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		LateTick_Grandprix(fTimeDelta);
		break;

	case LEVELID::LEVEL_ICELAND:
		break;

	default:
		break;
	}
}

void CUIMinigame_Manager::OnOff_TowerDefence_Select(_bool bOnOff)
{
	if (true == bOnOff)
	{
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		if (nullptr != m_pMenu)
			m_pMenu->Set_Active(true);

		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(true);
		}
		 
		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(true);

		if (nullptr != m_pStartBtn)
			m_pStartBtn->Set_Active(true);

		if (nullptr != m_pGold)
			m_pGold->Set_Active(true);
	}
	else
	{
		if (nullptr != m_pMenu)
			m_pMenu->Set_Active(false);

		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(false);
		}

		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(false);

		if (nullptr != m_pStartBtn)
			m_pStartBtn->Set_Active(false);

		if (nullptr != m_pGold)
			m_pGold->Set_Active(false);

		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}
}

void CUIMinigame_Manager::OnOff_Grandprix(_bool bOnOff)
{
	if (true == bOnOff)
	{
		m_pPlayerStat->Set_Active(true);

		m_bCountStart = false;
		m_bGrandprixEnd = false;

		m_bFlying = true; // 경기가 끝나면 끈다

		OnOff_GrandprixGauge(false);

		if (nullptr != m_pCloud)
			m_pCloud->Set_Active(true);

		for (auto& pHP : m_EnemyHP)
		{
			if (nullptr != pHP)
				pHP->Set_Active(true);
		}

		for (auto& pSkill : m_Skill)
		{
			if (nullptr != pSkill)
				pSkill->Set_Active(true);
		}

		for (auto& pFrame : m_RaderFrame)
		{
			if (nullptr != pFrame)
				pFrame->Set_Active(true);
		}
		m_pRader->Set_Active(true);
	}
	else
	{
		m_pPlayerStat->Set_Active(false);

		m_bGrandprixEnd = true;
		m_iCountIndex = 0;

		if (nullptr != m_pCloud)
			m_pCloud->Set_Active(false);

		for (auto& pHP : m_EnemyHP)
		{
			if (nullptr != pHP)
				pHP->Set_Active(false);
		}

		for (auto& pSkill : m_Skill)
		{
			if (nullptr != pSkill)
				pSkill->Set_Active(false);
		}

		for (auto& pFrame : m_RaderFrame)
		{
			if (nullptr != pFrame)
				pFrame->Set_Active(false);
		}
		m_pRader->Set_Active(false);

		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}
}

void CUIMinigame_Manager::Intro_Grandprix()
{
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);
	m_bIntroStarted = true;

	m_fIntroAcc = 0.f;
	m_bIntroFinished = false;

	m_pIntroBackground->Set_Active(true);

	CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	pCharacter->Get_CharacterTransformCom()->Set_Position(Vec4(0.f, 1.f, 0.f, 1.f));
	pCharacter->Get_CharacterTransformCom()->LookAt_ForLandObject(Vec4(0.f, 0.f, 1.f, 1.f));
	pCharacter->Get_ControllerCom()->Set_EnterLevel_Position(Vec4(0.f, 1.f, 0.f, 1.f));
}

void CUIMinigame_Manager::Start_Grandprix()
{
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f, true);

	if (true == CUI_Manager::GetInstance()->Is_DefaultSettingOn())
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

	m_bCountStart = true;
	m_bGrandprixEnd = false;

	CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
	CGrandprix_Manager::GetInstance()->Ready_Grandprix_EnemyInfo(); // 라이더 태우는 작업도 같이 수행함.

	// 플레이어 비행기 태우기
	if (CHARACTER_TYPE::SWORD_MAN != CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CharacterType())
		return;

	CStateMachine* m_pStateCom = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine");
	if (nullptr == m_pStateCom)
		return;

	m_pStateCom->Change_State(CCharacter::FLYING_STAND);
	CRiding_Manager::GetInstance()->Ride_ForCharacter(CRiding_Manager::BIPLANE, true);
}

void CUIMinigame_Manager::End_Grandprix()
{
	OnOff_GrandprixGauge(false);

	m_bFlying = false;

	m_bIntroStarted = false;
	m_bGrandprixEnd = true;
	m_iCountIndex = 5;

	CGrandprix_Manager::GetInstance()->End_Grandprix();

	if (CHARACTER_TYPE::SWORD_MAN != CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CharacterType())
		return;
	CStateMachine* m_pStateCom = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine");
	if (nullptr == m_pStateCom)
		return;
	m_pStateCom->Change_State(CCharacter::NEUTRAL_IDLE);
	CRiding_Manager::GetInstance()->Ride_ForCharacter(CRiding_Manager::BIPLANE, false);

	// 퀘스트 완료 포지션으로 이동.
	CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
	CTransform* pCharacterTransform = pCharacter->Get_CharacterTransformCom();
	pCharacterTransform->Set_Position(
		Vec4(-78.6f, -7.4f, -45.8f, 1.f));
	pCharacter->Get_ControllerCom()->Set_EnterLevel_Position(Vec4(-78.6f, -7.4f, -45.8f, 1.f));

	CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(pCharacter);
	CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(pCharacter);
	dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW))->Finish_LockOn(pCharacter);
	dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW))->Reset_MinMaxLimitY();

	Vec3 vScale = pCharacterTransform->Get_Scale();
	pCharacterTransform->Set_Right(XMVectorSet(1.f, 0.f, 0.f, 0.f));
	pCharacterTransform->Set_Up(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	pCharacterTransform->Set_Look(XMVectorSet(0.f, 0.f, 1.f, 0.f));

	CGameObject* pEngineerNpc = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Engineer_Dummy");
	if (nullptr != pEngineerNpc)
	{
		CTransform* pNpcTransform = pEngineerNpc->Get_Component_Transform();
		if (nullptr != pNpcTransform)
		{
			pCharacterTransform->LookAt_ForLandObject(pNpcTransform->Get_Position());
		}
	}
}

void CUIMinigame_Manager::Use_GrandprixSkill(SKILL_TYPE eType)
{
	if (SKILL_TYPE::FLYING_MUCKCLOUD > eType && SKILL_TYPE::SKILL_END == eType)
		return;

	_uint iIndex = 0;
	if (eType == SKILL_TYPE::FLYING_MUCKCLOUD)
		iIndex = 0;
	else if (eType == SKILL_TYPE::FLYING_RUSH)
		iIndex = 1;
	else if (eType == SKILL_TYPE::FLYING_GUIDED_MISSILE)
		iIndex = 2;
	else
		iIndex = 3;

	m_Skill[iIndex]->Set_Clicked(true);
}

void CUIMinigame_Manager::OnOff_RushVignette(_bool bOnOff)
{
	if (nullptr == m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_RUSH])
		return;

	if (true == bOnOff)
	{
		m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_RUSH]->Set_Active(true);
	}
	else
	{
		m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_RUSH]->Set_Active(false);
	}
}

void CUIMinigame_Manager::On_DamagedVignette()
{
	if (nullptr == m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_DAMAGED])
		return;

	if (true == m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_DAMAGED]->Get_Active())
		return;

	m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_DAMAGED]->Set_Active(true);
}

void CUIMinigame_Manager::OnOff_RaderIcons(_bool bOnOff)
{
	// 에러상태에 따른 UI 상태 세팅
	if (true == bOnOff) // 레이더 아이콘을 켠다 -> Error 끔
	{
		m_bError = false;
	}
	else // Default
	{
		m_bError = true;
	}
}

void CUIMinigame_Manager::On_GrandprixPopup(_uint iIndex)
{
	if (0 == m_Popup.size())
		return;

	for (auto& iter : m_Popup)
	{
		if (nullptr != iter)
			iter->Set_TextureIndex(iIndex);
	}
}

void CUIMinigame_Manager::OnOff_CurlingUI(_bool bOnOff)
{
	if (true == bOnOff)
	{
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		for (auto& iter : m_CurlingGameUIs)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
	}
	else
	{
		for (auto& iter : m_CurlingGameUIs)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}
}

void CUIMinigame_Manager::OnOff_GrandprixGauge(_bool bOnOff)
{
	if (true == bOnOff)
	{
		for (auto& iter : m_GaugeBack)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}

		if (nullptr != m_pGaugeBar)
			m_pGaugeBar->Set_Active(true);

		if (nullptr != m_pSpace)
			m_pSpace->Set_Active(true);

		if (nullptr != m_pBiplaneIcon)
			m_pBiplaneIcon->Set_Active(true);
	}
	else
	{
		for (auto& iter : m_GaugeBack)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		if (nullptr != m_pGaugeBar)
			m_pGaugeBar->Set_Active(false);

		if (nullptr != m_pSpace)
			m_pSpace->Set_Active(false);

		if (nullptr != m_pBiplaneIcon)
			m_pBiplaneIcon->Set_Active(false);
	}
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_Evermore()
{
	// 타워 디펜스용 UI
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Menu_Background"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_MENU), LAYER_UI, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Cannon"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_CANNON), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Crystal"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_CRYSTAL), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Flame"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_FLAME), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Shadow"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_SHADOW), LAYER_UI, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Timer"),
		CUI_Minigame_Timer::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_StartButton"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_START), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Gold"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_GOLD), LAYER_UI, true)))
		return E_FAIL;


	// 그랑프리용 UI
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_CloudFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_FRAME), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"),
		CUI_Minigame_EnemyInfo::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_First"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_FIRST), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Second"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_SECOND), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Third"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_THIRD), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkill"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_SPECIAL), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkillFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_CLASSFRAME), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkillFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_SPECIALFRAME), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBar"),
		CUI_Minigame_EnemyHP::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Ready"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_READY), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Three"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_THREE), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Two"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_TWO), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_One"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_ONE), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Start"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_START), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_End"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_END), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeGlow"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_GAUGEGLOW), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeBack"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_GAUGEBACK), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeBar"),
		CUI_Minigame_GaugeBar::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpaceIcon"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_SPACE), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_BiplaneIcon"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_BIPLANE), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBackground"),
		CUI_Minigame_PlayerInfo::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Intro_Background"),
		CUI_Grandprix_IntroBackground::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Swordman"),
		CUI_Grandprix_IntroIcons::Create(m_pDevice, m_pContext, CUI_Grandprix_IntroIcons::GRANDPRIX_INTRO::SWORDMAN), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Engineer"),
		CUI_Grandprix_IntroIcons::Create(m_pDevice, m_pContext, CUI_Grandprix_IntroIcons::GRANDPRIX_INTRO::ENGINEER), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Splitter"),
		CUI_Grandprix_IntroIcons::Create(m_pDevice, m_pContext, CUI_Grandprix_IntroIcons::GRANDPRIX_INTRO::SPLITTER), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Vs"),
		CUI_Grandprix_IntroIcons::Create(m_pDevice, m_pContext, CUI_Grandprix_IntroIcons::GRANDPRIX_INTRO::VS_ICON), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_Frame"),
		CUI_Grandprix_RaderFrame::Create(m_pDevice, m_pContext, CUI_Grandprix_RaderFrame::UI_RADER::RADER_FRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_FrameCircle"),
		CUI_Grandprix_RaderFrame::Create(m_pDevice, m_pContext, CUI_Grandprix_RaderFrame::UI_RADER::RADER_CIRCLE), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_FrameBackground"),
		CUI_Grandprix_RaderFrame::Create(m_pDevice, m_pContext, CUI_Grandprix_RaderFrame::UI_RADER::RADER_BACKGROUND), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader"),
		CUI_Grandprix_Rader::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Vignette_Rush"),
		CUI_Grandprix_Vignette::Create(m_pDevice, m_pContext, CUI_Grandprix_Vignette::UI_GRANDPRIX_VIGNETTE::VIGNETTE_RUSH), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Vignette_Damaged"),
		CUI_Grandprix_Vignette::Create(m_pDevice, m_pContext, CUI_Grandprix_Vignette::UI_GRANDPRIX_VIGNETTE::VIGNETTE_DAMAGED), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Error"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_ERROR), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Popup_Background"),
		CUI_Grandprix_Popup::Create(m_pDevice, m_pContext, CUI_Grandprix_Popup::UI_GRANDPRIX_POPUP::POPUP_BACKGROUND), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Popup_Main"),
		CUI_Grandprix_Popup::Create(m_pDevice, m_pContext, CUI_Grandprix_Popup::UI_GRANDPRIX_POPUP::POPUP_MAIN), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_TargetUI"),
		CUI_Grandprix_Target::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	// 매니저 내에서는 프로토타입만 생성함
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Enemy_WorldHP"),
		CUI_Minigame_WorldHP::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Enemy_Aim"),
		CUI_Minigame_Aim::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBar"),
		CUI_Grandprix_PlaneHP::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_RaderIcon"),
		CUI_Grandprix_RaderIcon::Create(m_pDevice, m_pContext), LAYER_UI, true)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_IceLand()
{
	/* Gauge */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::GUAGE],
		CUI_Minigame_Curling_Gauge::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::GUAGE]), LAYER_UI)))
		return E_FAIL;

	/* Info_Npc */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::INFO_NPC],
		CUI_Minigame_Curling_Info::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::INFO_NPC], false), LAYER_UI)))
		return E_FAIL;
	
	/* Info_Player */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::INFO_PLAYER],
		CUI_Minigame_Curling_Info::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::INFO_PLAYER], true), LAYER_UI)))
		return E_FAIL;

	/* Stones_Npc */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::STONES_NPC],
		CUI_Minigame_Curling_Stones::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::STONES_NPC], false), LAYER_UI)))
		return E_FAIL;

	/* Stones_Player */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::STONES_PLAYER],
		CUI_Minigame_Curling_Stones::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::STONES_PLAYER], true), LAYER_UI)))
		return E_FAIL;

	/* Title */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::TITLE],
		CUI_Minigame_Curling_Title::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::TITLE]), LAYER_UI)))
		return E_FAIL;

	/* Stones_Npc */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::SCORE_NPC],
		CUI_Minigame_Curling_Score::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::SCORE_NPC], false), LAYER_UI)))
		return E_FAIL;

	/* Stones_Player */
	if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::SCORE_PLAYER],
		CUI_Minigame_Curling_Score::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::SCORE_PLAYER], true), LAYER_UI)))
		return E_FAIL;

	///* TurnArrow */
	//if (FAILED(GI->Add_Prototype(g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::SCORE_PLAYER],
	//	CUI_Minigame_Curling_TurnArrow::Create(m_pDevice, m_pContext, g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::TURNARROW]), LAYER_UI)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_TowerDence()
{
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 281.f;
	UIDesc.fCY = 900.f;
	UIDesc.fX = UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	CGameObject* pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Menu_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMenu = dynamic_cast<CUI_Minigame_Basic*>(pBackground);
	if (nullptr == m_pMenu)
		return E_FAIL;
	Safe_AddRef(m_pMenu);

	m_TowerSelect.reserve(4);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	_float fOffsetY = 180.f;

	UIDesc.fCX = 176.f;
	UIDesc.fCY = 163.f;
	UIDesc.fX = 140.5f;
	UIDesc.fY = 50.f + fOffsetY;

	CGameObject* pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Cannon"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 2.f;
	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Crystal"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 3.f;
	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Flame"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 4.f;
	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Shadow"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.7f;
	UIDesc.fCY = 120.f * 0.7f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = UIDesc.fCY * 0.6f;

	CGameObject* pTimer = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Timer"), &UIDesc, &pTimer)))
		return E_FAIL;
	m_pTimer = dynamic_cast<CUI_Minigame_Timer*>(pTimer);
	if (nullptr == m_pTimer)
		return E_FAIL;
	Safe_AddRef(m_pTimer);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.7f;
	UIDesc.fCY = 128.f * 0.7f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 20.f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f - 20.f;

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_StartButton"), &UIDesc, &pButton)))
		return E_FAIL;
	m_pStartBtn = dynamic_cast<CUI_Minigame_Basic*>(pButton);
	if (nullptr == m_pStartBtn)
		return E_FAIL;
	Safe_AddRef(m_pStartBtn);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 133.f;
	UIDesc.fCY = 35.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 20.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 20.f;

	CGameObject* pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Gold"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_pGold = dynamic_cast<CUI_Minigame_Basic*>(pSlot);
	if (nullptr == m_pGold)
		return E_FAIL;
	Safe_AddRef(m_pGold);

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_Granprix()
{
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pTarget = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_TargetUI"), &UIDesc, &pTarget)))
		return E_FAIL;
	m_pTarget = dynamic_cast<CUI_Grandprix_Target*>(pTarget);
	Safe_AddRef(m_pTarget);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_CloudFrame"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pCloud = dynamic_cast<CUI_Minigame_Basic*>(pFrame);
	if (nullptr == m_pCloud)
		return E_FAIL;
	Safe_AddRef(m_pCloud);


	m_EnemyHP.reserve(9);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	_float2 vOffset = _float2(35.f, 0.f);
	UIDesc.fCX = 178.f;
	UIDesc.fCY = 64.f;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = 180.f;
	CGameObject* pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(0);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(1);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(2);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(3);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(4);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(5);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(6);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(7);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(8);
	Safe_AddRef(pHP);


	m_Skill.reserve(4);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	vOffset = _float2(35.f, 35.f);

	UIDesc.fCX = 256.f * 0.3f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - vOffset.y;
	CGameObject* pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_First"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	UIDesc.fX += UIDesc.fCX + vOffset.x;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Second"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	UIDesc.fX += UIDesc.fCX + vOffset.x;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Third"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.45f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + vOffset.x);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - vOffset.y;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkill"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);


	m_Counts.reserve(6);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 512.f;
	UIDesc.fCY = 128.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Ready"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	UIDesc.fCX = 256.f;
	UIDesc.fCY = 256.f;
	pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Three"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	UIDesc.fCX = 256.f;
	UIDesc.fCY = 256.f;
	pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Two"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	UIDesc.fCX = 256.f;
	UIDesc.fCY = 256.f;
	pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_One"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	UIDesc.fCX = 512.f;
	UIDesc.fCY = 128.f;
	pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Start"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	UIDesc.fCX = 256.f;
	UIDesc.fCY = 128.f;
	pText = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_End"), &UIDesc, &pText)))
		return E_FAIL;
	m_Counts.push_back(dynamic_cast<CUI_Minigame_Basic*>(pText));
	if (nullptr == pText)
		return E_FAIL;
	Safe_AddRef(pText);

	m_GaugeBack.reserve(2);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pGauge = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeBack"), &UIDesc, &pGauge)))
		return E_FAIL;
	m_GaugeBack.push_back(dynamic_cast<CUI_Minigame_Basic*>(pGauge));
	if (nullptr == pGauge)
		return E_FAIL;
	Safe_AddRef(pGauge);

	pGauge = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeGlow"), &UIDesc, &pGauge)))
		return E_FAIL;
	m_GaugeBack.push_back(dynamic_cast<CUI_Minigame_Basic*>(pGauge));
	if (nullptr == pGauge)
		return E_FAIL;
	Safe_AddRef(pGauge);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pGauge = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_GaugeBar"), &UIDesc, &pGauge)))
		return E_FAIL;
	m_pGaugeBar = dynamic_cast<CUI_Minigame_GaugeBar*>(pGauge);
	if (nullptr == m_pGaugeBar)
		return E_FAIL;
	Safe_AddRef(m_pGaugeBar);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 136.f;
	UIDesc.fCY = 58.f;
	UIDesc.fX = 525.f;
	UIDesc.fY = g_iWinSizeY * 0.835f;
	CGameObject* pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpaceIcon"), &UIDesc, &pButton)))
		return E_FAIL;
	m_pSpace = dynamic_cast<CUI_Minigame_Basic*>(pButton);
	if (nullptr == m_pSpace)
		return E_FAIL;
	Safe_AddRef(m_pSpace);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 210.f * 0.4f;
	UIDesc.fCY = 98.f * 0.4f;
	UIDesc.fX = 600.f;
	UIDesc.fY = 720.f;
	CGameObject* pIcon = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_BiplaneIcon"), &UIDesc, &pIcon)))
		return E_FAIL;
	m_pBiplaneIcon = dynamic_cast<CUI_Minigame_Basic*>(pIcon);
	if (nullptr == m_pBiplaneIcon)
		return E_FAIL;
	Safe_AddRef(m_pBiplaneIcon);

	CGameObject* pBack = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBackground"), nullptr, &pBack)))
		return E_FAIL;
	m_pPlayerStat = dynamic_cast<CUI_Minigame_PlayerInfo*>(pBack);
	if (nullptr == m_pPlayerStat)
		return E_FAIL;
	Safe_AddRef(m_pPlayerStat);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = _float(g_iWinSizeX);
	UIDesc.fCY = _float(g_iWinSizeY);
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Intro_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pIntroBackground = dynamic_cast<CUI_Grandprix_IntroBackground*>(pBackground);
	if (nullptr == m_pIntroBackground)
		return E_FAIL;
	Safe_AddRef(m_pIntroBackground);


	m_IntroIcons.reserve(4);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = _float(g_iWinSizeX);
	UIDesc.fCY = _float(g_iWinSizeY);
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pIcon = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Swordman"), &UIDesc, &pIcon)))
		return E_FAIL;
	m_IntroIcons.push_back(dynamic_cast<CUI_Grandprix_IntroIcons*>(pIcon));
	Safe_AddRef(pIcon);

	pIcon = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Engineer"), &UIDesc, &pIcon)))
		return E_FAIL;
	m_IntroIcons.push_back(dynamic_cast<CUI_Grandprix_IntroIcons*>(pIcon));
	Safe_AddRef(pIcon);

	pIcon = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Splitter"), &UIDesc, &pIcon)))
		return E_FAIL;
	m_IntroIcons.push_back(dynamic_cast<CUI_Grandprix_IntroIcons*>(pIcon));
	Safe_AddRef(pIcon);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pIcon = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_IntroIcon_Vs"), &UIDesc, &pIcon)))
		return E_FAIL;
	m_IntroIcons.push_back(dynamic_cast<CUI_Grandprix_IntroIcons*>(pIcon));
	Safe_AddRef(pIcon);

	m_RaderFrame.reserve(3);
	_float fOffset = 35.f;

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 512.f * 0.45f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + fOffset);
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;
	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_FrameBackground"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_RaderFrame.push_back(dynamic_cast<CUI_Grandprix_RaderFrame*>(pBackground));
	Safe_AddRef(pBackground);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_FrameCircle"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_RaderFrame.push_back(dynamic_cast<CUI_Grandprix_RaderFrame*>(pBackground));
	Safe_AddRef(pBackground);

	UIDesc.fCX = 409.6f;
	UIDesc.fCY = 230.4f; // 512 * 0.45
	CGameObject* pRader = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader"), &UIDesc, &pRader)))
		return E_FAIL;
	m_pRader = dynamic_cast<CUI_Grandprix_Rader*>(pRader);
	Safe_AddRef(m_pRader);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 512.f * 0.45f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + fOffset);
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;
	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Rader_Frame"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_RaderFrame.push_back(dynamic_cast<CUI_Grandprix_RaderFrame*>(pBackground));
	Safe_AddRef(pBackground);

	m_Vignette.reserve(2);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Vignette_Rush"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_Vignette.push_back(dynamic_cast<CUI_Grandprix_Vignette*>(pBackground));
	Safe_AddRef(pBackground);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Vignette_Damaged"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_Vignette.push_back(dynamic_cast<CUI_Grandprix_Vignette*>(pBackground));
	Safe_AddRef(pBackground);

	m_Popup.reserve(2);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 512.f;
	UIDesc.fCY = 256.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 200.f;
	CGameObject* pPopup = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Popup_Background"), &UIDesc, &pPopup)))
		return E_FAIL;
	m_Popup.push_back(dynamic_cast<CUI_Grandprix_Popup*>(pPopup));
	Safe_AddRef(pPopup);

	pPopup = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_Popup_Main"), &UIDesc, &pPopup)))
		return E_FAIL;
	m_Popup.push_back(dynamic_cast<CUI_Grandprix_Popup*>(pPopup));
	Safe_AddRef(pPopup);

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_Curling()
{
	CGameObject* pClone = nullptr;
	CUI_Minigame_Curling_Base* pUi = nullptr;

	/* Guage */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::GUAGE], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* Info_Npc */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::INFO_NPC], nullptr, &pClone)))
			return E_FAIL;
	
		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;
	
		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);
	
		pClone = pUi = nullptr;
	}
	
	/* Info_Player */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::INFO_PLAYER], nullptr, &pClone)))
			return E_FAIL;
	
		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;
	
		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);
	
		pClone = pUi = nullptr;
	}

	/* Stones_Npc */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::STONES_NPC], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* Stones_Player */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::STONES_PLAYER], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* Title */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::TITLE], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* Score_Npc */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::SCORE_NPC], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* Score_Player */
	{
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::SCORE_PLAYER], nullptr, &pClone)))
			return E_FAIL;

		pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
		if (nullptr == pUi)
			return E_FAIL;

		m_CurlingGameUIs.push_back(pUi);
		Safe_AddRef(pUi);

		pClone = pUi = nullptr;
	}

	/* TurnArrow */
	//{
	//	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_UI, g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::TURNARROW], nullptr, &pClone)))
	//		return E_FAIL;
	//
	//	pUi = dynamic_cast<CUI_Minigame_Curling_Base*>(pClone);
	//	if (nullptr == pUi)
	//		return E_FAIL;
	//
	//	m_CurlingGameUIs.push_back(pUi);
	//	Safe_AddRef(pUi);
	//
	//	pClone = pUi = nullptr;
	//}

	return S_OK;
}

void CUIMinigame_Manager::Tick_Grandprix(_float fTimeDelta)
{
	if (KEY_TAP(KEY::N))
		Intro_Grandprix();
	
	if (KEY_TAP(KEY::M))
		End_Grandprix();

	if (true == m_bError)
	{
		// Error로 레이더를 볼 수 없을 때 계속 비네트를 켠다
		if (false == m_Vignette[CUI_Grandprix_Vignette::VIGNETTE_DAMAGED]->Get_Active())
			On_DamagedVignette();
	}

	if (0 < m_IntroIcons.size())
	{
		if (true == m_bIntroStarted && false == m_bIntroFinished)
		{
			if (true == m_pIntroBackground->Get_Active())
			{
				CUI_Manager::GetInstance()->OnOff_TextUI(false);
			}

			if (false == m_IntroIcons[CUI_Grandprix_IntroIcons::SWORDMAN]->Get_Active())
			{
				m_IntroIcons[CUI_Grandprix_IntroIcons::SWORDMAN]->Set_Active(true);
			}

			if (true == m_IntroIcons[CUI_Grandprix_IntroIcons::SWORDMAN]->Is_Finished())
			{
				if (false == m_IntroIcons[CUI_Grandprix_IntroIcons::ENGINEER]->Get_Active())
				{
					m_IntroIcons[CUI_Grandprix_IntroIcons::ENGINEER]->Set_Active(true);
				}

				if (true == m_IntroIcons[CUI_Grandprix_IntroIcons::ENGINEER]->Is_Finished())
				{
					if (false == m_IntroIcons[CUI_Grandprix_IntroIcons::SPLITTER]->Get_Active())
					{
						m_IntroIcons[CUI_Grandprix_IntroIcons::SPLITTER]->Set_Active(true);
					}

					if (true == m_IntroIcons[CUI_Grandprix_IntroIcons::SPLITTER]->Is_Finished())
					{
						if (false == m_IntroIcons[CUI_Grandprix_IntroIcons::VS_ICON]->Get_Active())
						{
							m_IntroIcons[CUI_Grandprix_IntroIcons::VS_ICON]->Set_Active(true);
						}
						else
						{
							m_fIntroAcc += fTimeDelta;

							if (3.f < m_fIntroAcc)
							{
								m_bIntroFinished = true;
								m_fIntroAcc = 0.f;

								for (auto& iter : m_IntroIcons)
									iter->Set_Active(false);

								m_pIntroBackground->Set_Active(false);

								Start_Grandprix();
							}
						}
					}
				}
			}
		}
	}

	if (true == m_bCountStart && false == m_bGrandprixEnd)
	{
		if (m_iCountIndex == 5)
		{
			OnOff_GrandprixGauge(true);
		}

		if (0 == m_Counts.size() || m_iCountIndex > m_Counts.size() - 2)
			return;

		if (false == m_Counts[m_iCountIndex]->Get_Active()) // 만약 객체가 활성화가 되어있지 않다면
			m_Counts[m_iCountIndex]->Set_Active(true); // 활성화를 시킨다.

		if (true == m_Counts[m_iCountIndex]->Get_Active() && // 활성화 되어있는 상황에서
			false == m_Counts[m_iCountIndex]->Is_Started()) // 아직 resize를 진행하지 않았다면
			m_Counts[m_iCountIndex]->Set_Start(true); // 시작하도록 세팅한다.

		if (true == m_Counts[m_iCountIndex]->Is_End()) // 사이즈 조정이 끝났다면
		{
			m_Counts[m_iCountIndex]->Set_Active(false); // 비활성화 시키고
			m_iCountIndex++; // 다음 인덱스로 넘어간다.
		}
	}
}

void CUIMinigame_Manager::LateTick_Grandprix(_float fTimeDelta)
{
	if (true == m_bGrandprixEnd)
	{
		if (5 != m_iCountIndex)
			return;

		if (false == m_Counts[m_iCountIndex]->Get_Active()) // 만약 객체가 활성화가 되어있지 않다면
			m_Counts[m_iCountIndex]->Set_Active(true); // 활성화를 시킨다.

		if (true == m_Counts[m_iCountIndex]->Get_Active() && // 활성화 되어있는 상황에서
			false == m_Counts[m_iCountIndex]->Is_Started()) // 아직 resize를 진행하지 않았다면
			m_Counts[m_iCountIndex]->Set_Start(true); // 시작하도록 세팅한다.

		if (true == m_Counts[m_iCountIndex]->Is_End()) // 사이즈 조정이 끝났다면
		{
			m_Counts[m_iCountIndex]->Set_Active(false); // 비활성화 시키고
			OnOff_Grandprix(false); // UI 세팅을 복구한다.
		}
	}
}

void CUIMinigame_Manager::Tick_Curling(_float fTimeDelta)
{
	
}

void CUIMinigame_Manager::Free()
{
	__super::Free();

	// 타워디펜스
	Safe_Release(m_pMenu);
	for (auto& pButton : m_TowerSelect)
		Safe_Release(pButton);
	m_TowerSelect.clear();
	Safe_Release(m_pTimer);
	Safe_Release(m_pStartBtn);
	Safe_Release(m_pGold);

	// 그랑프리
	Safe_Release(m_pPlayerStat);
	Safe_Release(m_pCloud);
	for (auto& pBackground : m_EnemyHP)
		Safe_Release(pBackground);
	m_EnemyHP.clear();
	for (auto& pSkill : m_Skill)
		Safe_Release(pSkill);
	m_Skill.clear();
	for (auto& pCount : m_Counts)
		Safe_Release(pCount);
	m_Counts.clear();
	Safe_Release(m_pGaugeBar);
	Safe_Release(m_pSpace);
	for (auto& pBack : m_GaugeBack)
		Safe_Release(pBack);
	m_GaugeBack.clear();
	Safe_Release(m_pBiplaneIcon);
	Safe_Release(m_pIntroBackground);
	for (auto& pIcon : m_IntroIcons)
		Safe_Release(pIcon);
	m_IntroIcons.clear();
	for (auto& pFrame : m_RaderFrame)
		Safe_Release(pFrame);
	m_RaderFrame.clear();
	Safe_Release(m_pRader);
	for (auto& pVignette : m_Vignette)
		Safe_Release(pVignette);
	m_Vignette.clear();
	for (auto& pPopup : m_Popup)
		Safe_Release(pPopup);
	m_Popup.clear();
	Safe_Release(m_pTarget);

	// 컬링
	for (auto& pUI : m_CurlingGameUIs)
		Safe_Release(pUI);
	m_CurlingGameUIs.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
