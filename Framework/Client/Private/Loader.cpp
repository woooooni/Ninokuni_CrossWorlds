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
#include "Aurora.h"


#include <filesystem>
#include "Utils.h"
#include "UI_Manager.h"
#include "ImGui_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Weapon_Manager.h"
#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Skill_Manager.h"

#include "UI_Logo_Background.h"
#include "UI_Flare.h"
#include "UI_CharacterDummy.h"
#include "UI_CostumeTab_Map.h"
#include "Mirror.h"
#include "UIMinigame_Manager.h"

#include "Spawner_Ice01.h"
#include "Spawner_Ice02.h"
#include "Spawner_Ice03.h"
#include "Glanix.h"
#include "Glanix_IcePillar.h"
#include "Glanix_Phoenix.h"
#include "Glanix_ShockWave.h"
#include "Glanix_IceBall.h"
#include "Glanix_Icicle.h"

#include "Baobam_Water.h"
#include "Shadow_Thief.h"
#include "IceBearMan.h"

#include "Stellia.h"
#include "Stellia_Crystal.h"
#include "Stellia_Crystal_Destructible.h"
#include "Stellia_Crystal_Explosion.h"
#include "Stellia_Explosion.h"

#include "DMWitch.h"

#include "Spawner_Witch01.h"
#include "PumpkinCandle.h"
#include "Clown.h"
#include "Clown_Wizard.h"
#include "Clown_Wizard_DarkBall.h"
#include "Baobam_Dark.h"

#include "Criminal_Monster.h"

#include "Player.h"

#include "Kuu.h"
#include "Engineer_Npc.h"
#include "Destroyer_Npc.h"

#include "Criminal_Npc.h"
#include "DreamMazeWitch_Npc.h"
#include "Witch_BlackHole.h"
#include "Witch_Laser.h"
#include "Witch_Rage02Sphere.h"

#include "Door_Enter_FX.h"
#include "HumanFAT01.h"
#include "MouseFolkFat01.h"
#include "HumanFL04.h"
#include "HumanFL05.h"
#include "HumanFL07.h"
#include "HumanFM02.h"
#include "HumanFM09.h"
#include "GrimalkinOld01.h"
#include "HumanFLCapitalMerchant.h"
#include "KingdomGuard.h"
#include "NpcWeapon_Halberd.h"
#include "HumanML04.h"
#include "HumanML12.h"
#include "GrimalKinML03.h"
#include "GrimalKinML04.h"
#include "HumanMM03.h"
#include "HumanMM05.h"
#include "HumanMM18.h"
#include "HumanMM21.h"
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
#include "BlackSmithMaster.h"
#include "GrimalKinML01.h"
#include "GrimalKinML02.h"

#include "HumanChildHalloweenA.h"
#include "HumanChildHalloweenB.h"
#include "HumanFM12.h"
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
#include "Cyan.h"

#include "FunyaSnowman.h"
#include "GiftFunyaSnowman.h"
#include "AquarisBella.h"

#include "Quest_DestSpot.h"

#include "Particle.h"
#include "Effect.h"
#include "Decal.h"
#include "Motion_Trail.h"

#include "Portal.h"
#include "Trigger.h"

#include "Weapon_SwordTemp.h"

#include "UI_Dummy_Swordsman.h"
#include "UI_Dummy_Destroyer.h"
#include "UI_Dummy_Engineer.h"

#include "TowerDefence_Manager.h"
#include "Grandprix_Manager.h"

#include "Vehicle_Udadak.h"
#include "Vehicle_Flying_Biplane.h"
#include "Vehicle_Flying_EnemyBiplane.h"
#include "Vehicle_Flying_EnemyBoto.h"
#include "Character_Biplane_Bullet.h"
#include "Enemy_Biplane_Bullet.h"
#include "Biplane_GuidedMissile.h"
#include "Biplane_ThunderCloud.h"
#include "Biplane_BlackHole.h"
#include "Enemy_Biplane_BulletBall.h"
#include "Enemy_GuidedMissile.h"

#include "Grandprix_Engineer.h"
#include "Grandprix_Enemy_Ghost2.h"
#include "Grandprix_ItemBox.h"
#include "Grandprix_Goal.h"

#include "Respawn_Box.h"
#include "CurlingGame_Manager.h"



