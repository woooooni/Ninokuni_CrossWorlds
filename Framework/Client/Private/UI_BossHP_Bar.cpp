#include "stdafx.h"
#include "UI_BossHP_Bar.h"
#include "GameInstance.h"

CUI_BossHP_Bar::CUI_BossHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BossHP_Bar")
{
}

CUI_BossHP_Bar::CUI_BossHP_Bar(const CUI_BossHP_Bar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BossHP_Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BossHP_Bar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	//m_bActive = true;
	m_bLerp = false;

	// TestCode
	m_fMaxHP = 1000.f;
	m_fCurHP = 1000.f;
	m_fPreHP = 1000.f;

	return S_OK;
}

void CUI_BossHP_Bar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_fTimeAcc += fTimeDelta * 0.1f;

		// TestCode
		if (KEY_TAP(KEY::G))
		{
			m_fCurHP -= 100.f;
			m_bLerp = false;
		}

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 100.f;
		
			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_BossHP_Bar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0.f >= m_fCurHP)
		{
			m_iPass = 18;

			m_fDissolveAcc += fTimeDelta;

			m_fRatio -= fTimeDelta * 0.5f;
			if (0 >= m_fRatio)
				m_fRatio = 0.f;

			m_fSinValue = sin(m_fTimeAcc);
		}
		else
		{
			_float fX = g_iWinSizeX * 0.5f - 30.f;
			_float fY = 766.f;

			// ±âÁØÁ¡
			CRenderer::TEXT_DESC  DividerDesc;
			DividerDesc.strText = L"/";
			DividerDesc.strFontTag = L"Default_Bold";
			DividerDesc.vScale = { 0.45f, 0.45f };
			DividerDesc.vPosition = _float2(fX, fY);
			DividerDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);

			wstring strCurHP = to_wstring(_int(m_fCurHP));
			CRenderer::TEXT_DESC CurHPDesc;
			CurHPDesc.strText = strCurHP;
			CurHPDesc.strFontTag = L"Default_Bold";
			CurHPDesc.vScale = { 0.4f, 0.4f };
			CurHPDesc.vPosition = _float2(fX - 50.f, fY + 2.f);
			CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);

			wstring strMaxHP = to_wstring(_int(m_fMaxHP));
			CRenderer::TEXT_DESC MaxHPDesc;
			MaxHPDesc.strText = strMaxHP;
			MaxHPDesc.strFontTag = L"Default_Bold";
			MaxHPDesc.vScale = { 0.4f, 0.4f };
			MaxHPDesc.vPosition = _float2(fX + 30.f, fY + 2.f);
			MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BossHP_Bar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_BossHP_Bar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dissolve"),
		TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissolveTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge_Glow"),
		TEXT("Com_GlowTexture"), (CComponent**)&m_pGlowTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge_Back"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BossHP_Bar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BossHP_Bar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_LerpHP", &m_fPreHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxHP, sizeof(_float))))
		return E_FAIL;

	if (m_iPass == 17)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTimeAcc, sizeof(_float))))
			return E_FAIL;
	}

	if (m_iPass == 18)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveAcc, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_SinValue", &m_fSinValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fRatio, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pGlowTextureCom->Bind_ShaderResource(m_pShaderCom, "g_GlowTexture")))
		return E_FAIL;

	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_LerpTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_BossHP_Bar* CUI_BossHP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BossHP_Bar* pInstance = new CUI_BossHP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BossHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossHP_Bar::Clone(void* pArg)
{
	CUI_BossHP_Bar* pInstance = new CUI_BossHP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BossHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossHP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pGlowTextureCom);
	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
