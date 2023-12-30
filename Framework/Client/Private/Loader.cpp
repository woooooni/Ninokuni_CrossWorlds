#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "FileUtils.h"
#include "Sword.h"

#include "Dummy.h"
#include "Terrain.h"
#include "Cloth_Terrain.h"
#include "BackGround.h"
#include "MapHeaderGroup.h"
#include "DynamicGroup.h"
#include "SkyDome.h"
#include "SkyPlane.h"


#include <filesystem>
#include "Utils.h"
#include "UI_Manager.h"
#include "ImGui_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Weapon_Manager.h"
#include "Character_Manager.h"
#include "Game_Manager.h"

#include "UI_Logo_Background.h"
#include "UI_Flare.h"
#include "UI_CharacterDummy.h"

#include "Spawner_Ice01.h"
#include "Glanix.h"
#include "Glanix_IcePillar.h"
#include "Glanix_FireSpirit.h"
#include "Stellia.h"
#include "DMWitch.h"
#include "Baobam_Water.h"
#include "Shadow_Thief.h"
#include "IceBearMan.h"
#include "PumpkinCandle.h"
#include "Clown.h"
#include "Clown_Wizard.h"
#include "Baobam_Dark.h"

#include "HumanFAT01.h"
#include "HumanFL04.h"
#include "HumanFL05.h"
#include "HumanFL07.h"
#include "HumanFLCapitalMerchant.h"
#include "KingdomGuard.h"
#include "NpcWeapon_Halberd.h"
#include "HumanML12.h"
#include "HumanMM03.h"
#include "LuxerionHuman.h"
#include "HumanChild01.h"
#include "HumanChild02.h"
#include "SeekerCat.h"
#include "SeekerKing.h"
#include "SeekerObserver.h"
#include "Ruslan.h"
#include "HumanFS03.h"
#include "SwiftSolutionMaster.h"
#include "Zehra.h"
#include "Enbi.h"
#include "Edellian.h"
#include "Chloe.h"
#include "Aren.h"

#include "HumanChildHalloweenA.h"
#include "HumanChildHalloweenB.h"
#include "CaliaHuman.h"
#include "HumanMSCrossFieldMerchant.h"
#include "RunnerCat.h"
#include "ThiefCat.h"
#include "Ghost1.h"
#include "Ghost2.h"
#include "HumanFSPioneer.h"
#include "Ruby.h"
#include "TreeGrandfa.h"
#include "Verde.h"
#include "Gosling.h"

#include "FunyaSnowman.h"
#include "GiftFunyaSnowman.h"
#include "AquarisBella.h"

#include "Particle.h"
#include "Effect.h"
#include "Weapon_SwordTemp.h"

_bool CLoader::g_bFirstLoading = false;
CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY ThreadEntry(void* pArg)
{
	
	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel, const wstring& strFolderName)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;
	m_strFolderName = strFolderName;
	/* 새로운 스레드를 만들자. */
	/* 스레드 : 내 코드를 읽어주는 하나의 흐름? */
	/* 3 : 생성한 스레드가 호출해야하는 함수의 주소 */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;
	

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;

	case LEVEL_TEST:
		hr = Loading_For_Level_Test();
		break;

	case LEVEL_LOBBY:
		hr = Loading_For_Level_Lobby();
		break;

	case LEVEL_EVERMORE:
		hr = Loading_For_Level_Evermore();
		break;

	case LEVEL_TOOL:
		hr = Loading_For_Level_Tool();
		break;
	}

	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Background_Frames"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/LogoFrames/Logo_Texture_%d.jpg"), 274))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Flare"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/UI_Effect_Flare_Yellow.png")))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Background"),
		CUI_Logo_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Flare"),
		CUI_Flare::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;


	if (false == g_bFirstLoading)
	{
		if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;

		if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;
	}


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;
	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Lobby()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_NicknameFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_NickName_Frame.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Announce"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Announce_CharacterSelect.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Btn_GameStart"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Btn_HighGreen.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenuLeft"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Back_Deco_Left.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenRight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Back_Deco_Right.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_TitleText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_Text.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_BtnText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_Text_GameStart.png")))))
		return E_FAIL;

	// 클릭 전 이미지(캐릭터 선택창)
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Unclick_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Icon_Portrait_Style01_%d.png"), 5))))
		return E_FAIL;
	// 클릭 후 이미지(캐릭터 선택창)
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Click_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Icon_Portrait_Style02_%d.png"), 5))))
		return E_FAIL;

	// 닉네임 설정창
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Dice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Set_Nickname/UI_Set_Nickname_DiceIcon.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Set_Nickname/UI_Set_Nickname_Window.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Set_Nickname/UI_Set_Nickname_Btn.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Textbox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Set_Nickname/UI_SetNickname_TextBox.png")))))
		return E_FAIL;


	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	if (FAILED(CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_LOBBY)))
		return E_FAIL;

	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");

	if (false == g_bFirstLoading)
	{
		if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;

		if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;
	}
	

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;
	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Evermore()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

