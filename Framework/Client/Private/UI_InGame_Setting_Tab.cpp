#include "stdafx.h"
#include "UI_InGame_Setting_Tab.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_InGame_Setting_Tab::CUI_InGame_Setting_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTINGTAB eUIType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Tab")
	, m_eUIType(eUIType)
{
}

CUI_InGame_Setting_Tab::CUI_InGame_Setting_Tab(const CUI_InGame_Setting_Tab& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
{
}

void CUI_InGame_Setting_Tab::Set_Active(_bool bActive)
{
	if (SETTINGTAB_END == m_eUIType)
		return;

	if (false == bActive)
	{
		if (m_bEvent)
			m_bEvent = false;

		m_bClicked = false;
		m_fAlpha = 0.1f;
	}

	m_bActive = bActive;
}

HRESULT CUI_InGame_Setting_Tab::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Tab::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_InGame_Setting_Tab::Tick(_float fTimeDelta)
{
	if (SETTINGTAB_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_fAlpha > 0.9f)
			m_fAlpha = 0.9f;
		else
			m_fAlpha += fTimeDelta;
	}

	__super::Tick(fTimeDelta);
}

void CUI_InGame_Setting_Tab::LateTick(_float fTimeDelta)
{
	if (SETTINGTAB_END == m_eUIType)
		return;

	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_InGame_Setting_Tab::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_InGame_Setting_Tab::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_InGame_Setting_Tab::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Click_TabButton();
			CUI_Manager::GetInstance()->OnOff_SettingSlot(_uint(m_eUIType), true);

			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_InGame_Setting_Tab::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_InGame_Setting_Tab::Click_TabButton()
{
	if (m_eUIType == UI_SETTINGTAB::SETTINGTAB_END)
		return;

	if (false == m_bClicked)
		m_bClicked = true;
	else
		m_bClicked = false;
}

HRESULT CUI_InGame_Setting_Tab::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eUIType)
	{
	case UI_SETTINGTAB::SETTING_GRAPHIC:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Graphic"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SETTINGTAB::SETTING_GAME:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Game"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_SETTINGTAB::SETTING_AUDIO:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Audio"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	default:
		break;
	}

	
	return S_OK;
}

HRESULT CUI_InGame_Setting_Tab::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Tab::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_bClicked))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_InGame_Setting_Tab* CUI_InGame_Setting_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTINGTAB eUIType)
{
	CUI_InGame_Setting_Tab* pInstance = new CUI_InGame_Setting_Tab(pDevice, pContext, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_Tab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Tab::Clone(void* pArg)
{
	CUI_InGame_Setting_Tab* pInstance = new CUI_InGame_Setting_Tab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_Tab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Tab::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
