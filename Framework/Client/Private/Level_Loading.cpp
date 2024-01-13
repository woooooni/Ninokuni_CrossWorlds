#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_Tool.h"
#include "Level_Test.h"
#include "Level_Lobby.h"
#include "Level_Evermore.h"
#include "Level_Kingdom.h"
#include "Level_IceLand.h"
#include "UI_Manager.h"
#include "ImGui_Manager.h"


CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}


HRESULT CLevel_Loading::Initialize(LEVELID eNextLevel, const wstring& strFolderName)
{
	m_eNextLevel = eNextLevel;

	GI->Stop_Sound(CHANNELID::SOUND_BGM);
//	GI->Play_BGM(TEXT("BGM_Town_Evermore_Sanrio_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM));

	return S_OK;
}

HRESULT CLevel_Loading::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Loading::LateTick(_float fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	if (true == m_pLoader->Get_Finished())
	{
		if (KEY_TAP(KEY::SPACE))
		{
			CLevel* pNewLevel = nullptr;
			GI->Clear_PhysX_Ground();
			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_TOOL:
				pNewLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_TEST:
				pNewLevel = CLevel_Test::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_LOBBY:
				pNewLevel = CLevel_Lobby::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_EVERMORE:
				pNewLevel = CLevel_Evermore::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_KINGDOMHALL:
				pNewLevel = CLevel_Kingdom::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_ICELAND:
				pNewLevel = CLevel_IceLand::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return E_FAIL;

			if (FAILED(GI->Open_Level(m_eNextLevel, pNewLevel)))
				return E_FAIL;
		}

	}


	return S_OK;
}

HRESULT CLevel_Loading::Enter_Level()
{
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel, L"");
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Exit_Level()
{
	return S_OK;
}


HRESULT CLevel_Loading::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if(FAILED(CUI_Manager::GetInstance()->Ready_Loadings()))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel, const wstring& strFolderName)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, strFolderName)))
	{
		MSG_BOX("Failed to Create : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);
}
