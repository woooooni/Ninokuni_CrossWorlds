#include "stdafx.h"
#include "Level_Evermore.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"
#include "Game_Manager.h"
#include "Player.h"

#include "Utils.h"
#include <FileUtils.h>

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UIMinigame_Manager.h"

#include "Water.h"
#include "Light.h"

#include "Camera_Group.h"

#include "Quest_Manager.h"

#include "GameNpc.h"
#include "Animals.h"

#include "Portal.h"
#include "Trigger.h"
#include "Respawn_Box.h"

#include "Particle_Manager.h"
#include "Riding_Manager.h"
#include "Inventory_Manager.h"
#include "Grandprix_Manager.h"
#include "Monster.h"

_bool CLevel_Evermore::g_bFirstEnter = false;

CLevel_Evermore::CLevel_Evermore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Evermore::Initialize()
{	
	SetWindowText(g_hWnd, TEXT("Ni no Kuni : Cross Worlds"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;
//
	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;
//
//	if (FAILED(Ready_Layer_Terrain(LAYER_TYPE::LAYER_TERRAIN)))
//		return E_FAIL;
//	
	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Npc(LAYER_TYPE::LAYER_NPC)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Prop(LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(Ready_Trigger(TEXT("Evermore"))))
		return E_FAIL;

	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
	{
		if (FAILED(Ready_Layer_Monster(TEXT("Evermore"))))
			return E_FAIL;
	}

	if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, TEXT("Evermore"))))
		return E_FAIL;

	if (FAILED(Ready_Light(TEXT("Evermore Light"))))
		return E_FAIL;

	/* 쿠우 타겟 설정 */
	CGame_Manager::GetInstance()->Set_KuuTarget_Player();

	/* 퀘스트 세팅 */
	if (CQuest_Manager::GetInstance()->Get_IsReserve() == false)
	{
		if (FAILED(CQuest_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
			return E_FAIL;
		CQuest_Manager::GetInstance()->Set_Running(true);
	}

	/* 페이드 설정  */
	if (CQuest_Manager::QUESTEVENT_ENDING != CQuest_Manager::GetInstance()->Get_CurQuestEvent() && g_iStartQuestLevel != QUEST_LEVEL::QL_9_ENDING)
	{	
		if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 5.f);
	}
	else
	{
		/* 엔딩 퀘스트라면 하얀색으로 짧게 진행 */
		if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f, true);
	}

	/* Door Camera Action (엔딩 퀘스트에서는 사용하지 않는다.) */
	if (CQuest_Manager::QUESTEVENT_ENDING != CQuest_Manager::GetInstance()->Get_CurQuestEvent() && g_iStartQuestLevel != QUEST_LEVEL::QL_9_ENDING)
	{
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());
		if (nullptr != pActionCam)
		{
			pActionCam->Start_Action_Door();
		}
	}

	/* 구역 이름 (엔딩 퀘스트에서는 사용하지 않는다.) */
	if (CQuest_Manager::QUESTEVENT_ENDING != CQuest_Manager::GetInstance()->Get_CurQuestEvent() && g_iStartQuestLevel != QUEST_LEVEL::QL_9_ENDING)
	{
		if (false == g_bFirstEnter)
		{
			g_bFirstEnter = true;
			CUI_Manager::GetInstance()->OnOff_MapName(true, TEXT("남문 광장"));

			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_HideAndSeek_HSClearMachine_Spawn_1_St.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
		}
	}

	/* Set Fog */
	{
		CRenderer::FOG_DESC fogDesc;
		::ZeroMemory(&fogDesc, sizeof(fogDesc));
		{
			fogDesc.fFogDistanceValue = 30.0f;
			fogDesc.fFogHeightValue = 50.0f;
			fogDesc.fFogStartDepth = 100.0f;
			fogDesc.fFogStartDistance = 17.150f;
			fogDesc.fFogDistanceDensity = 0.04f;
			fogDesc.fFogHeightDensity = 0.06f;
		}
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogDesc(fogDesc);
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogColor(Vec4(0.396f, 0.744f, 1.0f, 1.0f));
	}

	/* Set Bgm (엔딩 퀘스트에서 에버모어 입장시 다른 BGM 실행 */
	if (CQuest_Manager::QUESTEVENT_ENDING != CQuest_Manager::GetInstance()->Get_CurQuestEvent() && g_iStartQuestLevel != QUEST_LEVEL::QL_9_ENDING)
	{
		GI->Play_BGM(TEXT("BGM_Town_Evermore_Normal_Castle_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);
	}
	else
	{
		GI->Play_BGM(TEXT("BGM_Town_Evermore_1stAnniversary_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);
	}
	return S_OK;
}

HRESULT CLevel_Evermore::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_Fade(fTimeDelta);
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_EVERMORE, fTimeDelta);
	CUIMinigame_Manager::GetInstance()->Tick_Minigame(LEVELID::LEVEL_EVERMORE, fTimeDelta);
	CGrandprix_Manager::GetInstance()->Tick(fTimeDelta);

	//if (KEY_TAP(KEY::PAGE_UP))
	//{
	//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	//}

	//if (KEY_TAP(KEY::PAGE_DOWN))
	//{
	//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	//}

	//if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
	//{
	//	GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_ICELAND, L"Winter"));
	//}

	//if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET))
	//{
	//	GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_KINGDOMHALL, L"Kingdom"));
	//}

	//if (KEY_TAP(KEY::F8))
	//{
	//	GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia"));
	//}

	return S_OK;
}

HRESULT CLevel_Evermore::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_Fade(fTimeDelta);
	CUI_Manager::GetInstance()->LateTick_GamePlayLevel(fTimeDelta);
	CUIMinigame_Manager::GetInstance()->LateTick_Minigame(LEVELID::LEVEL_EVERMORE, fTimeDelta);
	CGrandprix_Manager::GetInstance()->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Evermore::Render_Debug()
{
	return S_OK;
}

HRESULT CLevel_Evermore::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_Evermore::Exit_Level()
{
	GI->Clear_PhysX_Ground();
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	list<CGameObject*>& Grounds = GI->Find_GameObjects(LEVEL_EVERMORE, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}


	list<CGameObject*>& Buildings = GI->Find_GameObjects(LEVEL_EVERMORE, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}



	list<CGameObject*>& Props = GI->Find_GameObjects(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP);
	for (auto& Prop : Props)
	{
		CModel* pModel = Prop->Get_Component<CModel>(L"Com_Model");
		if (nullptr == pModel)
			continue;

		if (pModel->Get_Name().find(L"Evermore_Prob_03") != wstring::npos)
		{
			if (FAILED(GI->Add_Building(Prop,
				Prop->Get_Component<CModel>(L"Com_Model"),
				Prop->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
			{
				MSG_BOX("피직스 계단 생성에 실패했습니다.");
			}
		}
	}
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{	
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{

	if (false == g_bFirstEnter)
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
	}
	

	/* Set Camera */
	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			pFollowCam->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
			pFollowCam->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
			pFollowCam->Set_Default_Position();
		}
	}


	CGameObject* pDoor = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Door_Enter"), nullptr, &pDoor)))
		return E_FAIL;

	if (nullptr == pDoor)
		return E_FAIL;

	CTransform* pDoorTransform = pDoor->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pDoorTransform)
		return E_FAIL;

	CTransform* pCharacterTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pCharacterTransform)
		return E_FAIL;

	Vec3 vScale = pDoorTransform->Get_Scale();
	pDoorTransform->Set_WorldMatrix(pCharacterTransform->Get_WorldMatrix());
	Vec4 vPosition = pDoorTransform->Get_Position() + (pDoorTransform->Get_Look() * -1.f);
	pDoorTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	pDoorTransform->Set_Scale(vScale);

	if (false == g_bFirstEnter)
	{
		if (FAILED(CRiding_Manager::GetInstance()->Ready_Vehicle_GameObject(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;

		if (FAILED(CGrandprix_Manager::GetInstance()->Prepare_Grandprix()))
			return E_FAIL;

		/* 처음 에버모어 들어갈 때 문 살짝 내려가있는 현상 방지 */
		Vec3 vDoorPos = pDoorTransform->Get_Position();
		vDoorPos.y += 0.1f;
		pDoorTransform->Set_Position(vDoorPos);
	}
	else
	{
		if (FAILED(CRiding_Manager::GetInstance()->Ready_Vehicle_GameObjectToLayer(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;

		if (FAILED(CGrandprix_Manager::GetInstance()->Ready_Grandprix_GameObjectToLayer(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Monster(const wstring& strMonsterFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMonsterFileName + L"/" + strMonsterFileName + L"Monster.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVEL_TOOL, LAYER_TYPE::LAYER_MONSTER);
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

		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_MONSTER, strPrototypeTag, &Init_Data, &pObj)))
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

		_bool bInvastion = static_cast<CMonster*>(pObj)->Get_Invasition();
		if (true == bInvastion)
		{
			_uint invasionState;
			File->Read<_uint>(invasionState);
			static_cast<CMonster*>(pObj)->Set_InvasionState(static_cast<CMonster::MONSTER_INVASION_STATE>(invasionState));
		}
	}

	//MSG_BOX("Monster_Loaded.");
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Npc(const LAYER_TYPE eLayerType)
{
	wstring strNpcFileName = L"Evermore";
	wstring strMapFilePath = L"";

	if(CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
		strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC_Invasion.map";
	else if(CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_ENDING)
		strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC_Ending.map";
	else
		strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_EVERMORE, i);


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
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_EVERMORE, i, strPrototypeTag, &Init_Data, &pObj)))
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

			CPhysX_Controller* pController = pObj->Get_Component<CPhysX_Controller>(L"Com_Controller");

			if (nullptr != pController)
				pController->Set_EnterLevel_Position(pTransform->Get_Position());
		}

	}

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (false == g_bFirstEnter)
	{
		if (FAILED(CUI_Manager::GetInstance()->Ready_GameObject(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;
		if (FAILED(CUIMinigame_Manager::GetInstance()->Ready_MinigameUI_GameObject(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(CUI_Manager::GetInstance()->Ready_GameObjectToLayer(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;
		if (FAILED(CUIMinigame_Manager::GetInstance()->Ready_MinigameUI_ToLayer(LEVELID::LEVEL_EVERMORE)))
			return E_FAIL;
	}

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return E_FAIL;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return E_FAIL;

	CHARACTER_TYPE eCharacterType = pCharacter->Get_CharacterType();
	ELEMENTAL_TYPE eElementalType = pCharacter->Get_ElementalType();

	CUI_Manager::GetInstance()->Ready_CharacterTypeForUI(eCharacterType);
	CUI_Manager::GetInstance()->Ready_ElementalTypeForUI(eElementalType);

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	_matrix WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[3] = XMVectorSet(0.f, 30.f, 60.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_EVERMORE, TEXT("Particle_Leaf"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Prop(const LAYER_TYPE eLayerType)
{
	// Evermore -> Kingdom
	CPortal::PORTAL_DESC PortalInfo = {};
	PortalInfo.vStartPosition = XMVectorSet(-0.35f, 9.9f, 145.f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(0.f, 0.05f, -3.f, 1.f);

	PortalInfo.eCurrentLevel = LEVEL_EVERMORE;
	PortalInfo.eNextLevel = LEVEL_KINGDOMHALL;
	PortalInfo.vEffectScale = Vec3(5.f, 0.3f, 1.f);

	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo)))
		return E_FAIL;


	// Evermore -> IceLand
	CGameObject* pPortal = nullptr;
	PortalInfo.vStartPosition = XMVectorSet(-50.5f, 9.9f, 162.f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(127.3f, -0.675f, 11.4f, 1.f);
	PortalInfo.vNextRotation = { 0.f, -90.f, 0.f };

	PortalInfo.eCurrentLevel = LEVEL_EVERMORE;
	PortalInfo.eNextLevel = LEVEL_ICELAND;
	PortalInfo.vEffectScale = Vec3(5.f, 0.3f, 1.f);

	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;

	if (nullptr == pPortal)
		return E_FAIL;

	CTransform* pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));


	// Evermore -> WitchForest
	PortalInfo.vStartPosition = XMVectorSet(127.94f, -0.015f, 111.5f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(-2.92f, 2.262f, -89.727f, 1.f);
	PortalInfo.vNextRotation = { 0.f, -45.f, 0.f };

	PortalInfo.eCurrentLevel = LEVEL_EVERMORE;
	PortalInfo.eNextLevel = LEVEL_WITCHFOREST;
	PortalInfo.vEffectScale = Vec3(5.f, 0.3f, 1.f);

	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;

	if (nullptr == pPortal)
		return E_FAIL;

	pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));




	// Triggers.
	//CTrigger::TRIGGER_DESC TriggerDesc;
	//TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_MAP_NAME;
	//TriggerDesc.strMapName = TEXT("남문 광장");
	//TriggerDesc.vStartPosition = { 0.f, -20.f, 0.f, 1.f };
	//TriggerDesc.vExtents = { 50.f, 50.f, 150.f };
	//TriggerDesc.vAt = Vec4(0.0f,0.0f,0.0f,1.0f);
	//TriggerDesc.vEye = Vec4(7.0f, 100.0f, 0.0f, 1.0f);

	//if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;

	//TriggerDesc.strMapName = TEXT("서문 광장");
	//TriggerDesc.vStartPosition = { -85.5f, -20.f, 60.6f, 1.f };
	//TriggerDesc.vExtents = { 50.f, 50.f, 150.f };
	//TriggerDesc.vAt = Vec4(0.0f, -574.0f, 0.0f, 1.0f);
	//TriggerDesc.vEye = Vec4(-85.0f, 100.0f, 0.0f, 1.0f);
	//if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;

	//

	//TriggerDesc.strMapName = TEXT("동문 광장");
	//TriggerDesc.vStartPosition = { 88.85f, -20.f, 60.6f, 1.f };
	//TriggerDesc.vExtents = { 50.f, 50.f, 150.f };
	//if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;



	CRespawn_Box::RESPAWN_DESC RespawnDesc = {};
	RespawnDesc.vStartPosition = Vec4(0.f, -100.f, 0.f, 1.f);
	RespawnDesc.vRespawnPosition = Vec4(0.f, 0.f, 0.f, 1.f);
	RespawnDesc.vExtents = Vec3(1000.f, 5.f, 1000.f);

	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_RespawnBox"), &RespawnDesc)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVELID::LEVEL_EVERMORE, LAYER_TYPE::LAYER_DYNAMIC);

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

		_uint objectType;
		File->Read<_uint>(objectType);


		OBJECT_INIT_DESC Init_Data = {};
		Init_Data.vStartPosition = vPos;
		CGameObject* pObj = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_DYNAMIC, strPrototypeTag, &Init_Data, &pObj)))
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

		if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_WATER)
		{
			CWater::VS_GerstnerWave vsWave;
			File->Read<CWater::VS_GerstnerWave>(vsWave);
			CWater::PS_GerstnerWave psWave;
			File->Read<CWater::PS_GerstnerWave>(psWave);
			_float damp;
			File->Read<_float>(damp);

			static_cast<CWater*>(pObj)->Set_VSGerstnerWave(vsWave);
			static_cast<CWater*>(pObj)->Set_PSGerstnerWave(psWave);
			static_cast<CWater*>(pObj)->Set_Damper(damp);
		}
		else if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
		{
			_uint iSize;
			File->Read<_uint>(iSize);

			if (iSize != 0)
			{
				CAnimals* pAnimals = static_cast<CAnimals*>(pObj);
				vector<Vec4> Points;
				Points.reserve(iSize);

				for (_uint i = 0; i < iSize; ++i)
				{
					Vec4 vPoint;
					File->Read<Vec4>(vPoint);
					Points.push_back(vPoint);
				}

				pAnimals->Set_RomingPoints(Points);

				_float* pSpeed = pAnimals->Get_Speed();
				File->Read<_float>(*pSpeed); // 0

				vPos = Points.front();
			}
		}

		pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
		pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
		pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
		pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

		CPhysX_Controller* pController = pObj->Get_Component<CPhysX_Controller>(L"Com_Controller");

		if (nullptr != pController)
			pController->Set_EnterLevel_Position(pTransform->Get_Position());
	}

	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
	{
		list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVELID::LEVEL_EVERMORE, LAYER_TYPE::LAYER_DYNAMIC);

		for (auto& pAnimal : pGameObjects)
		{
			if (pAnimal->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
				Safe_Release<CGameObject*>(pAnimal);
		}
	}
	//MSG_BOX("Dynamic_Loaded.");
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Light(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + strLightFilePath + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Read);

	_uint iLightSize = 0;
	pFile->Read<_uint>(iLightSize);
	// 라이트 개수
	list<CLight*>* pLightlist = GI->Get_LightList();
	for (auto& pLight : *pLightlist)
		Safe_Release<CLight*>(pLight);

	pLightlist->clear();

	for (_uint i = 0; i < iLightSize; ++i)
	{
		LIGHTDESC LightDesc;
		::ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		// Type
		_uint iLightType = 0;
		_uint iLightID = 0;

		pFile->Read<_uint>(iLightType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == iLightType)
		{
			// ID
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vDiffuse, vAmbientLowerColor, vAmbientUpperColor, vDirection;
			pFile->Read<Vec3>(vDiffuse);
			pFile->Read<Vec3>(vAmbientLowerColor);
			pFile->Read<Vec3>(vAmbientUpperColor);
			pFile->Read<Vec3>(vDirection);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempColor = vDiffuse;
			LightDesc.vAmbientLowerColor = vAmbientLowerColor;
			LightDesc.vAmbientUpperColor = vAmbientUpperColor;
			LightDesc.vTempDirection = vDirection;
		}
		else if (LIGHTDESC::TYPE_POINT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vPos, vColor;
			_float fRange;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<_float>(fRange);
			pFile->Read<Vec3>(vColor);
			pFile->Read	<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.fTempRange = fRange;
			LightDesc.vTempColor = vColor;
			LightDesc.bNonCull = bNonCull;
		}
		else if (LIGHTDESC::TYPE_SPOT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			Vec3 vPos, vColor, vDirection;
			_float fTempRange, fOuterAngle, fInnerAngle;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<Vec3>(vDirection);
			pFile->Read<Vec3>(vColor);
			pFile->Read<_float>(fTempRange);
			pFile->Read<_float>(fOuterAngle);
			pFile->Read<_float>(fInnerAngle);
			pFile->Read<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.vTempDirection = vDirection;
			LightDesc.vTempColor = vColor;
			LightDesc.fTempRange = fTempRange;
			LightDesc.fOuterAngle = fOuterAngle;
			LightDesc.fInnerAngle = fInnerAngle;
			LightDesc.bNonCull = bNonCull;
		}

		if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Trigger(const wstring& strTriggerName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strTriggerName + L"/" + strTriggerName + L"Trigger.json";

	Json json = GI->Json_Load(strMapFilePath);
	Json parsedObjs = json["TriggerInfo"];

	for (const auto& obj : parsedObjs)
	{
		for (const auto& objInfo : obj)
		{
			string protoTypeTag = objInfo["ProtoTypeTag"];
			string objectTag = objInfo["ObjectTag"];

			Vec4 vRight, vUp, vLook, vPos;
			Vec4 vAt, vEye, vCamUp;
			_uint eTriggerType;

			wstring strBgmName;
			wstring strMapName;

			vRight.x = objInfo["Right"]["x"];
			vRight.y = objInfo["Right"]["y"];
			vRight.z = objInfo["Right"]["z"];

			vUp.x = objInfo["Up"]["x"];
			vUp.y = objInfo["Up"]["y"];
			vUp.z = objInfo["Up"]["z"];

			vLook.x = objInfo["Look"]["x"];
			vLook.y = objInfo["Look"]["y"];
			vLook.z = objInfo["Look"]["z"];

			vPos.x = objInfo["Position"]["x"];
			vPos.y = objInfo["Position"]["y"];
			vPos.z = objInfo["Position"]["z"];
			vPos.w = objInfo["Position"]["w"];

			eTriggerType = objInfo["TriggerType"];
			strBgmName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(objInfo["BgmName"]));
			strMapName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(objInfo["MapName"]));
			vAt.x = objInfo["At"]["x"];
			vAt.y = objInfo["At"]["y"];
			vAt.z = objInfo["At"]["z"];
			vAt.w = objInfo["At"]["w"];

			vEye.x = objInfo["Eye"]["x"];
			vEye.y = objInfo["Eye"]["y"];
			vEye.z = objInfo["Eye"]["z"];
			vEye.w = objInfo["Eye"]["w"];

			vCamUp.x = objInfo["CamUp"]["x"];
			vCamUp.y = objInfo["CamUp"]["y"];
			vCamUp.z = objInfo["CamUp"]["z"];
			vCamUp.w = objInfo["CamUp"]["w"];


			CGameObject* pGameObject = nullptr;
			if (GI->Add_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, CUtils::ToWString(protoTypeTag), nullptr,
				&pGameObject))
			{
				MSG_BOX("Load Object Failed : Trigger");
				return E_FAIL;
			}

			CTrigger* pTrigger = static_cast<CTrigger*>(pGameObject);
			CTransform* pTransform = pTrigger->Get_Component_Transform();
			if (nullptr == pTransform)
			{
				MSG_BOX("Not Found Transform");
				return E_FAIL;
			}

			pTransform->Set_Right(vRight);
			pTransform->Set_Up(vUp);
			pTransform->Set_Look(vLook);
			pTransform->Set_Position(vPos);

			pTrigger->Set_TriggerType(static_cast<TRIGGER_TYPE>(eTriggerType));
			pTrigger->Set_BgmName(strBgmName);
			pTrigger->Set_strMapName(strMapName);

			pTrigger->Set_At(vAt);
			pTrigger->Set_Eye(vEye);
			pTrigger->Set_Up(vCamUp);
		}
	}


	return S_OK;
}

CLevel_Evermore* CLevel_Evermore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Evermore* pInstance = new CLevel_Evermore(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_Evermore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Evermore::Free()
{
	__super::Free();
}
