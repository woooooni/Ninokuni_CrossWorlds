#include "stdafx.h"
#include "UI_InGame_Setting_Window.h"
#include "GameInstance.h"
#include "UI_InGame_Setting_Tab.h"
#include "UI_Manager.h"
#include "Character.h"

CUI_InGame_Setting_Window::CUI_InGame_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Window")
{
}

CUI_InGame_Setting_Window::CUI_InGame_Setting_Window(const CUI_InGame_Setting_Window& rhs)
	: CUI(rhs)
{
}

void CUI_InGame_Setting_Window::Set_Active(_bool bActive)
{
	m_tInfo.fX = m_vOffPosition.x;
	m_tInfo.fY = m_vOffPosition.y; // UI가 출발할 Position으로 다시 위치를 세팅한다.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	m_bShowInfo = false;

	if (bActive)
	{
		m_fAlpha = 0.1f;

		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}
	}
	else
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}

		CCharacter* pCharacter = nullptr;
		pCharacter = CUI_Manager::GetInstance()->Get_Character();
		if (nullptr == pCharacter)
			return;

		pCharacter->Set_Skill_Input(true);
		pCharacter->Set_Attack_Input(true);
	}

	m_bActive = bActive;
}

HRESULT CUI_InGame_Setting_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Window::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;
	
	m_fAlpha = 0.3f;

	m_vOnPosition.x = m_tInfo.fX; // UI가 이동할 최종 위치
	m_vOnPosition.y = m_tInfo.fY;

	m_vOffPosition.x = m_tInfo.fX + (m_tInfo.fCX * 0.5f); // UI가 Active되는 시점에 등장할 위치
	m_vOffPosition.y = m_tInfo.fY;

	m_tInfo.fX = m_vOffPosition.x;
	m_tInfo.fY = m_vOffPosition.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	_float fSize = 128.f * 0.7f;
	_float fDefaultY = -185.f;
	_float fOffsetY = fSize * 0.5f + 90.f;

	Make_Child(-246.f, fDefaultY - fOffsetY + 10.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Graphic"));
	Make_Child(-246.f, fDefaultY, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Game"));
	Make_Child(-246.f, fDefaultY + fOffsetY - 5.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Audio"));

	m_bTabOpen[0] = false;
	m_bTabOpen[1] = false;
	m_bTabOpen[2] = false;

	m_bActive = false;

	return S_OK;
}

void CUI_InGame_Setting_Window::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		CCharacter* pCharacter = nullptr;
		pCharacter = CUI_Manager::GetInstance()->Get_Character();
		if (nullptr == pCharacter)
			return;

		pCharacter->Set_Skill_Input(false);
		pCharacter->Set_Attack_Input(false);

		Update_SettingTab();

		if (!m_bShowInfo)
		{
			if (m_fAlpha > 0.9f)
				m_fAlpha = 0.9f;
			else
				m_fAlpha += fTimeDelta;

			m_tInfo.fX -= fTimeDelta * 200.f;

			if (m_tInfo.fX <= m_vOnPosition.x) // 목표 위치 이상으로 넘어가지 않도록 한다.
			{
				m_bShowInfo = true;
				m_tInfo.fX = m_vOnPosition.x;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
	}

	__super::Tick(fTimeDelta);
}

void CUI_InGame_Setting_Window::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_InGame_Setting_Window::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_InGame_Setting_Window::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Window"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_InGame_Setting_Window::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Window::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_InGame_Setting_Window::Update_SettingTab()
{
	CUI* pGraphicTab = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Graphic"));
	if (pGraphicTab == nullptr)
		return;
	CUI_InGame_Setting_Tab* pGraphic = nullptr;
	pGraphic = dynamic_cast<CUI_InGame_Setting_Tab*>(pGraphicTab);
	if (pGraphic == nullptr)
		return;

	CUI* pGameTab = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Game"));
	if (pGameTab == nullptr)
		return;
	CUI_InGame_Setting_Tab* pGame = nullptr;
	pGame = dynamic_cast<CUI_InGame_Setting_Tab*>(pGameTab);
	if (pGame == nullptr)
		return;

	CUI* pAudioTab = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Audio"));
	if (pAudioTab == nullptr)
		return;
	CUI_InGame_Setting_Tab* pAudio = nullptr;
	pAudio = dynamic_cast<CUI_InGame_Setting_Tab*>(pAudioTab);
	if (pAudio == nullptr)
		return;

	if (true == pGraphic->Get_Clicked() &&
		false == m_bTabOpen[0])
	{
		pGame->Set_Click(false);
		m_bTabOpen[1] = false;
		pAudio->Set_Click(false);
		m_bTabOpen[2] = false;

		m_bTabOpen[0] = true;
	}
	
	if (true == pGame->Get_Clicked() &&
		false == m_bTabOpen[1])
	{
		pGraphic->Set_Click(false);
		m_bTabOpen[0] = false;
		pAudio->Set_Click(false);
		m_bTabOpen[2] = false;

		m_bTabOpen[1] = true;
	}
	
	if (true == pAudio->Get_Clicked() &&
		false == m_bTabOpen[2])
	{
		pGraphic->Set_Click(false);
		m_bTabOpen[0] = false;
		pGame->Set_Click(false);
		m_bTabOpen[1] = false;

		m_bTabOpen[2] = true;
	}
}

CUI_InGame_Setting_Window* CUI_InGame_Setting_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InGame_Setting_Window* pInstance = new CUI_InGame_Setting_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Window::Clone(void* pArg)
{
	CUI_InGame_Setting_Window* pInstance = new CUI_InGame_Setting_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