_bool CLoader::g_bFirstLoading = false;
_bool CLoader::g_bLevelFirst[LEVELID::LEVEL_WITCHFOREST + 1] = {};

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

	case LEVEL_KINGDOMHALL:
		hr = Loading_For_Level_Kingdom();
		break;

	case LEVEL_ICELAND:
		hr = Loading_For_Level_IceLand();
		break;

	case LEVEL_WITCHFOREST:
		hr = Loading_For_Level_WitchForest();
		break;

	case LEVEL_TOOL:
		hr = Loading_For_Level_Tool();
		break;
	}

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

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
	if(GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Grass"), 
		CGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_Grass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Grass_Purple"),
		CGrass_Purple::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_Grass_Purple")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LargeGrass"),
		CLargeGrass_Purple::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_LargeGrass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LargeGrass_Purple"),
		CLargeGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_LargeGrass_Purple")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_EntireGrass"),
		CEntireGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_EntireGrass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;



	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Background"), CUI_Logo_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Flare"), CUI_Flare::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Portal"), CPortal::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Trigger"), CTrigger::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_RespawnBox"), CRespawn_Box::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Door_Enter"), CDoor_Enter_FX::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Kuu", CKuu::Create(m_pDevice, m_pContext, TEXT("Kuu")), LAYER_NPC, true)))
		return E_FAIL;


	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Kuu", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Public/Kuu/", L"Kuu")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DoorEnter", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Door/", L"Door")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_GrassPlane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Large_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_Large_Grass_Plane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Entire_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_Grass_Entire_Plane")))
		return E_FAIL;


	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Engineer_Bullet", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Bullet/", L"Engineer_Bullet")))
		return E_FAIL;

	
	switch (g_eLoadCharacter)
	{
	case Client::SWORDMAN_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_SWORDMAN] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::SWORD_MAN);
	}
		break;
	case Client::DESTROYER_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_DESTROYER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::DESTROYER);
	}
		break;
	case Client::ENGINEER_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_ENGINEER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::ENGINEER);
	}
		break;
	case Client::ALL_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_SWORDMAN] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::SWORD_MAN);
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_DESTROYER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::DESTROYER);
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_ENGINEER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::ENGINEER);
	}
		break;
	default:
		break;
	}

	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if(true == m_Threads[i].valid())
			m_Threads[i].wait();
	}
		
	if (FAILED(Reserve_Character_Managers()))
		return E_FAIL;

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

	if (false == g_bLevelFirst[LEVEL_LOBBY])
	{
		if (FAILED(CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_LOBBY)))
			return E_FAIL;

		switch (g_eLoadCharacter)
		{
		case Client::SWORDMAN_CH:
		{
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Swordman",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Swordman"), CHARACTER_TYPE::SWORD_MAN), LAYER_CHARACTER, true)))
				return E_FAIL;
		}
		break;
		case Client::DESTROYER_CH:
		{
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Destroyer",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Destroyer"), CHARACTER_TYPE::DESTROYER), LAYER_CHARACTER, true)))
				return E_FAIL;
		}
		break;
		case Client::ENGINEER_CH:
		{
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Engineer",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Engineer"), CHARACTER_TYPE::ENGINEER), LAYER_CHARACTER, true)))
				return E_FAIL;
		}
		break;
		case Client::ALL_CH:
		{
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Swordman",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Swordman"), CHARACTER_TYPE::SWORD_MAN), LAYER_CHARACTER, true)))
				return E_FAIL;
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Destroyer",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Destroyer"), CHARACTER_TYPE::DESTROYER), LAYER_CHARACTER, true)))
				return E_FAIL;
			if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Engineer",
				CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy_Engineer"), CHARACTER_TYPE::ENGINEER), LAYER_CHARACTER, true)))
				return E_FAIL;
		}
		break;
		default:
			break;
		}

		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Map_CostumRoom"),
			CUI_CostumeTab_Map::Create(m_pDevice, m_pContext), LAYER_PROP, true)))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Map_Mirror"),
			CMirror::Create(m_pDevice, m_pContext, TEXT("Map_Common_Mirror"), OBJ_TYPE::OBJ_PROP),
			LAYER_TYPE::LAYER_PROP, true)))
			return E_FAIL;

		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Dummy_Swordsman"),
			CUI_Dummy_Swordsman::Create(m_pDevice, m_pContext), LAYER_CHARACTER, true)))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Dummy_Destroyer"),
			CUI_Dummy_Destroyer::Create(m_pDevice, m_pContext), LAYER_CHARACTER, true)))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Dummy_Engineer"),
			CUI_Dummy_Engineer::Create(m_pDevice, m_pContext), LAYER_CHARACTER, true)))
			return E_FAIL;

		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Common_ColliderWall"),
			CColliderWall::Create(m_pDevice, m_pContext, TEXT("Common_ColliderWall"), OBJ_TYPE::OBJ_BUILDING),
			LAYER_TYPE::LAYER_BUILDING, true)))
			return E_FAIL;

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Collider_Wall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/ColliderWall/", L"Common_ColliderWall")))
			return E_FAIL;

		m_Threads[LOADING_THREAD::STATIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Static_Map_Objects, this, L"../Bin/Export/NonAnimModel/Map/");
		m_Threads[LOADING_THREAD::DYNAMIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Dynamic_Map_Objects, this, L"../Bin/Export/AnimModel/Map/");

		// Mirror
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Mirror"),
			CMirror::Create(m_pDevice, m_pContext, TEXT("Common_Mirror"), OBJ_TYPE::OBJ_PROP),
			LAYER_TYPE::LAYER_PROP, true)))
			return E_FAIL;
		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Mirror", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/CustomRoom/", L"CustomRoomMirror")))
			return E_FAIL;
		// Mirror


		g_bLevelFirst[LEVEL_LOBBY] = true;
	}

	


	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Loading_Proto_Monster_Npc, this);
	m_Threads[LOADING_THREAD::VEHICLES] = std::async(&CLoader::Loading_Proto_Vehicles, this);

	m_Threads[LOADING_THREAD::STATIC_OBJECT_PROTOTYPE].wait();
	m_Threads[LOADING_THREAD::DYNAMIC_OBJECT_PROTOTYPE].wait();
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC].wait();

	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Lobby");

	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if(true == m_Threads[i].valid())
			m_Threads[i].wait();
	}


		
	if (FAILED(Reserve_Character_Managers()))
		return E_FAIL;

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


	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");

		
	g_bFirstLoading = true;
	if (false == g_bLevelFirst[LEVEL_EVERMORE])
	{
		switch (g_ePlayCharacter)
		{
		case Client::SWORDMAN_CH:
			CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::SWORD_MAN, XMVectorSet(0.f, 0.f, 0.f, 1.f), true);
			break;
		case Client::DESTROYER_CH:
			CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::DESTROYER, XMVectorSet(0.f, 0.f, 0.f, 1.f), true);
			break;
		case Client::ENGINEER_CH:
			CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::ENGINEER, XMVectorSet(0.f, 0.f, 0.f, 1.f), true);
			break;
		default:
			CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::SWORD_MAN, XMVectorSet(0.f, 0.f, 0.f, 1.f), true);
			break;
		}

		m_Threads[LOADING_THREAD::TOWER_DEFENCE_READY] = std::async(&CLoader::Loading_For_TowerDefence, this);

		// 미니게임(타워 디펜스)
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_TowerSelect"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/TowerDefence/UI_Minigame_Select_Renewal_%d.png"), 8))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Timer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/TowerDefence/UI_Minigame_Timer.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_StartBtn"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/TowerDefence/UI_Minigame_StartBtn_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Background"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/TowerDefence/UI_Minigame_Towerlist_Background.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Gold"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/TowerDefence/UI_Minigame_Gold.png")))))
			return E_FAIL;

		// 미니게임(그랑프리)
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_ClassSkills"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Skill_%d.png"), 3))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_BurstSkill"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Burst_Skill_0.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_MaskTexture"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Skill_Mask.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_SkillFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_SkillFrame_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar_Backs"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Granprix_HPBack_Test_%d.png"), 9))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Granprix_HP.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Frame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Background.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Text_%d.png"), 3))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Text_Count_%d.png"), 3))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_SpaceIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Gauge/UI_Grandprix_Gauge_SpaceIcon.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_BiplaneIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Gauge/UI_Grandprix_BiplaneIcon.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_Gauge"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Gauge/UI_Grandprix_Gauge_%d.png"), 23))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_GaugeBackground"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Gauge/UI_Grandprix_Gauge_Background.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_GaugeGlowBackground"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Gauge/UI_Grandprix_Gauge_Glow.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Vehicle_Minigame_Grandprix_Biplane_ColorBlue"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/AnimModel/Vehicle/Biplane/SKM_Biplane_White_Color.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Vehicle_Minigame_Grandprix_Boto_Textures"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/AnimModel/Vehicle/Boto/SKM_Boto3_Color_TextureCom_%d.dds"), 5))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_AimCursor"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Aim_Cursor_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_PlayerHP_Background"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_PlayerHP_Background.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar_Lerp"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Granprix_HP_Back.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_IntroBackground"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Intro/UI_Grandprix_Intro_Background.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_IntroIcons"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Intro/UI_Grandprix_Intro_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_Frame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Rader/UI_Grandprix_Minimap_Frame.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_FrameBackground"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Rader/UI_Grandprix_Minimap_Background_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_MaskTexture"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Rader/UI_Grandprix_Minimap_MaskRatioTest.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Grandprix_Rader_Icons"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Rader/UI_Grandprix_Rader_Icon_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_Circle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Rader/UI_Grandprix_RaderCircle_SampleB_%d.png"), 12))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Vignette_Rush"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Vignette/UI_Grandprix_Vignette_Speed_%d.png"), 7))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Vignette_OnDamaged"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Vignette/UI_Grandprix_Vignette_Damaged.png"), 7))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_TextError"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Error_1.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_ItemPopup"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Item/UI_Grandprix_Popup_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_ItemPopup_Background"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/Item/UI_Grandprix_ItemPopup_1.png")))))
			return E_FAIL;
		if (FAILED(GI->Add_Prototype(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_Target"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Grandprix/UI_Grandprix_Target_2.png")))))
			return E_FAIL;

		// 미니게임용 프로토타입
		if (FAILED(CUIMinigame_Manager::GetInstance()->Ready_MinigameUI_Prototypes(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;

		if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH || g_eLoadCharacter == LOAD_CHARACTER_TYPE::ENGINEER_CH)
		{
			if (GI->Add_Prototype(TEXT("Prototype_GameObject_Grandprix_Engineer"),
				CGrandprix_Engineer::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
				return E_FAIL;
		}

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LensFlare"),
			CLensFlare::Create(m_pDevice, m_pContext, TEXT("Common_LensFlare"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true))
			return E_FAIL;

		// 탈 것
		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Vehicle_Udadak"),
			CVehicle_Udadak::Create(m_pDevice, m_pContext, TEXT("Vehicle_Udadak")), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Vehicle_Biplane"),
			CVehicle_Flying_Biplane::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;
		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Vehicle_EnemyBiplane"),
			CVehicle_Flying_EnemyBiplane::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_MONSTER, true))
			return E_FAIL;
		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Vehicle_EnemyBoto"),
			CVehicle_Flying_EnemyBoto::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_MONSTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Character_Biplane_Bullet"),
			CCharacter_Biplane_Bullet::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC CharacterProjectileDesc = {};
		CharacterProjectileDesc.bPool = true;

		if (FAILED(CPool<CCharacter_Biplane_Bullet>::Ready_Pool(m_pDevice, m_pContext, L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &CharacterProjectileDesc, 500)))
			return E_FAIL;



		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Biplane_Bullet"),
			CEnemy_Biplane_Bullet::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC EnemyProjectileDesc = {};
		EnemyProjectileDesc.bPool = true;

		if (FAILED(CPool<CEnemy_Biplane_Bullet>::Ready_Pool(m_pDevice, m_pContext, L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &EnemyProjectileDesc, 500)))
			return E_FAIL;

		// Enemy용.

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Biplane_BulletBall"),
			CEnemy_Biplane_BulletBall::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Biplane_GuidedMissile"),
			CEnemy_GuidedMissile::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (FAILED(CPool<CEnemy_GuidedMissile>::Ready_Pool(m_pDevice, m_pContext, L"Prototype_GameObject_Enemy_Biplane_GuidedMissile", LAYER_TYPE::LAYER_CHARACTER, &CharacterProjectileDesc, 500)))
			return E_FAIL;
		

		// 플레이어용.
		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Biplane_GuidedMissile"),
			CBiplane_GuidedMissile::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Biplane_ThunderCloud"),
			CBiplane_Thunder_Cloud::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Biplane_BlackHole"),
			CBiplane_BlackHole::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_CHARACTER, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prototype_GameObject_Grandprix_ItemBox"),
			CGrandprix_ItemBox::Create(m_pDevice, m_pContext, TEXT("Grandprix_ItemBox")), LAYER_TYPE::LAYER_ETC, true))
			return E_FAIL;

		if (GI->Add_Prototype(TEXT("Prorotype_GameObject_Grandprix_Goal"),
			CGrandprix_Goal::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_ETC, true))
			return E_FAIL;
		
		g_bLevelFirst[LEVEL_EVERMORE] = true;

	}

	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Evermore");
	//m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Load_Npc_Data, this, L"Evermore");

	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}


	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Kingdom()
{
	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Kingdom");
	// m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Load_Monster_Data, this, L"Kingdom");
	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}

	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_IceLand()
{
	if (false == g_bLevelFirst[LEVEL_ICELAND])
	{
		/* 컬링 미니 게임 텍스처 프로토타입 추가 */
		{
			//if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_GaugeBar_Back"),
			//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/UI_Minigame_CurlingGame_GaugeBar_Frame.png")))))
			//	return E_FAIL;
			//
			//if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_GaugeBar"),
			//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/UI_Minigame_CurlingGame_GaugeBar_Full.png")))))
			//	return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Count"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Count_%d.png"), 10))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Guage_Frame"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Guage_Frame.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Icon_Destroyer"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Icon_Destroyer.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Icon_Swordsman"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Icon_Swordsman.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Info_Frame_Blue"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Info_Frame_Blue.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Info_Frame_Yellow"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Info_Frame_Yellow.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Life_Empty"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Life_Empty.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Life_Full"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Life_Full.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Lifes"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Lifes_%d.png"), 6))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_TitleFrame"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_TitleFrame.png")))))
				return E_FAIL;

			if (FAILED(GI->Add_Prototype(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Turn_Arrow"),
				CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MiniGame/Curling/Curling_Turn_Arrow.png")))))
				return E_FAIL;

		}

		/* 컬링 미니게임 UI 오브젝트 프로토타입 추가 */
		{
			if (FAILED(CUIMinigame_Manager::GetInstance()->Ready_MinigameUI_Prototypes(LEVELID::LEVEL_ICELAND)))
				return E_FAIL;
		}

		g_bLevelFirst[LEVEL_ICELAND] = true;
	}

	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Winter");
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Load_Monster_Data, this, L"Winter");
	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}

	g_bFirstLoading = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_WitchForest()
{
	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Moon"),
		CMoon::Create(m_pDevice, m_pContext, TEXT("Common_Moon"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX))
		return E_FAIL;

	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp = 100;
	statDesc.fMaxMp = 100;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Witch_Barricade"),
		CWitch_Barricade::Create(m_pDevice, m_pContext, TEXT("Witch_Barricade"), statDesc), LAYER_TYPE::LAYER_MONSTER, true))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Witch_Barricade", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/Witch_Barricade/", L"Witch_Barricade")))
		return E_FAIL;


	g_bFirstLoading = true;
	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Witch");
	//m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Load_Monster_Data, this, L"Witch");
	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}
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

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_UI_CharacterDummy_Swordman",
		CUI_CharacterDummy::Create(m_pDevice, m_pContext, TEXT("UI_Dummy"), CHARACTER_TYPE::SWORD_MAN), LAYER_CHARACTER)))
		return E_FAIL;

	m_strLoading = TEXT("모델을 로딩 중 입니다.");


	if (false == g_bFirstLoading)
	{
		switch (g_ePlayCharacter)
		{
		case Client::SWORDMAN_CH:
			m_Threads[LOADING_THREAD::CHARACTER_MODEL_SWORDMAN] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::SWORD_MAN);
			break;
		case Client::DESTROYER_CH:
			m_Threads[LOADING_THREAD::CHARACTER_MODEL_DESTROYER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::DESTROYER);
			break;
		case Client::ENGINEER_CH:
			m_Threads[LOADING_THREAD::CHARACTER_MODEL_ENGINEER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::ENGINEER);
			break;
		default:
			CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::SWORD_MAN, XMVectorSet(0.f, 0.f, 0.f, 1.f), true);
			break;
		}
	}
		

	
	m_Threads[LOADING_THREAD::STATIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Static_Map_Objects, this, L"../Bin/Export/NonAnimModel/Map/");
	m_Threads[LOADING_THREAD::DYNAMIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Dynamic_Map_Objects, this, L"../Bin/Export/AnimModel/Map/");
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Loading_Proto_Monster_Npc, this);

	m_Threads[LOADING_THREAD::STATIC_OBJECT_PROTOTYPE].wait();
	m_Threads[LOADING_THREAD::DYNAMIC_OBJECT_PROTOTYPE].wait();
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC].wait();


	m_Threads[LOADING_THREAD::LOAD_MAP] = std::async(&CLoader::Load_Map_Data, this, L"Winter");
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Load_Monster_Data, this, L"Winter");


	CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_TEST);
	

	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}

	if (FAILED(Reserve_Character_Managers()))
		return E_FAIL;

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

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Grass"),
		CGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_Grass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Grass_Purple"),
		CGrass_Purple::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_Grass_Purple")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LargeGrass"),
		CLargeGrass_Purple::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_LargeGrass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LargeGrass_Purple"),
		CLargeGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_LargeGrass_Purple")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_EntireGrass"),
		CEntireGrass::Create(m_pDevice, m_pContext, TEXT("Common_RealTime_EntireGrass")), LAYER_TYPE::LAYER_GRASS, true))
		return E_FAIL;

	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp = 100;
	statDesc.fMaxMp = 100;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Witch_Barricade"),
		CWitch_Barricade::Create(m_pDevice, m_pContext, TEXT("Witch_Barricade"), statDesc), LAYER_TYPE::LAYER_MONSTER, true))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_LensFlare"),
		CLensFlare::Create(m_pDevice, m_pContext, TEXT("Common_LensFlare"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX))
		return E_FAIL;

	if (GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Moon"),
		CMoon::Create(m_pDevice, m_pContext, TEXT("Common_Moon"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Trigger"), CTrigger::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	/* Prototype_GameObject_TempSword */
	{
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_TempSword"),
			CWeapon_SwordTemp::Create(m_pDevice, m_pContext, TEXT("TempSword")), LAYER_WEAPON)))
			return E_FAIL;

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_TempSword", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Weapon/Rifle/", L"Rifle_CyberPunks")))
			return E_FAIL;
	}
		
	// 툴 파티클
	CParticle::PARTICLE_DESC ParticleInfo = {};
	ParticleInfo.iVelocityUse = 30;
	ParticleInfo.pVelocityMin = new _float3[ParticleInfo.iVelocityUse];
	ParticleInfo.pVelocityMax = new _float3[ParticleInfo.iVelocityUse];
	ParticleInfo.pVelocityTime = new _float2[ParticleInfo.iVelocityUse];
	CParticle::PARTICLE_RIGIDBODY_DESC ParticleRigidbody = {};
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_TempParticle"),
		CParticle::Create(m_pDevice, m_pContext, TEXT("TempParticle"), &ParticleInfo, &ParticleRigidbody),
		LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	// 툴 이펙트
	CEffect::EFFECT_DESC EffectInfo = {};
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_TempMeshEffect"),
		CEffect::Create(m_pDevice, m_pContext, TEXT("TempMeshEffect"), &EffectInfo), 
		LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	// 툴 데칼
	CDecal::DECAL_DESC DecalInfo = {};
	CDecal::DECAL_SCALE_DESC DecalScaleInfo = {};
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_TempDecal"),
		CDecal::Create(m_pDevice, m_pContext, TEXT("TempDecal"), &DecalInfo, &DecalScaleInfo),
		LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	m_strLoading = TEXT("모델을 로딩 중 입니다.");



	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Common_ColliderWall"),
		CColliderWall::Create(m_pDevice, m_pContext, TEXT("Common_ColliderWall"), OBJ_TYPE::OBJ_BUILDING),
		LAYER_TYPE::LAYER_BUILDING)))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Collider_Wall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/ColliderWall/", L"Common_ColliderWall")))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Common_Mirror"),
		CMirror::Create(m_pDevice, m_pContext, TEXT("Common_Mirror"), OBJ_TYPE::OBJ_PROP),
		LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Mirror", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/CustomRoom/", L"CustomRoomMirror")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_GrassPlane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Large_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_Large_Grass_Plane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Entire_GrassPlane", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/GrassPlane/", L"Common_Grass_Entire_Plane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Witch_Barricade", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/Witch_Barricade/", L"Witch_Barricade")))
		return E_FAIL;

	switch (g_eLoadCharacter)
	{
	case Client::SWORDMAN_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_SWORDMAN] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::SWORD_MAN);
	}
	break;
	case Client::DESTROYER_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_DESTROYER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::DESTROYER);
	}
	break;
	case Client::ENGINEER_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_ENGINEER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::ENGINEER);
	}
	break;
	case Client::ALL_CH:
	{
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_SWORDMAN] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::SWORD_MAN);
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_DESTROYER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::DESTROYER);
		m_Threads[LOADING_THREAD::CHARACTER_MODEL_ENGINEER] = std::async(&CLoader::Loading_For_Character, this, CHARACTER_TYPE::ENGINEER);
	}
	break;
	default:
		break;
	}
		

	m_Threads[LOADING_THREAD::STATIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Static_Map_Objects, this, L"../Bin/Export/NonAnimModel/Map/");
	m_Threads[LOADING_THREAD::DYNAMIC_OBJECT_PROTOTYPE] = std::async(&CLoader::Loading_Proto_Dynamic_Map_Objects, this, L"../Bin/Export/AnimModel/Map/");
	m_Threads[LOADING_THREAD::MONSTER_AND_NPC] = std::async(&CLoader::Loading_Proto_Monster_Npc, this);


	for (_uint i = 0; i < LOADING_THREAD::THREAD_END; ++i)
	{
		if (true == m_Threads[i].valid())
			m_Threads[i].wait();
	}

	if (FAILED(Reserve_Character_Managers()))
		return E_FAIL;


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