//	if (FAILED(CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_EVERMORE)))
//		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Engineer",
		CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Engineer")), LAYER_CHARACTER)))
		return E_FAIL;

	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");

	
	if (false == g_bFirstLoading)
	{
		if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;

		if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;
	}


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;
	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Test()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Background", CBackGround::Create(m_pDevice, m_pContext), LAYER_BACKGROUND)))
		return E_FAIL;

	/*if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Cloth_Terrain", CCloth_Terrain::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_TERRAIN)))
		return E_FAIL;*/


	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp= 100;
	statDesc.fMaxMp = 100;

	/* Monster */
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix", CGlanix::Create(m_pDevice, m_pContext, TEXT("Glanix"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_IcePillar", CGlanix_IcePillar::Create(m_pDevice, m_pContext, TEXT("Clanix_IcePillar")), LAYER_PROP)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_FireSpirit", CGlanix_FireSpirit::Create(m_pDevice, m_pContext, TEXT("Clanix_IcePillar")), LAYER_PROP)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Spawner_Ice01", CSpawner_Ice01::Create(m_pDevice, m_pContext, TEXT("Spawner_Ice01")), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia", CStellia::Create(m_pDevice, m_pContext, TEXT("Stellia"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_DreamerMazeWitch", CDMWitch::Create(m_pDevice, m_pContext, TEXT("DreamerMazeWitch"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Water", CBaobam_Water::Create(m_pDevice, m_pContext, TEXT("Baobam_Water"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shadow_Thief", CShadow_Thief::Create(m_pDevice, m_pContext, TEXT("Shadow_Thief"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_IceBearMan", CIceBearMan::Create(m_pDevice, m_pContext, TEXT("IceBearMan"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_PumpkinCandle", CPumpkinCandle::Create(m_pDevice, m_pContext, TEXT("PumpkinCandle"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown", CClown::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown_Wizard", CClown_Wizard::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Dark", CBaobam_Dark::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
		return E_FAIL;

	/* Npc */
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFAT01", CHumanFAT01::Create(m_pDevice, m_pContext, TEXT("HumanFAT01")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL04", CHumanFL04::Create(m_pDevice, m_pContext, TEXT("HumanFL04")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL05", CHumanFL05::Create(m_pDevice, m_pContext, TEXT("HumanFL05")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL07", CHumanFL07::Create(m_pDevice, m_pContext, TEXT("HumanFL07")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFLCapitalMerchant", CHumanFLCapitalMerchant::Create(m_pDevice, m_pContext, TEXT("HumanFLCapitalMerchant")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_KingdomGuard", CKingdomGuard::Create(m_pDevice, m_pContext, TEXT("KingdomGuard")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_NpcWeapon_Halberd", CNpcWeapon_Halberd::Create(m_pDevice, m_pContext, TEXT("NpcWeapon_Halberd")), LAYER_WEAPON)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanML12", CHumanML12::Create(m_pDevice, m_pContext, TEXT("HumanML12")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMM03", CHumanMM03::Create(m_pDevice, m_pContext, TEXT("HumanMM03")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_LuxerionHuman", CLuxerionHuman::Create(m_pDevice, m_pContext, TEXT("LuxerionHuman")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChild01", CHumanChild01::Create(m_pDevice, m_pContext, TEXT("HumanChild01")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChild02", CHumanChild02::Create(m_pDevice, m_pContext, TEXT("HumanChild02")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerCat", CSeekerCat::Create(m_pDevice, m_pContext, TEXT("SeekerCat")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerKing", CSeekerKing::Create(m_pDevice, m_pContext, TEXT("SeekerKing")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerObserver", CSeekerObserver::Create(m_pDevice, m_pContext, TEXT("SeekerObserver")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ruslan", CRuslan::Create(m_pDevice, m_pContext, TEXT("Ruslan")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFS03", CHumanFS03::Create(m_pDevice, m_pContext, TEXT("HumanFS03")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SwiftSolutionMaster", CSwiftSolutionMaster::Create(m_pDevice, m_pContext, TEXT("SwiftSolutionMaster")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Zehra", CZehra::Create(m_pDevice, m_pContext, TEXT("Zehra")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Enbi", CEnbi::Create(m_pDevice, m_pContext, TEXT("Enbi")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Edellian", CEdellian::Create(m_pDevice, m_pContext, TEXT("Edellian")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Chloe", CChloe::Create(m_pDevice, m_pContext, TEXT("Chloe")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Aren", CAren::Create(m_pDevice, m_pContext, TEXT("Aren")), LAYER_NPC)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChildHalloweenA", CHumanChildHalloweenA::Create(m_pDevice, m_pContext, TEXT("HumanChildHalloweenA")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChildHalloweenB", CHumanChildHalloweenB::Create(m_pDevice, m_pContext, TEXT("HumanChildHalloweenB")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CaliaHuman", CCaliaHuman::Create(m_pDevice, m_pContext, TEXT("CaliaHuman")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMSCrossFieldMerchant", CHumanMSCrossFieldMerchant::Create(m_pDevice, m_pContext, TEXT("HumanMSCrossFieldMerchant")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_RunnerCat", CRunnerCat::Create(m_pDevice, m_pContext, TEXT("RunnerCat")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_ThiefCat", CThiefCat::Create(m_pDevice, m_pContext, TEXT("ThiefCat")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ghost1", CGhost1::Create(m_pDevice, m_pContext, TEXT("Ghost1")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ghost2", CGhost2::Create(m_pDevice, m_pContext, TEXT("Ghost2")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ruby", CRuby::Create(m_pDevice, m_pContext, TEXT("Ruby")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFSPioneer", CHumanFSPioneer::Create(m_pDevice, m_pContext, TEXT("HumanFSPioneer")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_TreeGrandfa", CTreeGrandfa::Create(m_pDevice, m_pContext, TEXT("TreeGrandfa")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Verde", CVerde::Create(m_pDevice, m_pContext, TEXT("Verde")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Gosling", CGosling::Create(m_pDevice, m_pContext, TEXT("Gosling")), LAYER_NPC)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_FunyaSnowman", CFunyaSnowman::Create(m_pDevice, m_pContext, TEXT("FunyaSnowman")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GiftFunyaSnowman", CGiftFunyaSnowman::Create(m_pDevice, m_pContext, TEXT("GiftFunyaSnowman")), LAYER_NPC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_AquarisBella", CAquarisBella::Create(m_pDevice, m_pContext, TEXT("AquarisBella")), LAYER_NPC)))
		return E_FAIL;


	m_strLoading = TEXT("모델을 로딩 중 입니다.");

	/* Monster */
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Stellia", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Stellia/", L"Stellia")))
		return E_FAIL;

	m_strLoading = TEXT("모델을 로딩 중 입니다.");

	/*if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Stellia", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Stellia/", L"Stellia")))
		return E_FAIL;*/

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Glanix", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Glanix/", L"Glanix")))
		return E_FAIL;

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DreamerMazeWitch", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/DreamerMazeWitch/", L"DreamerMazeWitch")))
	//	return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Baobam_Water", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Ice/Baobam_Water/", L"Baobam_Water")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Shadow_Thief", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Ice/Shadow_Thief/", L"Shadow_Thief")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_IceBearMan", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Ice/IceBearMan/", L"IceBearMan")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_PumpkinCandle", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/PumpkinCandle/", L"PumpkinCandle")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Clown", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Clown/", L"Clown")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Clown_Wizard", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Clown_Wizard/", L"Clown_Wizard")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Baobam_Dark", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Baobam_Dark/", L"Baobam_Dark")))
		return E_FAIL;


	/* Npc */
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFAT01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFAT01/", L"HumanFAT01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL04", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL04/", L"HumanFL04")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL05", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL05/", L"HumanFL05")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL07", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL07/", L"HumanFL07")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFLCapitalMerchant", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFLCapitalMerchant/", L"HumanFLCapitalMerchant")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_KingdomGuard", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/KingdomGuard/", L"KingdomGuard")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_NpcWeapon_Halberd", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Npc_Weapon/NpcWeapon_Halberd/", L"NpcWeapon_Halberd")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanML12", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanML12/", L"HumanML12")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMM03", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanMM03/", L"HumanMM03")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_LuxerionHuman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/LuxerionHuman/", L"LuxerionHuman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChild01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanChild01/", L"HumanChild01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChild02", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanChild02/", L"HumanChild02")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SeekerCat", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/SeekerCat/", L"SeekerCat")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SeekerKing", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/SeekerKing/", L"SeekerKing")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SeekerObserver", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/SeekerObserver/", L"SeekerObserver")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Ruslan", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Ruslan/", L"Ruslan")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFS03", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFS03/", L"HumanFS03")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SwiftSolutionMaster", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/SwiftSolutionMaster/", L"SwiftSolutionMaster")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Zehra", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Zehra/", L"Zehra")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Enbi", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Enbi/", L"Enbi")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Edellian", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Edellian/", L"Edellian")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Chloe", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Chloe/", L"Chloe")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Aren", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Aren/", L"Aren")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChildHalloweenA", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanChildHalloweenA/", L"HumanChildHalloweenA")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChildHalloweenB", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanChildHalloweenB/", L"HumanChildHalloweenB")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_CaliaHuman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/CaliaHuman/", L"CaliaHuman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMSCrossFieldMerchant", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanMSCrossFieldMerchant/", L"HumanMSCrossFieldMerchant")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_RunnerCat", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/RunnerCat/", L"RunnerCat")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_ThiefCat", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/ThiefCat/", L"ThiefCat")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Ghost1", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Ghost1/", L"Ghost1")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Ghost2", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Ghost2/", L"Ghost2")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFSPioneer", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanFSPioneer/", L"HumanFSPioneer")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Ruby", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Ruby/", L"Ruby")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_TreeGrandfa", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/TreeGrandfa/", L"TreeGrandfa")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Verde", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Verde/", L"Verde")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Gosling", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Gosling/", L"Gosling")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_FunyaSnowman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/FunyaSnowman/", L"FunyaSnowman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GiftFunyaSnowman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/GiftFunyaSnowman/", L"GiftFunyaSnowman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_AquarisBella", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/AquarisBella/", L"AquarisBella")))
		return E_FAIL;

	CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_TEST);

	if (FAILED(Loading_Proto_AllObjects(L"../Bin/Export/NonAnimModel/Map/")))
		return E_FAIL;

	if (FAILED(Loading_Proto_DynamicObjects(L"..Bin/Export/AnimModel/Map/")))
		return E_FAIL;

	Load_Map_Data(L"Evermore");


	if (false == g_bFirstLoading)
	{
		if (FAILED(CWeapon_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;

		if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;

		if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;
	}
	
	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;
	g_bFirstLoading = true;

	return S_OK;
}





HRESULT CLoader::Loading_For_Level_Tool()
{
	/* For.ImGuiManager */
	m_strLoading = TEXT("툴 작업을 준비중입니다.");


	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.Prototype_Component_Shader_Model */


	m_strLoading = TEXT("객체 원형을 로딩 중 입니다.");


	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Dummy"),
		CDummy::Create(m_pDevice, m_pContext, TEXT("Dummy")), LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

 	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_TERRAIN)))
		return E_FAIL;

