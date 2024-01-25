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

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Light_Manager.h"
#include "Light.h"
#include <Utils.h>
#include <FileUtils.h>


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

//	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
//		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);

	GI->Play_BGM(TEXT("BGM_Field_Village_Winter_Po_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);

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

	if (KEY_TAP(KEY::F8))
	{
		GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"));
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

	list<CGameObject*> Grounds = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}


	list<CGameObject*> Buildings = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}



	list<CGameObject*> Props = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP);
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
	if (FAILED(CGame_Manager::GetInstance()->Get_Player()->Set_Character(CHARACTER_TYPE::SWORD_MAN, Vec4(-44.f, 1.6f, 315.f, 1.f), true)))
		return E_FAIL;

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	}

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

HRESULT CLevel_WitchForest::Ready_Light(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + strLightFilePath + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Write);

	list<CLight*>* pLightList = GI->Get_LightList();
	pFile->Write<_uint>(pLightList->size());
	// 라이트 개수

	for (auto& pLight : *pLightList)
	{
		const LIGHTDESC* pLightDesc = pLight->Get_LightDesc();

		// Type
		pFile->Write<_uint>(pLightDesc->eType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == pLightDesc->eType)
		{
			// ID
			pFile->Write<_uint>(pLight->Get_LightID());

			// State
			pFile->Write<Vec3>(pLightDesc->vTempColor);
			pFile->Write<Vec3>(pLightDesc->vAmbientLowerColor);
			pFile->Write<Vec3>(pLightDesc->vAmbientUpperColor);
			pFile->Write<Vec3>(pLightDesc->vTempDirection);
		}
		else if (LIGHTDESC::TYPE_POINT == pLightDesc->eType)
		{
			// ID
			pFile->Write<_uint>(pLight->Get_LightID());

			// State
			pFile->Write<Vec3>(pLightDesc->vTempPosition);
			pFile->Write<_float>(pLightDesc->fTempRange);
			pFile->Write<Vec3>(pLightDesc->vTempColor);
		}
		else if (LIGHTDESC::TYPE::TYPE_SPOT == pLightDesc->eType)
		{
			pFile->Write<_uint>(pLight->Get_LightID());

			pFile->Write<Vec3>(pLightDesc->vTempPosition);
			pFile->Write<Vec3>(pLightDesc->vTempDirection);
			pFile->Write<Vec3>(pLightDesc->vTempColor);
			pFile->Write<_float>(pLightDesc->fTempRange);
			pFile->Write<_float>(pLightDesc->fOuterAngle);
			pFile->Write<_float>(pLightDesc->fInnerAngle);
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