/*===================================================================*/
HRESULT CLoader::Load_Map_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L".map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	vector<CGameObject*>* pStadium = CCurlingGame_Manager::GetInstance()->Get_Stadium();
	pStadium->clear();

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (i == LAYER_TYPE::LAYER_CAMERA
			|| i == LAYER_TYPE::LAYER_TERRAIN
			|| i == LAYER_TYPE::LAYER_BACKGROUND
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
				_uint ObjectType;
				File->Read(ObjectType);

				if (OBJ_TYPE::OBJ_TRIGGER == ObjectType)
					continue;

				// 3. Object_Prototype_Tag
				wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
				wstring strObjectTag = CUtils::ToWString(File->Read<string>());

				// 6. Obejct States
				_float4 vRight, vUp, vLook, vPos;

				File->Read<_float4>(vRight);
				File->Read<_float4>(vUp);
				File->Read<_float4>(vLook);
				File->Read<_float4>(vPos);

				OBJECT_INIT_DESC Init_Data = {};
				Init_Data.vStartPosition = vPos;

				CGameObject* pObj = nullptr;
				if (FAILED(GI->Add_GameObject(m_eNextLevel, i, strPrototypeTag, &Init_Data, &pObj, true)))
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

				vPos.w = 1.f;
	

				pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
				pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
				pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
				pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

				_bool IsQuest;
				File->Read<_bool>(IsQuest);
				pObj->Set_QuestItem(IsQuest);

				if (OBJ_TYPE::OBJ_MINIGAME_STRUCTURE == pObj->Get_ObjectType())
					pStadium->push_back(pObj);
			}
		}


	}



	return S_OK;

}