#pragma region Monster

	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp = 100;
	statDesc.fMaxMp = 100;


	/*if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia", CStellia::Create(m_pDevice, m_pContext, TEXT("Stellia"), statDesc), LAYER_MONSTER)))
		return E_FAIL;*/
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Water", CBaobam_Water::Create(m_pDevice, m_pContext, TEXT("Baobam_Water"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shadow_Thief", CShadow_Thief::Create(m_pDevice, m_pContext, TEXT("Shadow_Thief"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_PumpkinCandle", CPumpkinCandle::Create(m_pDevice, m_pContext, TEXT("PumpkinCandle"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown", CClown::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown_Wizard", CClown_Wizard::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Dark", CBaobam_Dark::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER)))
	//	return E_FAIL;
	/*if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_DreamerMazeWitch", CDMWitch::Create(m_pDevice, m_pContext, TEXT("DreamerMazeWitch"), statDesc), LAYER_MONSTER)))
		return E_FAIL;*/

	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Body", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Test/", L"SwordMan_Body")))
	//	return E_FAIL;

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Witch", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Test/", L"Witch")))
	//	return E_FAIL;

	/*if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Stellia", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Stellia/", L"Stellia")))
		return E_FAIL;*/

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Baobam_Water", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Ice/Baobam_Water/", L"Baobam_Water")))
	//	return E_FAIL;

	/*if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Shadow_Thief", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Ice/Shadow_Thief/", L"Shadow_Thief")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DreamerMazeWitch", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/DreamerMazeWitch/", L"DreamerMazeWitch")))
		return E_FAIL;*/

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_PumpkinCandle", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/PumpkinCandle/", L"PumpkinCandle")))
	//	return E_FAIL;

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Clown", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Clown/", L"Clown")))
	//	return E_FAIL;

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Clown_Wizard", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Clown_Wizard/", L"Clown_Wizard")))
	//	return E_FAIL;

	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Baobam_Dark", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/Baobam_Dark/", L"Baobam_Dark")))
	//	return E_FAIL;
