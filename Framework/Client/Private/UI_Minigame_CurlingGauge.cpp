#include "stdafx.h"
#include "UI_Minigame_CurlingGauge.h"
#include "GameInstance.h"

CUI_Minigame_CurlingGauge::CUI_Minigame_CurlingGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CURLINGGAUGE eType)
	: CUI(pDevice, pContext, L"UI_Minigame_CurlingGauge")
	, m_eType(eType)
{
}

CUI_Minigame_CurlingGauge::CUI_Minigame_CurlingGauge(const CUI_Minigame_CurlingGauge& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Minigame_CurlingGauge::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_CurlingGauge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// Default값
	m_fAlpha = 1.f;
	m_bActive = false;

	return S_OK;
}

void CUI_Minigame_CurlingGauge::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (CURLINGGAUGE_END == m_eType)
			return;

		if (GAUGE_BAR == m_eType)
			Tick_GaugeBar(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_CurlingGauge::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (CURLINGGAUGE_END == m_eType)
			return;

		if (GAUGE_BAR == m_eType)
			LateTick_GaugeBar(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_CurlingGauge::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Minigame_CurlingGauge::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Texture Component
	switch (m_eType)
	{
	case GAUGE_BACK:
		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_GaugeBar_Back"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iPass = 1;
		break;

	case GAUGE_BAR:
		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_GaugeBar"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iPass = 4;
		m_fProgress = 1.f; // 텍스처 확인 목적의 임의의 값
		// 로딩바에서 사용한 Pass. g_LoadingProgress에 0~1의 값을 던지면 됨(m_fProgress 이용)
		break;
	}

	return S_OK;
}

HRESULT CUI_Minigame_CurlingGauge::Ready_State()
{

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_CurlingGauge::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (GAUGE_BAR == m_eType)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fProgress, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_Minigame_CurlingGauge::Tick_GaugeBar(_float fTimeDelta)
{
}

void CUI_Minigame_CurlingGauge::LateTick_GaugeBar(_float fTimeDelta)
{
}

CUI_Minigame_CurlingGauge* CUI_Minigame_CurlingGauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CURLINGGAUGE eType)
{
	CUI_Minigame_CurlingGauge* pInstance = new CUI_Minigame_CurlingGauge(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_CurlingGauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_CurlingGauge::Clone(void* pArg)
{
	CUI_Minigame_CurlingGauge* pInstance = new CUI_Minigame_CurlingGauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_CurlingGauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_CurlingGauge::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
