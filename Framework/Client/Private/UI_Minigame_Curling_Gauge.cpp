#include "stdafx.h"
#include "UI_Minigame_Curling_Gauge.h"
#include "GameInstance.h"

CUI_Minigame_Curling_Gauge::CUI_Minigame_Curling_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag)
	: CUI_Minigame_Curling_Base(pDevice, pContext, wstrTag)
{
}

CUI_Minigame_Curling_Gauge::CUI_Minigame_Curling_Gauge(const CUI_Minigame_Curling_Gauge& rhs)
	: CUI_Minigame_Curling_Base(rhs)
{
}

HRESULT CUI_Minigame_Curling_Gauge::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Gauge::Initialize(void* pArg)
{
	CUI::UI_INFO UIDesc = {};
	{
		UIDesc.fCX = 300.f;
		UIDesc.fCY = 300.f;
		UIDesc.fX = g_vWinCenter.x * 1.65f;
		UIDesc.fY = g_vWinCenter.y;
	}

	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Default()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_Gauge::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (m_tLerpProgress.bActive)
		m_tLerpProgress.Update(fTimeDelta);
}

void CUI_Minigame_Curling_Gauge::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Minigame_Curling_Gauge::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Frame */
	{
		m_iPass = 23; // PS_VERTICAL_PROGRESS_COLOR

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Minigame_Curling_Gauge::Send_Message()
{
	m_tLerpProgress.Start(0.f, 2.f, LERP_MODE::SMOOTHER_STEP);
}

void CUI_Minigame_Curling_Gauge::Send_Message_Float(const _float& fValue)
{
	if (m_tLerpProgress.bActive)
		m_tLerpProgress.Clear();

	m_tLerpProgress.fCurValue = fValue;
}

HRESULT CUI_Minigame_Curling_Gauge::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Guage_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Gauge::Ready_Default()
{
	if (FAILED(__super::Ready_Default()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Gauge::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	/* Texture */
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	/* Raw */
	{
		memcpy(&m_vColor, &DirectX::Colors::Yellow, sizeof(Vec4));
		
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Progress", &m_tLerpProgress.fCurValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Diffusecolor", &m_vColor, sizeof(Vec4))))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Minigame_Curling_Gauge* CUI_Minigame_Curling_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag)
{
	CUI_Minigame_Curling_Gauge* pInstance = new CUI_Minigame_Curling_Gauge(pDevice, pContext, wstrTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Curling_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Curling_Gauge::Clone(void* pArg)
{
	CUI_Minigame_Curling_Gauge* pInstance = new CUI_Minigame_Curling_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Curling_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Curling_Gauge::Free()
{
	__super::Free();
}
