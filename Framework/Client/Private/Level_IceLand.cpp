#include "stdafx.h"
#include "Level_IceLand.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"
#include "Game_Manager.h"
#include "Player.h"
#include "GameNpc.h"

#include "Utils.h"
#include <FileUtils.h>

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Camera_Group.h"
#include "Particle_Manager.h"
#include <Utils.h>
#include <FileUtils.h>
#include "Water.h"
#include "GameNpc.h"
#include "Animals.h"

#include "Portal.h"
#include "Trigger.h"

CLevel_IceLand::CLevel_IceLand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_IceLand::Initialize()
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

	if(FAILED(Ready_Layer_Prop(LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, L"Winter")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc(LAYER_TYPE::LAYER_NPC)))
		return E_FAIL;
 
	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);

	/* Camera Action */
	{
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());
		if (nullptr != pActionCam)
		{
			pActionCam->Start_Action_Door();
		}
	}

	GI->Stop_Sound(CHANNELID::SOUND_BGM);
	GI->Play_BGM(TEXT("BGM_Field_Village_Winter_Po_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM));

	return S_OK;
}

HRESULT CLevel_IceLand::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_Fade(fTimeDelta);
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_ICELAND, fTimeDelta);

	if (KEY_TAP(KEY::PAGE_UP))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}

	if (KEY_TAP(KEY::PAGE_DOWN))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	}

	if (KEY_TAP(KEY::F8))
	{
		GI->Add_GameObject(LEVEL_ICELAND, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"));
	}

	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
	{
		GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_EVERMORE, L"Evermore"));
	}

	return S_OK; 
}

HRESULT CLevel_IceLand::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_Fade(fTimeDelta);
	CUI_Manager::GetInstance()->LateTick_EvermoreLevel(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_IceLand::Render_Debug()
{

	return S_OK;
}

HRESULT CLevel_IceLand::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_IceLand::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	list<CGameObject*> Grounds = GI->Find_GameObjects(LEVEL_ICELAND, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}


	list<CGameObject*> Buildings = GI->Find_GameObjects(LEVEL_ICELAND, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}



	list<CGameObject*> Props = GI->Find_GameObjects(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP);
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

HRESULT CLevel_IceLand::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND,LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
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
	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Door_Enter"), nullptr, &pDoor)))
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

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Prop(const LAYER_TYPE eLayerType)
{

	CGameObject* pPortal = nullptr;

	CPortal::PORTAL_DESC PortalInfo = {};
	PortalInfo.vStartPosition = XMVectorSet(143.f, 0.11316f, 12.f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(-20.f, 9.9f, 133.f, 1.f);
	PortalInfo.eCurrentLevel = LEVEL_ICELAND;
	PortalInfo.eNextLevel = LEVEL_EVERMORE;

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;

	if (nullptr == pPortal)
		return E_FAIL;

	CTransform* pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-100.f));




	PortalInfo.vStartPosition = XMVectorSet(141.75f, 0.11316f, 20.f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(-20.f, 9.9f, 133.f, 1.f);
	PortalInfo.eCurrentLevel = LEVEL_ICELAND;
	PortalInfo.eNextLevel = LEVEL_EVERMORE;

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;

	if (nullptr == pPortal)
		return E_FAIL;

	pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-100.f));



	CTrigger::TRIGGER_DESC TriggerDesc;
	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_MAP_NAME;
	TriggerDesc.strMapName = "코에루크 설원";
	TriggerDesc.vStartPosition = { 107.93f, -10.f, 5.067f, 1.f };
	TriggerDesc.vExtents = { 100.f, 100.f, 100.f };

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.strMapName = "그늘진 저지대";
	TriggerDesc.vStartPosition = { -79.747f, -10.f, 70.857f, 1.f };
	TriggerDesc.vExtents = { 100.f, 100.f, 100.f };

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.strMapName = "얼어붙은 유령숲";
	TriggerDesc.vStartPosition = { -124.5f, -10.f, 290.2f, 1.f };
	TriggerDesc.vExtents = { 200.f, 100.f, 150.f };

	if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
	//Vec4 tempPos = Vec4(-108.f, -4.14f, 234.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//tempPos = Vec4(-105.f, -4.14f, 234.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//tempPos = Vec4(-102.f, -4.14f, 234.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//tempPos = Vec4(-99.f, -4.14f, 234.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Shadow_Thief"), &tempPos)))
	//	return E_FAIL;
	//
	//tempPos = Vec4(113.f, 1.5f, 8.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_IceBearMan"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_IceBearMan"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_IceBearMan"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_IceBearMan"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_IceBearMan"), &tempPos)))
	//	return E_FAIL;
	//
	//tempPos = Vec4(115.f, 1.5f, 8.f, 1.f);
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Baobam_Water"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Baobam_Water"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Baobam_Water"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Baobam_Water"), &tempPos)))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_MONSTER, TEXT("Prorotype_GameObject_Baobam_Water"), &tempPos)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_NPC(const LAYER_TYPE eLayerType)
{
	wstring strNpcFileName = L"Winter";
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_ICELAND, i);


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
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_ICELAND, i, strPrototypeTag, &Init_Data, &pObj)))
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

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_GameObjectToLayer(LEVELID::LEVEL_ICELAND)))
		return E_FAIL;

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

HRESULT CLevel_IceLand::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	_matrix WorldMatrix = XMMatrixIdentity();
	
	WorldMatrix.r[3] = XMVectorSet(-60.f, 50.f, 150.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_02"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	
	WorldMatrix.r[3] = XMVectorSet(-60.f, 50.f, 350.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_02"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	
	WorldMatrix.r[3] = XMVectorSet(100.f, 50.f, 0.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_ICELAND, TEXT("Particle_Snow_02"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVELID::LEVEL_ICELAND, LAYER_TYPE::LAYER_DYNAMIC);


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
		if (FAILED(GI->Add_GameObject(LEVEL_ICELAND, LAYER_TYPE::LAYER_DYNAMIC, strPrototypeTag, &Init_Data, &pObj)))
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
	}

	return S_OK;
}

HRESULT CLevel_IceLand::Ready_Layer_Npc(const LAYER_TYPE eLayerType)
{
	wstring strNpcFileName = L"Winter";
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_ICELAND, i);


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
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_ICELAND, i, strPrototypeTag, &Init_Data, &pObj)))
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

	return S_OK;
}

CLevel_IceLand* CLevel_IceLand::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_IceLand* pInstance = new CLevel_IceLand(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_IceLand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_IceLand::Free()
{
	__super::Free();
}