#pragma endregion


	if (FAILED(Loading_Proto_AllObjects(L"../Bin/Export/NonAnimModel/Map/")))
		return E_FAIL;

	if (FAILED(Loading_Proto_DynamicObjects(L"..Bin/Export/AnimModel/Map/")))
		return E_FAIL;

		
#pragma region Particle && Effect
#pragma region Particle
	// 툴 파티클
	CParticle::PARTICLE_DESC ParticleInfo = {};
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_TempParticle"),
		CParticle::Create(m_pDevice, m_pContext, TEXT("TempParticle"), &ParticleInfo), 
		LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;
#pragma endregion

#pragma region Effect
	// 툴 이펙트
	CEffect::EFFECT_DESC EffectInfo = {};
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_TempMeshEffect"),
		CEffect::Create(m_pDevice, m_pContext, TEXT("TempMeshEffect"), &EffectInfo), 
		LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;
#pragma endregion
#pragma endregion

#pragma region TerrainBrush


	//if (FAILED(GI->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_StructuredBuffer"),
	//	CStructuredBuffer::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
#pragma endregion

#pragma region Parts


	/* 툴 팔로우 카메라 테스트 용도 */
	/*{
		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Dummy", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/SwordMan/Dummy/", L"SwordMan_Dummy")))
			return E_FAIL;

		if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Character_SwordMan", CCharacter_SwordMan::Create(m_pDevice, m_pContext, TEXT("SwordMan")), LAYER_CHARACTER)))
			return E_FAIL;
	}*/

