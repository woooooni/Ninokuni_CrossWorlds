#include "stdafx.h"
#include "UI_Minigame_Curling_Stones.h"
#include "GameInstance.h"

#include "CurlingGame_Group.h"

CUI_Minigame_Curling_Stones::CUI_Minigame_Curling_Stones(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
	: CUI_Minigame_Curling_Base(pDevice, pContext, wstrTag)
	, m_bPlayerType(bPlayerType)
{
}

CUI_Minigame_Curling_Stones::CUI_Minigame_Curling_Stones(const CUI_Minigame_Curling_Stones& rhs)
	: CUI_Minigame_Curling_Base(rhs)
	, m_bPlayerType(rhs.m_bPlayerType)
{
}

HRESULT CUI_Minigame_Curling_Stones::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Stones::Initialize(void* pArg)
{
	CUI::UI_INFO UIDesc = {};

	const Vec2 vPosDelta	= { g_vWinCenter.x * 0.815f, g_vWinCenter.y * 0.475f };
	const Vec2 vOriginSize	= { 1100.f, 220.f };
	const Vec2 vSizeMag		= { 0.275f, 0.275f };

	if (m_bPlayerType)
	{
		UIDesc.fX = g_vWinCenter.x + vPosDelta.x;
		UIDesc.fY = g_vWinCenter.y + vPosDelta.y;
	}
	else
	{
		UIDesc.fX = g_vWinCenter.x - vPosDelta.x;
		UIDesc.fY = g_vWinCenter.y + vPosDelta.y;
	}

	UIDesc.fCX = vOriginSize.x * vSizeMag.x;
	UIDesc.fCY = vOriginSize.y * vSizeMag.y;
	
	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Default()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_Stones::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Minigame_Curling_Stones::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Minigame_Curling_Stones::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Frame */
	{
		m_iPass = 1; // PS_MAIN_ALPHA

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Stones::Send_Message_Int(const _int& iValue)
{
	m_iCurStones = iValue;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Stones::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Lifes"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Stones::Ready_Default()
{
	if (FAILED(__super::Ready_Default()))
		return E_FAIL;


	m_iCurStones = g_iMaxStoneCount;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Stones::Bind_ShaderResources()
{
	if (g_iMaxStoneCount < m_iCurStones)
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	/* Texture */
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iCurStones)))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Minigame_Curling_Stones* CUI_Minigame_Curling_Stones::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
{
	CUI_Minigame_Curling_Stones* pInstance = new CUI_Minigame_Curling_Stones(pDevice, pContext, wstrTag, bPlayerType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Curling_Stones");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Curling_Stones::Clone(void* pArg)
{
	CUI_Minigame_Curling_Stones* pInstance = new CUI_Minigame_Curling_Stones(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Curling_Stones");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Curling_Stones::Free()
{
	__super::Free();
}
