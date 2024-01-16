#include "stdafx.h"
#include "Level_Logo.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"

#include "UI_Manager.h"
#include "UI_Fade.h"


CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Ni no Kuni : Cross Worlds"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f, true);

	GI->Stop_Sound(CHANNELID::SOUND_BGM_CURR);
	GI->Play_BGM(TEXT("BGM_Scene_Title_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR));

	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{
	_bool bSceneChange = false;

	for (_uint i = 0; i < (_uint)KEY::TYPE_END; ++i)
	{
		if (KEY_TAP((KEY)i))
			bSceneChange = true;
	}

	if (bSceneChange)
	{
		if (FAILED(GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY, L""))))
			return E_FAIL;
	}

	CUI_Manager::GetInstance()->Tick_Fade(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_Fade(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Logo::Render_Debug()
{

	return S_OK;
}

HRESULT CLevel_Logo::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_Logo::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_Veils(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(GI->Add_GameObject(LEVEL_LOGO, _uint(eLayerType), TEXT("Prototype_GameObject_UI_Logo_Background"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
