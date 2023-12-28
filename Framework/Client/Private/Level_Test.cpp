#include "stdafx.h"
#include "Level_Test.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"

#include "Stellia.h"
#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Utils.h"
#include <FileUtils.h>

CLevel_Test::CLevel_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Test::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Scene_Tool"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(LAYER_TYPE::LAYER_TERRAIN)))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Player(LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL;

 	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, TEXT("Evermore"))))
		return E_FAIL;

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);


	return S_OK;
}

HRESULT CLevel_Test::Tick(_float fTimeDelta)
{

	if (KEY_TAP(KEY::PAGE_UP))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}

	if (KEY_TAP(KEY::PAGE_DOWN))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	}

	if (KEY_TAP(KEY::F7))
	{
		GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_DreamerMazeWitch"));
	}
	if (KEY_TAP(KEY::F8))
	{
		GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"));
	}
	if (KEY_TAP(KEY::F9))
	{
		GI->Add_GameObject(LEVEL_TEST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia"));
	}

	if (KEY_TAP(KEY::PAGE_UP))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}

	if (KEY_TAP(KEY::PAGE_DOWN))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	}

	CUI_Manager::GetInstance()->Tick_Fade(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Test::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_Fade(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Test::Render_Debug()
{
	
	return S_OK;
}

HRESULT CLevel_Test::Enter_Level()
{
	/*Protocol::S_ENTER_LEVEL tSendPkt;
	tSendPkt.set_ilevelid(LEVEL_TOOL);
	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(tSendPkt);
	CNetwork_Manager::GetInstance()->Send(sendBuffer);*/

	return S_OK;
}

HRESULT CLevel_Test::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_Test::Ready_Lights()
{
	


	//GI->Reset_Lights();

	//LIGHTDESC			LightDesc;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(15.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(25.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	//LightDesc.vDiffuse = _float4(0.5, 0.5, 0.5, 1.f);
	//LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	CGameObject* pTest = nullptr;

	/*if (FAILED(GI->Add_GameObject(LEVEL_TEST, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Character_SwordMan"), nullptr, &pTest)))
		return E_FAIL;*/

	if (FAILED(GI->Add_GameObject(LEVEL_TEST, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Character_Engineer"), nullptr, &pTest)))
		return E_FAIL;

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(pTest);
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(pTest);
	}

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	list<CGameObject*> Grounds = GI->Find_GameObjects(LEVEL_TEST, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}
	

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{
	/*if (FAILED(GI->Add_GameObject(LEVEL_TEST, LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Cloth_Terrain"))))
		return E_FAIL;*/
	
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
		
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Stellia"))))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Glanix"))))
	// 	return E_FAIL;

	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Baobam_Water"))))
	//	return E_FAIL;

	if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Shadow_Thief"))))
		return E_FAIL;

	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_IceBearMan"))))
	//	return E_FAIL;
	//
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_PumpkinCandle"))))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Clown"))))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Clown_Wizard"))))
	//	return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Baobam_Dark"))))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_Veils()))
		return E_FAIL;

	if (FAILED(CUI_Manager::GetInstance()->Ready_CommonUIs(LEVELID::LEVEL_TEST)))
		return E_FAIL;

	CUI_Manager::GetInstance()->Ready_CharacterTypeForUI(CHARACTER_TYPE::ENGINEER);
	CUI_Manager::GetInstance()->Ready_ElementalTypeForUI(ELEMENTAL_TYPE::DARK);

	//CUI_Manager::GetInstance()->Ready_CharacterTypeForUI(CHARACTER_TYPE::ENGINEER);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);


	_uint iObjectCount = File->Read<_uint>();
	for (_uint j = 0; j < iObjectCount; ++j)
	{
		// 3. Object_Prototype_Tag
		wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
		wstring strObjectTag = CUtils::ToWString(File->Read<string>());

		CGameObject* pObj = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_TEST, eLayerType, strPrototypeTag, nullptr, &pObj)))
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

	return S_OK;
}

CLevel_Test* CLevel_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Test* pInstance = new CLevel_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Test::Free()
{
	__super::Free();
}

