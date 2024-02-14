#include "stdafx.h"
#include "UI_Stellia_DamageBar.h"
#include "GameInstance.h"

CUI_Stellia_DamageBar::CUI_Stellia_DamageBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Stellia_DamageBar")
{
}

CUI_Stellia_DamageBar::CUI_Stellia_DamageBar(const CUI_Stellia_DamageBar& rhs)
	: CUI(rhs)
{
}

void CUI_Stellia_DamageBar::Set_Active(_bool bActive)
{
	if(true == bActive)
	{
		m_iCurDamage = 0;
	}

	m_bActive = bActive;
}

HRESULT CUI_Stellia_DamageBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Stellia_DamageBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CUI::UI_INFO UIDesc = {};
	UIDesc.fCX = 690.f * 0.45f; // 0.8f -> 0.45f
	UIDesc.fCY = 50.f * 0.45f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 70.f; // 130.f -> 70.f
	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_iMaxDamage = 50000;
	m_iCurDamage = 0;

	m_bActive = false;

	return S_OK;
}

void CUI_Stellia_DamageBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_iMaxDamage <= m_iCurDamage)
		{
			m_iCurDamage = m_iMaxDamage;

			m_fTimeAcc += fTimeDelta; // 데미지가 다 차면 시간을 누적한다 -> 바로 사라지는 것은 어색함.
			if (1.f < m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				Set_Active(false);
				Set_Dead(true);
			}
		}

		if (m_iCurDamage > m_iPreDamage)
			m_bLerp = false;

		if ((false == m_bLerp) && (m_iPreDamage < m_iCurDamage))
		{
			m_iPreDamage += fTimeDelta * (m_iMaxDamage / 9.f);

			if (m_iPreDamage >= m_iCurDamage)
			{
				m_iPreDamage = m_iCurDamage;
				m_bLerp = true;
			}
		}

//		m_fTextureAcc += fTimeDelta;
//
//		if (0.01f < m_fTextureAcc)
//		{
//			m_fTextureAcc = 0.f;
//
//			if (7 == m_iTextureIndex)
//				m_iTextureIndex = 0;
//			else
//				m_iTextureIndex++;
//		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Stellia_DamageBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		_float4 vBlack = { 0.f, 0.f, 0.f, 1.f };
		_float4 vWhite = { 1.f, 1.f, 1.f, 1.f };
		_float4 vGray = { 0.47f, 0.47f, 0.47f, 1.f };

		// Text //
		// Current Damage
		CRenderer::TEXT_DESC CurDesc;
		CurDesc.strText = to_wstring(m_iCurDamage);
		_float fOffsetX = ((CurDesc.strText).length() - 1.f) * 9.f;
		CurDesc.strFontTag = L"Default_Bold";
		CurDesc.vScale = m_vTextScale;
		CurDesc.vColor = vBlack;
		CurDesc.vPosition = _float2(m_vCurTextPos.x - fOffsetX - 1.f, m_vCurTextPos.y);
		m_pRendererCom->Add_Text(CurDesc);
		CurDesc.vPosition = _float2(m_vCurTextPos.x - fOffsetX + 1.f, m_vCurTextPos.y);
		m_pRendererCom->Add_Text(CurDesc);
		CurDesc.vPosition = _float2(m_vCurTextPos.x - fOffsetX, m_vCurTextPos.y - 1.f);
		m_pRendererCom->Add_Text(CurDesc);
		CurDesc.vPosition = _float2(m_vCurTextPos.x - fOffsetX, m_vCurTextPos.y + 1.f);
		m_pRendererCom->Add_Text(CurDesc);

		CurDesc.vColor = vGray;
		CurDesc.vPosition = _float2(m_vCurTextPos.x - fOffsetX, m_vCurTextPos.y);
		m_pRendererCom->Add_Text(CurDesc);

		
		// Separator
		CRenderer::TEXT_DESC SeparatorDesc;
		SeparatorDesc.strText = TEXT("/");
		SeparatorDesc.strFontTag = L"Default_Bold";
		SeparatorDesc.vScale = m_vTextScale;
		SeparatorDesc.vColor = vBlack;
		SeparatorDesc.vPosition = _float2(m_vSeparatorPos.x - 1.f, m_vSeparatorPos.y);
		m_pRendererCom->Add_Text(SeparatorDesc);
		SeparatorDesc.vPosition = _float2(m_vSeparatorPos.x + 1.f, m_vSeparatorPos.y);
		m_pRendererCom->Add_Text(SeparatorDesc);
		SeparatorDesc.vPosition = _float2(m_vSeparatorPos.x, m_vSeparatorPos.y - 1.f);
		m_pRendererCom->Add_Text(SeparatorDesc);
		SeparatorDesc.vPosition = _float2(m_vSeparatorPos.x, m_vSeparatorPos.y + 1.f);
		m_pRendererCom->Add_Text(SeparatorDesc);

		SeparatorDesc.vColor = vGray;
		SeparatorDesc.vPosition = m_vSeparatorPos;
		m_pRendererCom->Add_Text(SeparatorDesc);
		

		// Max Damage
		CRenderer::TEXT_DESC MaxDesc;
		MaxDesc.strText = to_wstring(m_iMaxDamage);
		MaxDesc.strFontTag = L"Default_Bold";
		MaxDesc.vScale = m_vTextScale;
		MaxDesc.vColor = vBlack;
		MaxDesc.vPosition = _float2(m_vMaxTextPos.x -1.f, m_vMaxTextPos.y);
		m_pRendererCom->Add_Text(MaxDesc);
		MaxDesc.vPosition = _float2(m_vMaxTextPos.x + 1.f, m_vMaxTextPos.y);
		m_pRendererCom->Add_Text(MaxDesc);
		MaxDesc.vPosition = _float2(m_vMaxTextPos.x, m_vMaxTextPos.y - 1.f);
		m_pRendererCom->Add_Text(MaxDesc);
		MaxDesc.vPosition = _float2(m_vMaxTextPos.x, m_vMaxTextPos.y + 1.f);
		m_pRendererCom->Add_Text(MaxDesc);

		MaxDesc.vColor = vGray;
		MaxDesc.vPosition = m_vMaxTextPos;
		m_pRendererCom->Add_Text(MaxDesc);
		
		// TestCode
		if (KEY_TAP(KEY::O))
			Add_Damage(500);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Stellia_DamageBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Stellia_DamageBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_DamageBar_Background"),
		TEXT("Com_Texture_Background"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_DamageBar"),
		TEXT("Com_Texture"), (CComponent**)&m_pGaugeTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Stellia_DamageBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Stellia_DamageBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	_float fPreDamage = m_iPreDamage * 1.f;
	_float fCurDamage = m_iCurDamage * 1.f;
	_float fMaxDamage = m_iMaxDamage * 1.f;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_LerpHP", &fPreDamage, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &fCurDamage, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &fMaxDamage, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HPGaugeTexture")))
		return E_FAIL;
//	if (FAILED(m_pGaugeTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
//		return E_FAIL;
	if (FAILED(m_pGaugeTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Stellia_DamageBar* CUI_Stellia_DamageBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Stellia_DamageBar* pInstance = new CUI_Stellia_DamageBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Stellia_DamageBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Stellia_DamageBar::Clone(void* pArg)
{
	CUI_Stellia_DamageBar* pInstance = new CUI_Stellia_DamageBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Stellia_DamageBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Stellia_DamageBar::Free()
{
	__super::Free();

	Safe_Release(m_pGaugeTextureCom);
	Safe_Release(m_pTextureCom);
}
