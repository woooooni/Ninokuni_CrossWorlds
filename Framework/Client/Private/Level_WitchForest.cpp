#include "stdafx.h"
#include "Level_WitchForest.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"
#include "Game_Manager.h"
#include "Player.h"

#include "Riding_Manager.h"
#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Light_Manager.h"
#include "Light.h"
#include <Utils.h>
#include <FileUtils.h>
#include "Portal.h"
#include "Trigger.h"
#include "GameNpc.h"
#include "Animals.h"
#include "Water.h"
#include "Particle_Manager.h"
#include "Ruby.h"

#include "Camera_Action.h"
#include "Respawn_Box.h"

_bool CLevel_WitchForest::g_bFirstEnter = false;

CLevel_WitchForest::CLevel_WitchForest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_WitchForest::Initialize()
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

	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, L"Witch")))
		return E_FAIL;

	if (FAILED(Ready_Light(TEXT("Witch Light"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc(LAYER_TYPE::LAYER_NPC)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Prop(LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;

	if (FAILED(Ready_Trigger(TEXT("Witch"))))
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

	GI->Play_BGM(TEXT("BGM_Field_Forest_Mystic_Night_2.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);

	if (false == g_bFirstEnter)
	{
		g_bFirstEnter = true;
		CUI_Manager::GetInstance()->OnOff_MapName(true, TEXT("숲의 입구"));
	}

	return S_OK;
}

HRESULT CLevel_WitchForest::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_WITCHFOREST, fTimeDelta);

	if (KEY_TAP(KEY::PAGE_UP))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}

	if (KEY_TAP(KEY::PAGE_DOWN))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	}

	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
	{
		GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_EVERMORE, L"Evermore"));
	}

	if (KEY_TAP(KEY::F8))
	{
		GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia"));
	}

	return S_OK;
}

HRESULT CLevel_WitchForest::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_GamePlayLevel(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_WitchForest::Render_Debug()
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{

	list<CGameObject*>& Grounds = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}


	list<CGameObject*>& Buildings = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}



	list<CGameObject*>& Props = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP);
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

HRESULT CLevel_WitchForest::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	}

	CGameObject* pDoor = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Door_Enter"), nullptr, &pDoor)))
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

	if (FAILED(CRiding_Manager::GetInstance()->Ready_Vehicle_GameObjectToLayer(LEVELID::LEVEL_WITCHFOREST)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_GameObjectToLayer(LEVELID::LEVEL_WITCHFOREST)))
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

HRESULT CLevel_WitchForest::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	_matrix WorldMatrix = XMMatrixIdentity();

	WorldMatrix.r[3] = XMVectorSet(30.f, 0.f, 20.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Particle_Firefly_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Particle_Firefly_02"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;

	WorldMatrix.r[3] = XMVectorSet(65.f, 4.f, 12.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Particle_Firefly_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;

	WorldMatrix.r[3] = XMVectorSet(37.f, -3.f, -11.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Witch_Grass_Particle_1"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	WorldMatrix.r[3] = XMVectorSet(50.f, 0.0f, 1.0f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Witch_Grass_Particle_2"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;
	WorldMatrix.r[3] = XMVectorSet(65.f, 4.f, 12.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_WITCHFOREST, TEXT("Witch_Grass_Particle_3"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_DYNAMIC);


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
		if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_DYNAMIC, strPrototypeTag, &Init_Data, &pObj)))
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

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Npc(const LAYER_TYPE eLayerType)
{
	wstring strNpcFileName = L"Witch";
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";
	 
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_WITCHFOREST, i);


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
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_WITCHFOREST, i, strPrototypeTag, &Init_Data, &pObj)))
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

HRESULT CLevel_WitchForest::Ready_Light(const wstring& strLightFilePath)
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

