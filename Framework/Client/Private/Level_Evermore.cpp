#include "stdafx.h"
#include "Level_Evermore.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"

#include "UI_Manager.h"


CLevel_Evermore::CLevel_Evermore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Evermore::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Ni no Kuni : Cross Worlds"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Evermore::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_EVERMORE, fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Evermore::LateTick(_float fTimeDelta)
{

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
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	
	return S_OK;
}

HRESULT CLevel_Evermore::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_CommonUIs(LEVELID::LEVEL_EVERMORE)))
		return E_FAIL;

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