HRESULT CLoader::Load_Monster_Data(const wstring& strMonsterFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMonsterFileName + L"/" + strMonsterFileName + L"Monster.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

		GI->Clear_Layer(m_eNextLevel, i);
		 

		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;

			if (FAILED(GI->Add_GameObject(m_eNextLevel, i, strPrototypeTag, &Init_Data, &pObj, true)))
			{
				MSG_BOX("Load_Objects_Failed.");
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



			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}
	//MSG_BOX("Monster_Loaded.");
	return S_OK;
}

HRESULT CLoader::Load_Npc_Data(const wstring& strNpcFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(m_eNextLevel, i);


		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;
			if (FAILED(GI->Add_GameObject(m_eNextLevel, i, strPrototypeTag, &Init_Data, &pObj)))
			{
				MSG_BOX("Load_Objects_Failed.");
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

			_uint ObjectType;
			File->Read<_uint>(ObjectType);

			if (OBJ_TYPE::OBJ_NPC == ObjectType)
			{
				CGameNpc* pNpc = dynamic_cast<CGameNpc*>(pObj);

				if (pNpc == nullptr)
				{
					MSG_BOX("Fail Load : NPC");
					return E_FAIL;
				}

				_uint iSize;
				File->Read<_uint>(iSize);

				_uint eState;
				File->Read<_uint>(eState);


				if (iSize != 0)
				{
					vector<Vec4> Points;
					Points.reserve(iSize);

					for (_uint i = 0; i < iSize; ++i)
					{
						Vec4 vPoint;
						File->Read<Vec4>(vPoint);
						Points.push_back(vPoint);
					}

					pNpc->Set_RoamingArea(Points);

					if (Points.size() != 0)
					{
						vPos = Points.front();
						pNpc->Set_Point(true);
					}
				}

				CGameNpc::NPC_STAT eStat;
				File->Read<CGameNpc::NPC_STAT>(eStat);

				pNpc->Set_NpcState(static_cast<CGameNpc::NPC_STATE>(eState));
				CStateMachine* pStateMachine = pNpc->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
				if (pStateMachine != nullptr)
				{
					pStateMachine->Change_State(eState);
				}
				else
				{
					MSG_BOX("Fail Get : NPC_StateMachine");
					return E_FAIL;
				}
				pNpc->Set_Stat(eStat);
			}


			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}
	//MSG_BOX("Npc_Loaded.");
	return S_OK;
}