HRESULT CLevel_WitchForest::Ready_Layer_Prop(const LAYER_TYPE eLayerType)
{
	CGameObject* pPortal = nullptr;

	CPortal::PORTAL_DESC PortalInfo = {};
	PortalInfo.vStartPosition = XMVectorSet(4.f, 2.262f, -95.961, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(124.f, 0.f, 111.f, 1.f);
	PortalInfo.eCurrentLevel = LEVEL_WITCHFOREST;
	PortalInfo.eNextLevel = LEVEL_EVERMORE;
	PortalInfo.vEffectScale = Vec3(15.f, 0.3f, 1.f);
	PortalInfo.vNextRotation = Vec3(8.f, -90.f, 0.f);
	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;
	if (nullptr == pPortal)
		return E_FAIL;
	CTransform* pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-50.f));


	PortalInfo.vStartPosition = XMVectorSet(8.f, 2.262f, -91.f, 1.f);
	PortalInfo.vNextPosition = XMVectorSet(124.f, 0.f, 111.f, 1.f);
	PortalInfo.eCurrentLevel = LEVEL_WITCHFOREST;
	PortalInfo.eNextLevel = LEVEL_EVERMORE;
	PortalInfo.vEffectScale = Vec3(15.f, 0.3f, 1.f);
	PortalInfo.vNextRotation = Vec3(8.f, -90.f, 0.f);
	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Portal"), &PortalInfo, &pPortal)))
		return E_FAIL;
	if (nullptr == pPortal)
		return E_FAIL;
	pPortalTransform = pPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
		return E_FAIL;
	pPortalTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-50.f));



	CTrigger::TRIGGER_DESC TriggerDesc;
	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_WOOD_ENTER;
	TriggerDesc.strMapName = L"";
	TriggerDesc.vStartPosition = { 101.275f, -4.906f, 28.147f, 1.f };
	TriggerDesc.vExtents = { 5.f, 5.f, 5.f };

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, eLayerType, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_ESCORT1;
	TriggerDesc.vStartPosition = { 49.282f, -5.259f, -3.901f, 1.f };
	TriggerDesc.vExtents = { 3.0f, 3.0f, 3.0f };

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_ESCORT2;
	TriggerDesc.vStartPosition = { 7.889f, -6.283f, -22.237f, 1.f };
	TriggerDesc.vExtents = { 3.0f, 3.0f, 3.0f };

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	// MapName Trigger (strMapName만 완료)
//	TriggerDesc = {};
//	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_MAP_NAME;
//	TriggerDesc.strMapName = L"숲의 입구";
//	TriggerDesc.vStartPosition = { 107.93f, -50.f, 5.067f, 1.f };
//	TriggerDesc.vExtents = { 100.f, 200.f, 100.f };
//	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
//		return E_FAIL;
//	
//	TriggerDesc = {};
//	TriggerDesc.strMapName = L"정령의 쉼터";
//	TriggerDesc.vStartPosition = { -79.747f, -50.f, 70.857f, 1.f };
//	TriggerDesc.vExtents = { 100.f, 200.f, 100.f };
//	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
//		return E_FAIL;
//
//	TriggerDesc = {};
//	TriggerDesc.strMapName = L"초승달 평원 북부";
//	TriggerDesc.vStartPosition = { -124.5f, -50.f, 290.2f, 1.f };
//	TriggerDesc.vExtents = { 200.f, 200.f, 150.f };
//	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
//		return E_FAIL;
//
//	TriggerDesc = {};
//	TriggerDesc.strMapName = L"반디 정원";
//	TriggerDesc.vStartPosition = { -124.5f, -50.f, 290.2f, 1.f };
//	TriggerDesc.vExtents = { 200.f, 200.f, 150.f };
//	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
//		return E_FAIL;

	CRespawn_Box::RESPAWN_DESC RespawnDesc = {};
	RespawnDesc.vStartPosition = Vec4(0.f, -100.f, 0.f, 1.f);
	RespawnDesc.vRespawnPosition = Vec4(0.f, 0.f, 0.f, 1.f);
	RespawnDesc.vExtents = Vec3(1000.f, 5.f, 1000.f);

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_RespawnBox"), &RespawnDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Trigger(const wstring& strTriggerName)
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
			if (GI->Add_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP, CUtils::ToWString(protoTypeTag), nullptr,
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

CLevel_WitchForest* CLevel_WitchForest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_WitchForest* pInstance = new CLevel_WitchForest(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_WitchForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WitchForest::Free()
{
	__super::Free();
}