#pragma endregion


	/* Prototype_GameObject_TempSword */
	{
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_TempSword"),
			CWeapon_SwordTemp::Create(m_pDevice, m_pContext, TEXT("TempSword")), LAYER_WEAPON)))
			return E_FAIL;

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_TempSword", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Weapon/", L"TempSword")))
			return E_FAIL;


		if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_NpcWeapon_Halberd"),
			CNpcWeapon_Halberd::Create(m_pDevice, m_pContext, TEXT("NpcWeapon_Halberd")), LAYER_WEAPON)))
			return E_FAIL;

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_NpcWeapon_Halberd", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Npc_Weapon/NpcWeapon_Halberd/", L"NpcWeapon_Halberd")))
			return E_FAIL;
	}



	//if (false == g_bFirstLoading)
	//{
	//	if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
	//		return E_FAIL;

	//	if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
	//		return E_FAIL;
	//}

	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	_matrix		PivotMatrix = XMMatrixIdentity();

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Load_Navi_Data(const wstring& strNaviFileName)
{
	const wstring strNaviFilePath = L"../Bin/DataFiles/Map/" + strNaviFileName + L"/" + strNaviFileName + L".nav";
	CNavigation* pPrototypeNavigation = dynamic_cast<CNavigation*>(GI->Find_Prototype_Component(LEVEL_STATIC, L"Prototype_Component_Navigation"));

	if (nullptr == pPrototypeNavigation)
		return E_FAIL;

	if (FAILED(pPrototypeNavigation->Load_NaviData(strNaviFilePath)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLoader::Load_Map_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L".map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	GI->Clear_PhysX_Ground();

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (i == LAYER_TYPE::LAYER_CAMERA
			|| i == LAYER_TYPE::LAYER_TERRAIN
			|| i == LAYER_TYPE::LAYER_BACKGROUND
			|| i == LAYER_TYPE::LAYER_SKYBOX
			|| i == LAYER_TYPE::LAYER_UI
			|| i == LAYER_TYPE::LAYER_PLAYER
			|| i == LAYER_TYPE::LAYER_WEAPON
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC
			|| i == LAYER_TYPE::LAYER_WEAPON
			|| i == LAYER_TYPE::LAYER_MONSTER
			|| i == LAYER_TYPE::LAYER_CHARACTER
			|| i == LAYER_TYPE::LAYER_DYNAMIC)
			continue;

		GI->Clear_Layer(m_eNextLevel, i);
		{
			_uint iObjectCount = File->Read<_uint>();

			for (_uint j = 0; j < iObjectCount; ++j)
			{
				// 3. Object_Prototype_Tag
				wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
				wstring strObjectTag = CUtils::ToWString(File->Read<string>());

				CGameObject* pObj = nullptr;
				if (FAILED(GI->Add_GameObject(m_eNextLevel, i, strPrototypeTag, nullptr, &pObj)))
				{
					MSG_BOX("Load_Map_Objects_Failed.");
					return E_FAIL;
				}

				if (nullptr == pObj)
				{
					MSG_BOX("Add_Object_Failed.");
					return E_FAIL;
				}
				pObj->Set_ObjectTag(strObjectTag);

				CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pTransform)
				{
					MSG_BOX("Get_Transform_Failed.");
					return E_FAIL;
				}

				// 6. Obejct States
				_float4 vRight, vUp, vLook, vPos;

				File->Read<_float4>(vRight);
				File->Read<_float4>(vUp);
				File->Read<_float4>(vLook);
				File->Read<_float4>(vPos);

				pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
				pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
				pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
				pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
			}
		}


	}

	return S_OK;

}

