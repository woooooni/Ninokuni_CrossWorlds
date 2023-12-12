#include "stdafx.h"
#include "Level_Test.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"

#include "Stellia.h"

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

	if (FAILED(Ready_Layer_Player(LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;






	return S_OK;
}

HRESULT CLevel_Test::Tick(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CLevel_Test::LateTick(_float fTimeDelta)
{
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
	CCamera_Manager::GetInstance()->Set_MainCamera(CCamera_Manager::CAMERA_TYPE::TOOL);

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	/*if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prototype_GameObject_Witch"))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	//if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Background"))))
	//	return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_TEST, _uint(eLayerType), TEXT("Prorotype_GameObject_Stellia"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{

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
