#include "stdafx.h"
#include "UI_Minigame_GaugeBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"

CUI_Minigame_GaugeBar::CUI_Minigame_GaugeBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_GaugeBar")
{
}

CUI_Minigame_GaugeBar::CUI_Minigame_GaugeBar(const CUI_Minigame_GaugeBar& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_GaugeBar::Set_Active(_bool bActive)
{
	if (false == bActive)
	{
		m_iTextureIndex = 0;
		m_iValue = 0;
	}

	m_bActive = bActive;
}

HRESULT CUI_Minigame_GaugeBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_GaugeBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_iValue = 0;
	m_iTextureIndex = 0;

	return S_OK;
}

void CUI_Minigame_GaugeBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::SPACE))
		{
			m_iValue++;
		}

		if (m_iValue == 3)
		{
			if (m_iTextureIndex < 22)
			{
				m_iTextureIndex++;
				m_iValue = 0;
			}
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_GaugeBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_iTextureIndex == 22)
		{
			CUIMinigame_Manager::GetInstance()->OnOff_Grandprix(true);
			return;
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_GaugeBar::Render()
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

HRESULT CUI_Minigame_GaugeBar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_Gauge"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_GaugeBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_GaugeBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_Minigame_GaugeBar* CUI_Minigame_GaugeBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_GaugeBar* pInstance = new CUI_Minigame_GaugeBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_GaugeBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_GaugeBar::Clone(void* pArg)
{
	CUI_Minigame_GaugeBar* pInstance = new CUI_Minigame_GaugeBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_GaugeBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_GaugeBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