HRESULT CLoader::Loading_Proto_AllObjects(const wstring& strPath)
{
	for (auto& p : std::filesystem::directory_iterator(strPath))
	{
		if (p.is_directory())
		{
			Loading_Proto_AllObjects(p.path().wstring());
		}

 		wstring strFilePath = CUtils::PathToWString(p.path().wstring());
		
		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".fbx"), strExt) || 0 == lstrcmp(TEXT(".mesh"), strExt))
		{
			if (strFilePath.find(L"Buildings") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CBuilding::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_BUILDING, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_BUILDING)))
				{
					return E_FAIL;
				}
			}
			else if ((strFilePath.find(L"Props") != wstring::npos) || (strFilePath.find(L"Prop") != wstring::npos))
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CProbs::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_PROP, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_PROP)))
				{
					return E_FAIL;
				}
			}
			else if (strFilePath.find(L"Plants") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CPlants::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_GRASS, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_GRASS)))
					return E_FAIL;
			}
			else if (strFilePath.find(L"Grounds") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CGrounds::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_GROUND, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_GROUND)))
					return E_FAIL;
			}
			else if (strFilePath.find(L"Rocks And Trees") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CTreeRock::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_TREEROCK, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_TREEROCK)))
					return E_FAIL;
			}
			//else if (strFilePath.find(L"Water") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CWater::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_WATER)))
			//		return E_FAIL;
			//}
			//else if (strFilePath.find(L"Road") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_ROAD)))
			//		return E_FAIL;
			//}

			//else if (strFilePath.find(L"Mountain") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_MOUNTAIN)))
			//		return E_FAIL;
			//}
		}

	}
	return S_OK;
}