HRESULT CLoader::Loading_Proto_Static_Map_Objects(const wstring& strPath)
{
	for (auto& p : std::filesystem::directory_iterator(strPath))
	{
		if (p.is_directory())
		{
			Loading_Proto_Static_Map_Objects(p.path().wstring());
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
					CBuilding::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_BUILDING, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_BUILDING, true)))
				{
					return E_FAIL;
				}
			}
			else if ((strFilePath.find(L"Props") != wstring::npos) || (strFilePath.find(L"Prop") != wstring::npos))
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CProbs::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_PROP, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_PROP, true)))
				{
					return E_FAIL;
				}
			}
			else if (strFilePath.find(L"Plants") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CPlants::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_GRASS, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_GRASS, true)))
					return E_FAIL;
			}
			else if (strFilePath.find(L"Grounds") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CGrounds::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_GROUND, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_GROUND, true)))
					return E_FAIL;
			}
			else if (strFilePath.find(L"Rocks And Trees") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CTreeRock::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_TREEROCK, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_TREEROCK, true)))
					return E_FAIL;
			}
		}

	}

	return S_OK;
}

HRESULT CLoader::Loading_Proto_Dynamic_Map_Objects(const wstring& strPath)
{
#pragma region Dog_FootPrints ProtoType
	if (FAILED(GI->Add_Prototype(TEXT("Evermore_Dog_FootPrints"), CDog_FootPrints::Create(m_pDevice, m_pContext, TEXT("Evermore_Dog_FootPrints"), OBJ_TYPE::OBJ_PROP), LAYER_TYPE::LAYER_PROP, true)))
		return E_FAIL;
#pragma endregion
		
#pragma region Animal ProtoType
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Cat"), CCat::Create(m_pDevice, m_pContext, TEXT("Animal_Cat"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Dochi"), CDochi::Create(m_pDevice, m_pContext, TEXT("Animal_Dochi"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_DuckGoo"), CDuckGoo::Create(m_pDevice, m_pContext, TEXT("Animal_DuckGoo"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Fox"), CFox::Create(m_pDevice, m_pContext, TEXT("Animal_Fox"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Rabbit"), CRabbit::Create(m_pDevice, m_pContext, TEXT("Animal_Rabbit"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_PolarBear"), CPolarBear::Create(m_pDevice, m_pContext, TEXT("Animal_PolarBear"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Ermine"), CErmine::Create(m_pDevice, m_pContext, TEXT("Animal_Ermine"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Pigeon"), CPigeon::Create(m_pDevice, m_pContext, TEXT("Animal_Pigeon"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Whale"), CWhale::Create(m_pDevice, m_pContext, TEXT("Animal_Whale"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_WelshCorgi"), CWelshCorgi::Create(m_pDevice, m_pContext, TEXT("Animal_WelshCorgi"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Animal_Ray"), CAnimal_Ray::Create(m_pDevice, m_pContext, TEXT("Animal_Ray"), OBJ_TYPE::OBJ_ANIMAL), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
#pragma endregion Animals Prototype

#pragma region RubyRiding
	if (FAILED(GI->Add_Prototype(TEXT("Prorotype_GameObject_Witch_Ruby_Carriage"), CRubyCarriage::Create(m_pDevice, m_pContext, TEXT("Ruby_Carriage"), OBJ_TYPE::OBJ_DYNAMIC), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
#pragma endregion


#pragma region MiniMap_ProtoType
	if(FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Wall"),
		CWinter_MiniWall::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Wall"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE),LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Stair"),
		CWinter_MiniStair::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Stair"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_GROUND, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Stair"),
		CWinter_MiniBlock::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Block"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Colmn"),
		CWinter_MiniColumn::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Colmn"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Colmn2"),
		CWinter_MiniRevColumn::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Colmn2"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Sculpture"),
		CWinter_Sculpture::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Sculpture"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_SnowMan"),
		CWinter_MiniSnowMan::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_SnowMan"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_BKHouse_01"),
		CWinter_MiniBKHouse::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_BKHouse_01"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_House_01"),
		CWinter_MiniHouse_01::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_House_01"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_House_02"),
		CWinter_MiniHouse_02::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_House_02"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_House_03"),
		CWinter_MiniHouse_03::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_House_03"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_MiniGameMap_Walls"),
		CWinter_MiniWalls::Create(m_pDevice, m_pContext, TEXT("Winter_MiniGameMap_Walls"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Winter_CurlingFloor"),
		CWinter_CurlingFloor::Create(m_pDevice, m_pContext, TEXT("Winter_CurlingFloor"), OBJ_TYPE::OBJ_MINIGAME_STRUCTURE), LAYER_TYPE::LAYER_GROUND, true)))
		return E_FAIL;
#pragma endregion



#pragma region Sky
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_SkySun"), CSun::Create(m_pDevice, m_pContext, TEXT("Sky_Sun"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Skydome"), CSkyDome::Create(m_pDevice, m_pContext, TEXT("Sky_dome"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Skydome2"), CSkyDome::Create(m_pDevice, m_pContext, TEXT("Sky_dome2"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Skydome3"), CSkyDome::Create(m_pDevice, m_pContext, TEXT("Sky_dome3"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_SkyPlane"), CSkyPlane::Create(m_pDevice, m_pContext, TEXT("Sky_Plane"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_SkyPlane2"), CSkyPlane::Create(m_pDevice, m_pContext, TEXT("Sky_Plane2"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
#pragma endregion Sky

#pragma region Water
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Water"), CWater::Create(m_pDevice, m_pContext, TEXT("Evermore_Water"), OBJ_TYPE::OBJ_WATER), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_WInter_Water"), CWinterWater::Create(m_pDevice, m_pContext, TEXT("Winter_Water"), OBJ_TYPE::OBJ_WATER), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_EvermoreWater"), CEvermoreWater::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_DYNAMIC, true)))
		return E_FAIL;
#pragma endregion Water

#pragma region Aurora
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Aurora"), CAurora::Create(m_pDevice, m_pContext, TEXT("Sky_Aurora"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Aurora_Reverse"), CAurora::Create(m_pDevice, m_pContext, TEXT("Sky_Aurora_Reverse"), OBJ_TYPE::OBJ_SKY), LAYER_TYPE::LAYER_SKYBOX, true)))
		return E_FAIL;
