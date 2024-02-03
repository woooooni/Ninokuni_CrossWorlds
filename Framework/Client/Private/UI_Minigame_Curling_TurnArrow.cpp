#include "stdafx.h"
#include "UI_Minigame_Curling_TurnArrow.h"
#include "GameInstance.h"

CUI_Minigame_Curling_TurnArrow::CUI_Minigame_Curling_TurnArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag)
	: CUI_Minigame_Curling_Base(pDevice, pContext, wstrTag)
{
}

CUI_Minigame_Curling_TurnArrow::CUI_Minigame_Curling_TurnArrow(const CUI_Minigame_Curling_TurnArrow& rhs)
	: CUI_Minigame_Curling_Base(rhs)
{
}

HRESULT CUI_Minigame_Curling_TurnArrow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_TurnArrow::Initialize(void* pArg)
{
	CUI::UI_INFO UIDesc = {};
	{
		const Vec2 vPosDelta = { g_vWinCenter.x * 0.f, g_vWinCenter.y * -0.85f };
		const Vec2 vOriginSize = { 1760.f, 220.f };
		const Vec2 vSizeMag = { 0.6f, 0.6f };

		UIDesc.fX = g_vWinCenter.x + vPosDelta.x;
		UIDesc.fY = g_vWinCenter.y + vPosDelta.y;
		UIDesc.fCX = vOriginSize.x * vSizeMag.x;
		UIDesc.fCY = vOriginSize.y * vSizeMag.y;
	}

	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Default()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_TurnArrow::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Minigame_Curling_TurnArrow::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Minigame_Curling_TurnArrow::Render()
{
	//if (FAILED(Bind_ShaderResources()))
	//	return E_FAIL;
	//
	//m_iPass = 1; // PS_MAIN_ALPHA
	//
	//m_pShaderCom->Begin(m_iPass);
	//
	//m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Minigame_Curling_TurnArrow::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_TitleFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_TurnArrow::Ready_Default()
{
	if (FAILED(__super::Ready_Default()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_TurnArrow::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	/* Texture */
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Minigame_Curling_TurnArrow* CUI_Minigame_Curling_TurnArrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag)
{
	CUI_Minigame_Curling_TurnArrow* pInstance = new CUI_Minigame_Curling_TurnArrow(pDevice, pContext, wstrTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Curling_TurnArrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Curling_TurnArrow::Clone(void* pArg)
{
	CUI_Minigame_Curling_TurnArrow* pInstance = new CUI_Minigame_Curling_TurnArrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Curling_TurnArrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Curling_TurnArrow::Free()
{
	__super::Free();
}