HRESULT CLoader::Loading_Proto_DynamicObjects(const wstring& strPath)
{
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Cat"), CCat::Create(m_pDevice, m_pContext, TEXT("Animal_Cat"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Dochi"), CDochi::Create(m_pDevice, m_pContext, TEXT("Animal_Dochi"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_DuckGoo"), CDuckGoo::Create(m_pDevice, m_pContext, TEXT("Animal_DuckGoo"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Fox"), CFox::Create(m_pDevice, m_pContext, TEXT("Animal_Fox"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Rabbit"), CRabbit::Create(m_pDevice, m_pContext, TEXT("Animal_Rabbit"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_PolarBear"), CPolarBear::Create(m_pDevice, m_pContext, TEXT("Animal_PolarBear"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Ermine"), CErmine::Create(m_pDevice, m_pContext, TEXT("Animal_Ermine"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Pigeon"), CPigeon::Create(m_pDevice, m_pContext, TEXT("Animal_Pigeon"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Water"), CWater::Create(m_pDevice, m_pContext, TEXT("Evermore_Water"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Skydome"), CSkyDome::Create(m_pDevice, m_pContext, TEXT("Sky_dome"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_SKYBOX)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_SkyPlane"), CSkyPlane::Create(m_pDevice, m_pContext, TEXT("Sky_Plane"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_SKYBOX)))
		return E_FAIL;


	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cat", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Cat")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Dochi", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Dochi")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DuckGoo", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Duckgoo")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Fox", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_FennecFox")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Rabbit", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Rabbit")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_PolarBear", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_PolarBear")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Ermine", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Ermine")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Pigeon", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/", L"Animal_Pigeon")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Everemore_Water", CModel::TYPE_NONANIM, L"../Bin/Export/AnimModel/Map/Water/", L"Evermore_WaterA_01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Sky_dome", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/SkyDom/", L"Skydome")))
		return E_FAIL;


	return S_OK;
}



CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel, const wstring& strFolderName)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, strFolderName)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}