#pragma endregion Aurora

#pragma region WitchDynamic
	if (FAILED(GI->Add_Prototype(TEXT("Witch_Wood_Wall"), CWitchWood::Create(m_pDevice, m_pContext, TEXT("Witch_Wood_Wall"), OBJ_TYPE::OBJ_BUILDING), LAYER_TYPE::LAYER_BUILDING, true)))
		return E_FAIL;
#pragma endregion


	
#pragma region Animal
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cat", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Cat/", L"Animal_Cat")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Dochi", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Dochi/", L"Animal_Dochi")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DuckGoo", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Duckgoo/", L"Animal_Duckgoo")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Fox", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Fox/", L"Animal_Fox")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Rabbit", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Rabbit/", L"Animal_Rabbit")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_PolarBear", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/PolarBear/", L"Animal_PolarBear")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Ermine", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Ermine/", L"Animal_Ermine")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Pigeon", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Pigeon/", L"Animal_Pigeon")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Whale", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Whale/", L"Animal_Whale")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_WelshCorgi", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/WelshCorgi/", L"Animal_WelshCorgi")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Animal_Ray", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/Animal/Ray/", L"Animal_Ray")))
		return E_FAIL;
#pragma endregion Animal

#pragma region Ruby Rigding
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Ruby_Carriage", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/NPC_Riding/", L"Ruby_Carriage")))
		return E_FAIL;
#pragma endregion


#pragma region Water
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Everemore_Water", CModel::TYPE_NONANIM, L"../Bin/Export/AnimModel/Map/Water/", L"Ocean_Tesselation")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Entire_Water", CModel::TYPE_NONANIM, L"../Bin/Export/AnimModel/Map/Water/", L"Entire_Water")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Evermore_CircleWater", CModel::TYPE_NONANIM, L"../Bin/Export/AnimModel/Map/Water/", L"Evermore_WaterA_01")))
		return E_FAIL;
#pragma endregion Water

#pragma region Sky
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Sky_dome", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/SkyDom/", L"Skydome")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Sky_dome2", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/SkyDom/", L"Skydome2")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Aurora", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/SkyDom/", L"AuroraTesselatedPlane")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SkySun", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/SkyDom/", L"Sun")))
		return E_FAIL;
#pragma endregion Sky

#pragma region MiniGameMap_ProtoType
	// TODO 쓸 FBX 다 뽑고.
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Wall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Wall")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Stair", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Stair")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Block", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Block")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Colmn", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Colmn")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Colmn2", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Colmn2")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Sculpture", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Sculpture")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_SnowMan", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_SnowMan")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_BKHouse_01", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_BKHouse_01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_House_01", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_House_01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_House_02", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_House_02")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_House_03", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_House_03")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_MiniGameMap_Walls", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_MiniGameMap_Walls")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Winter_CurlingFloor", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/MiniGame/", L"Winter_CurlingFloor")))
		return E_FAIL;
#pragma endregion

#pragma region Witch_Dynamic
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Witch_Wood", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Map/WitchForestTree_Anim/", L"Witch_LairVineWall")))
		return E_FAIL;
#pragma endregion



	return S_OK;
}

HRESULT CLoader::Loading_For_Character(CHARACTER_TYPE eCharacterType)
{
	if (eCharacterType >= CHARACTER_TYPE::CHARACTER_END)
		return E_FAIL;
	
	if (eCharacterType == CHARACTER_TYPE::SWORD_MAN)
	{
		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Dummy", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/SwordMan/Dummy/", L"SwordMan_Dummy")))
			return E_FAIL;

		if (FAILED(Loading_Character_Models(L"../Bin/Export/AnimModel/Character/SwordMan/")))
			return E_FAIL;
	}

	else if (eCharacterType == CHARACTER_TYPE::ENGINEER)
	{

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Engineer_BurstSkill_Cannon", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Skill/", L"Engineer_BurstSkill_Cannon")))
			return E_FAIL;

		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Engineer_BurstSkill_CannonBomb", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/TowerDefence/Projectile/", L"Cannon_Ball")))
			return E_FAIL;
		
		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Engineer_Dummy", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Engineer/Dummy/", L"Engineer_Dummy")))
			return E_FAIL;

		if (FAILED(Loading_Character_Models(L"../Bin/Export/AnimModel/Character/Engineer/")))
			return E_FAIL;
	}
	else if (eCharacterType == CHARACTER_TYPE::DESTROYER)
	{
		if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Destroyer_Dummy", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Character/Destroyer/Dummy/", L"Destroyer_Dummy")))
			return E_FAIL;

		if (FAILED(Loading_Character_Models(L"../Bin/Export/AnimModel/Character/Destroyer/")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Character_Models(const wstring& strFolderPath)
{
	for (auto& p : std::filesystem::directory_iterator(strFolderPath))
	{
		if (p.is_directory())
			Loading_Character_Models(p.path().wstring());

		wstring strFilePath = CUtils::PathToWString(p.path().wstring());



		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];



		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (strFilePath.find(L"Dummy") != wstring::npos)
			continue;

		if (true == !lstrcmp(strExt, L".mesh"))
		{
			wstring strPrototypeName = L"Prototype_Component_Model_";
			strPrototypeName += strFileName;
			if (FAILED(GI->Import_Model_Data(LEVEL_PARTS, strPrototypeName, CModel::TYPE_ANIM, strFolderName, strFileName)))
				return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT CLoader::Reserve_Character_Managers()
{

	if (FAILED(CWeapon_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CCharacter_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_TowerDefence()
{
	if (FAILED(CTowerDefence_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
	{
		MSG_BOX("Loading_For_TowerDefence Failed.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Proto_Monster_Npc()
{
	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp = 100;
	statDesc.fMaxMp = 100;

	/* Quest */
	// 임시로 몬스터에 담는다. 충돌 처리 그룹 추가 될 때까지.
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Quest_DestSpot", CQuest_DestSpot::Create(m_pDevice, m_pContext, TEXT("Quest_DestSpot")), LAYER_ETC, true)))
		return E_FAIL;

	/* Monster */
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix", CGlanix::Create(m_pDevice, m_pContext, TEXT("Glanix"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_IcePillar", CGlanix_IcePillar::Create(m_pDevice, m_pContext, TEXT("Clanix_IcePillar")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_Phoenix", CGlanix_Phoenix::Create(m_pDevice, m_pContext, TEXT("Glanix_Phoenix")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_ShockWave", CGlanix_ShockWave::Create(m_pDevice, m_pContext, TEXT("Glanix_ShockWave")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_GlanixIceBall", CGlanix_IceBall::Create(m_pDevice, m_pContext, TEXT("Glanix_IceBall")), LAYER_PROP, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Glanix_GlanixIcicle", CGlanix_Icicle::Create(m_pDevice, m_pContext, TEXT("Glanix_Icicle")), LAYER_PROP, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Spawner_Ice01", CSpawner_Ice01::Create(m_pDevice, m_pContext, TEXT("Spawner_Ice01")), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Spawner_Ice02", CSpawner_Ice02::Create(m_pDevice, m_pContext, TEXT("Spawner_Ice02")), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Spawner_Ice03", CSpawner_Ice03::Create(m_pDevice, m_pContext, TEXT("Spawner_Ice03")), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Water", CBaobam_Water::Create(m_pDevice, m_pContext, TEXT("Baobam_Water"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Shadow_Thief", CShadow_Thief::Create(m_pDevice, m_pContext, TEXT("Shadow_Thief"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_IceBearMan", CIceBearMan::Create(m_pDevice, m_pContext, TEXT("IceBearMan"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia", CStellia::Create(m_pDevice, m_pContext, TEXT("Stellia"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia_Crystal", CStellia_Crystal::Create(m_pDevice, m_pContext, TEXT("Stellia_Crystal")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia_Crystal_Destructible", CStellia_Crystal_Destructible::Create(m_pDevice, m_pContext, TEXT("Stellia_Crystal_Destructible"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia_Crystal_Explosion", CStellia_Crystal_Explosion::Create(m_pDevice, m_pContext, TEXT("Stellia_Crystal_Explosion")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia_Explosion", CStellia_Explosion::Create(m_pDevice, m_pContext, TEXT("Stellia_Explosion")), LAYER_PROP, true)))
		return E_FAIL;

	//if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_DreamerMazeWitch", CDMWitch::Create(m_pDevice, m_pContext, TEXT("DreamerMazeWitch"), statDesc), LAYER_MONSTER, true)))
	//	return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Spawner_Witch01", CSpawner_Witch01::Create(m_pDevice, m_pContext, TEXT("Spawner_Witch01")), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_PumpkinCandle", CPumpkinCandle::Create(m_pDevice, m_pContext, TEXT("PumpkinCandle"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown", CClown::Create(m_pDevice, m_pContext, TEXT("Clown"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown_Wizard", CClown_Wizard::Create(m_pDevice, m_pContext, TEXT("Clown_Wizard"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Clown_Wizard_DarkBall", CClown_Wizard_DarkBall::Create(m_pDevice, m_pContext, TEXT("Clown_Wizard_DarkBall")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Baobam_Dark", CBaobam_Dark::Create(m_pDevice, m_pContext, TEXT("Baobam_Dark"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Criminal_Monster", CCriminal_Monster::Create(m_pDevice, m_pContext, TEXT("Criminal_Monster"), statDesc), LAYER_MONSTER, true)))
		return E_FAIL;

	// NPC
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Criminal_Npc", CCriminal_Npc::Create(m_pDevice, m_pContext, TEXT("Criminal")), LAYER_NPC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_DreamMazeWitch_Npc", CDreamMazeWitch_Npc::Create(m_pDevice, m_pContext, TEXT("DreamMazeWitch")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Witch_BlackHole", CWitch_BlackHole::Create(m_pDevice, m_pContext, TEXT("Witch_BlackHole")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Witch_Rage02Sphere", CWitch_Rage02Sphere::Create(m_pDevice, m_pContext, TEXT("Witch_Rage02Sphere")), LAYER_PROP, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Witch_Laser", CWitch_Laser::Create(m_pDevice, m_pContext, TEXT("Witch_Laser")), LAYER_PROP, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Engineer_Npc", CEngineer_Npc::Create(m_pDevice, m_pContext, TEXT("Engineer_Npc")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Destroyer_Npc", CDestroyer_Npc::Create(m_pDevice, m_pContext, TEXT("Destroyer_Npc")), LAYER_NPC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFAT01", CHumanFAT01::Create(m_pDevice, m_pContext, TEXT("HumanFAT01")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_MouseFolkFat01", CMouseFolkFat01::Create(m_pDevice, m_pContext, TEXT("MouseFolkFat01")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL04", CHumanFL04::Create(m_pDevice, m_pContext, TEXT("HumanFL04")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL05", CHumanFL05::Create(m_pDevice, m_pContext, TEXT("HumanFL05")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFL07", CHumanFL07::Create(m_pDevice, m_pContext, TEXT("HumanFL07")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFM02", CHumanFM02::Create(m_pDevice, m_pContext, TEXT("HumanFM02")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFM09", CHumanFM09::Create(m_pDevice, m_pContext, TEXT("HumanFM09")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GrimalkinOld01", CGrimalkinOld01::Create(m_pDevice, m_pContext, TEXT("GrimalkinOld01")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFLCapitalMerchant", CHumanFLCapitalMerchant::Create(m_pDevice, m_pContext, TEXT("HumanFLCapitalMerchant")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_KingdomGuard", CKingdomGuard::Create(m_pDevice, m_pContext, TEXT("KingdomGuard")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_NpcWeapon_Halberd", CNpcWeapon_Halberd::Create(m_pDevice, m_pContext, TEXT("NpcWeapon_Halberd")), LAYER_WEAPON, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanML04", CHumanML04::Create(m_pDevice, m_pContext, TEXT("HumanML04")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanML12", CHumanML12::Create(m_pDevice, m_pContext, TEXT("HumanML12")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GrimalKinML03", CGrimalKinML03::Create(m_pDevice, m_pContext, TEXT("GrimalKinML03")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GrimalKinML04", CGrimalKinML04::Create(m_pDevice, m_pContext, TEXT("GrimalKinML04")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMM03", CHumanMM03::Create(m_pDevice, m_pContext, TEXT("HumanMM03")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMM05", CHumanMM05::Create(m_pDevice, m_pContext, TEXT("HumanMM05")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMM18", CHumanMM18::Create(m_pDevice, m_pContext, TEXT("HumanMM18")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMM21", CHumanMM21::Create(m_pDevice, m_pContext, TEXT("HumanMM21")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_LuxerionHuman", CLuxerionHuman::Create(m_pDevice, m_pContext, TEXT("LuxerionHuman")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChild01", CHumanChild01::Create(m_pDevice, m_pContext, TEXT("HumanChild01")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChild02", CHumanChild02::Create(m_pDevice, m_pContext, TEXT("HumanChild02")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerCat", CSeekerCat::Create(m_pDevice, m_pContext, TEXT("SeekerCat")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerKing", CSeekerKing::Create(m_pDevice, m_pContext, TEXT("SeekerKing")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SeekerObserver", CSeekerObserver::Create(m_pDevice, m_pContext, TEXT("SeekerObserver")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ruslan", CRuslan::Create(m_pDevice, m_pContext, TEXT("Ruslan")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFS03", CHumanFS03::Create(m_pDevice, m_pContext, TEXT("HumanFS03")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_SwiftSolutionMaster", CSwiftSolutionMaster::Create(m_pDevice, m_pContext, TEXT("SwiftSolutionMaster")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Zehra", CZehra::Create(m_pDevice, m_pContext, TEXT("Zehra")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Enbi", CEnbi::Create(m_pDevice, m_pContext, TEXT("Enbi")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Edellian", CEdellian::Create(m_pDevice, m_pContext, TEXT("Edellian")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Chloe", CChloe::Create(m_pDevice, m_pContext, TEXT("Chloe")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Aren", CAren::Create(m_pDevice, m_pContext, TEXT("Aren")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_BlackSmithMaster", CBlackSmithMaster::Create(m_pDevice, m_pContext, TEXT("BlackSmithMaster")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GrimalKinML01", CGrimalKinML01::Create(m_pDevice, m_pContext, TEXT("GrimalKinML01")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GrimalKinML02", CGrimalKinML02::Create(m_pDevice, m_pContext, TEXT("GrimalKinML02")), LAYER_NPC, true)))
		return E_FAIL;		
	
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChildHalloweenA", CHumanChildHalloweenA::Create(m_pDevice, m_pContext, TEXT("HumanChildHalloweenA")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanChildHalloweenB", CHumanChildHalloweenB::Create(m_pDevice, m_pContext, TEXT("HumanChildHalloweenB")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFM12 ", CHumanFM12::Create(m_pDevice, m_pContext, TEXT("HumanFM12")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_CaliaHuman", CCaliaHuman::Create(m_pDevice, m_pContext, TEXT("CaliaHuman")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanMSCrossFieldMerchant", CHumanMSCrossFieldMerchant::Create(m_pDevice, m_pContext, TEXT("HumanMSCrossFieldMerchant")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_RunnerCat", CRunnerCat::Create(m_pDevice, m_pContext, TEXT("RunnerCat")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_ThiefCat", CThiefCat::Create(m_pDevice, m_pContext, TEXT("ThiefCat")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ghost1", CGhost1::Create(m_pDevice, m_pContext, TEXT("Ghost1")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ghost2", CGhost2::Create(m_pDevice, m_pContext, TEXT("Ghost2")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Ruby", CRuby::Create(m_pDevice, m_pContext, TEXT("Ruby")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_HumanFSPioneer", CHumanFSPioneer::Create(m_pDevice, m_pContext, TEXT("HumanFSPioneer")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_TreeGrandfa", CTreeGrandfa::Create(m_pDevice, m_pContext, TEXT("TreeGrandfa")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Verde", CVerde::Create(m_pDevice, m_pContext, TEXT("Verde")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Gosling", CGosling::Create(m_pDevice, m_pContext, TEXT("Gosling")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Cyan", CCyan::Create(m_pDevice, m_pContext, TEXT("Cyan")), LAYER_NPC, true)))
		return E_FAIL;
	
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_FunyaSnowman", CFunyaSnowman::Create(m_pDevice, m_pContext, TEXT("FunyaSnowman")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_GiftFunyaSnowman", CGiftFunyaSnowman::Create(m_pDevice, m_pContext, TEXT("GiftFunyaSnowman")), LAYER_NPC, true)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_AquarisBella", CAquarisBella::Create(m_pDevice, m_pContext, TEXT("AquarisBella")), LAYER_NPC, true)))
		return E_FAIL;
	
	/* Monster */
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Stellia", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Stellia/", L"Stellia")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Stellia_Crystal", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Monster/Witch/StelliaCrystal/", L"StelliaCrystal")))
		return E_FAIL;
	/*Texture*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Stellia_Crystals"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Monster/StelliaCrystal/T_GachaCommonCrystal%d_Glass.png"), 3))))
		return E_FAIL;
	/**/

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Glanix", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/Glanix/", L"Glanix")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GlanixPillar", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Monster/GlanixPillar/", L"GlanixPillar")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Phoenix", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/Phoenix/", L"Phoenix")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GlanixIceBall", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Monster/GlanixIceBall/", L"GlanixIceBall")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GlanixIcicle_0", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Monster/GlanixIcicle/", L"Glanix_Icicle_0")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GlanixIcicle_1", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Monster/GlanixIcicle/", L"Glanix_Icicle_1")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DreamerMazeWitch", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/DreamerMazeWitch/", L"DreamerMazeWitch")))
		return E_FAIL;

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
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Criminal", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/Criminal/", L"Criminal")))
		return E_FAIL;
	//if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_DreamMazeWitch", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Boss/DreamerMazeWitch/", L"DreamerMazeWitch")))
	//	return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFAT01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFAT01/", L"HumanFAT01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_MouseFolkFat01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/MouseFolkFat01/", L"MouseFolkFat01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL04", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL04/", L"HumanFL04")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL05", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL05/", L"HumanFL05")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFL07", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFL07/", L"HumanFL07")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFM02", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFM02/", L"HumanFM02")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFM09", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFM09/", L"HumanFM09")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrimalkinOld01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/GrimalkinOld01/", L"GrimalkinOld01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFLCapitalMerchant", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanFLCapitalMerchant/", L"HumanFLCapitalMerchant")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_KingdomGuard", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/KingdomGuard/", L"KingdomGuard")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_NpcWeapon_Halberd", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Npc_Weapon/NpcWeapon_Halberd/", L"NpcWeapon_Halberd")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanML04", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanML04/", L"HumanML04")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanML12", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanML12/", L"HumanML12")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrimalKinML03", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/GrimalKinML03/", L"GrimalKinML03")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrimalKinML04", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/GrimalKinML04/", L"GrimalKinML04")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMM03", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanMM03/", L"HumanMM03")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMM05", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanMM05/", L"HumanMM05")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMM18", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanMM18/", L"HumanMM18")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanMM21", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/HumanMM21/", L"HumanMM21")))
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
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_BlackSmithMaster", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/BlackSmithMaster/", L"BlackSmithMaster")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrimalKinML01", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/GrimalKinML01/", L"GrimalKinML01")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GrimalKinML02", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/KingDom/GrimalKinML02/", L"GrimalKinML02")))
		return E_FAIL;
	
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChildHalloweenA", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanChildHalloweenA/", L"HumanChildHalloweenA")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanChildHalloweenB", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanChildHalloweenB/", L"HumanChildHalloweenB")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_HumanFM12", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/HumanFM12/", L"HumanFM12")))
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
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cyan", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Witch/Cyan/", L"Cyan")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_FunyaSnowman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/FunyaSnowman/", L"FunyaSnowman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_GiftFunyaSnowman", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/GiftFunyaSnowman/", L"GiftFunyaSnowman")))
		return E_FAIL;
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_AquarisBella", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/NPC/Ice/AquarisBella/", L"AquarisBella")))
		return E_FAIL;

	return S_OK;
}
HRESULT CLoader::Loading_Proto_Vehicles()
{
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Udadak", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Vehicle/Udadak/", L"Udadak")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Biplane", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Vehicle/Biplane/", L"Biplane")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Boto", CModel::TYPE_ANIM, L"../Bin/Export/AnimModel/Vehicle/Boto/", L"Boto")))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Grandprix_Enemy_Ghost2",
		CGrandprix_Enemy_Ghost2::Create(m_pDevice, m_pContext, TEXT("Grandprix_Enemy_Ghost2")), LAYER_TYPE::LAYER_ETC)))
		return E_FAIL;
	
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Biplane_GuidedMissile", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/MiniGame/Missile/", L"Biplane_GuidedMissile")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Bullet_Blue", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Grandprix/Bullets/", L"Bullet_Blue")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Bullet_Orange", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Grandprix/Bullets/", L"Bullet_Orange")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Bullet_Ball", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Grandprix/Bullets/", L"Bullet_Ball")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Grandprix_ItemBox", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Grandprix/Granprix_ItemBox/", L"Grandprix_ItemBox")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Grandprix_SiriusRelic", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Grandprix/SiriusRelic/", L"SiriusRelic")))
		return E_FAIL;


	return S_OK;
}
/*===================================================================*/

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