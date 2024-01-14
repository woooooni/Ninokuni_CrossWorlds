#include "stdafx.h"
#include "UI_BossHP_Bar.h"
#include "GameInstance.h"
#include "Boss.h"
#include "UI_Manager.h"

CUI_BossHP_Bar::CUI_BossHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BossHP_Bar")
{
}

CUI_BossHP_Bar::CUI_BossHP_Bar(const CUI_BossHP_Bar& rhs)
	: CUI(rhs)
{
}

void CUI_BossHP_Bar::Set_Owner(CBoss* pBoss)
{
	if (nullptr == pBoss)
		return;

	m_pOwner = pBoss;

	CMonster::MONSTER_STAT StatDesc = {};
	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));

	memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

	if (TEXT("Glanix") == m_pOwner->Get_ObjectTag())
	{
		m_iGaugeIndex = 0;
		m_iFXTexIndex = 0;
	}
	else if (TEXT("Stellia") == m_pOwner->Get_ObjectTag())
	{
		m_iGaugeIndex = 1;
		m_iFXTexIndex = 1;
	}
	else if (TEXT("DreamerMazeWitch") == m_pOwner->Get_ObjectTag())
	{
		m_iGaugeIndex = 1;
		m_iFXTexIndex = 2;
	}

	m_fMaxHP = StatDesc.fMaxHp;
	m_fCurHP = StatDesc.fHp;
	m_fPreHP = m_fCurHP;
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

	m_bActive = false;
	m_bLerp = false;

	return S_OK;
}

void CUI_BossHP_Bar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_fTimeAcc += fTimeDelta * 0.1f;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 3000.f;
		
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
		CMonster::MONSTER_STAT StatDesc = {};
		ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));
		memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

		m_fCurHP = StatDesc.fHp;

		if (0.f >= m_fCurHP)
		{
			CUI_Manager::GetInstance()->Set_BossActive(false);
			CUI_Manager::GetInstance()->OnOff_BossHP(false);

			//m_iPass = 18;
			//
			//m_fDissolveAcc += fTimeDelta;
			//
			//m_fRatio -= fTimeDelta * 0.5f;
			//if (0 >= m_fRatio)
			//	m_fRatio = 0.f;
			//
			//m_fSinValue = sin(m_fTimeAcc);
		}
		else
		{
			// 기준점
			CRenderer::TEXT_DESC  DividerDesc;
			DividerDesc.strText = L"/";
			DividerDesc.strFontTag = L"Default_Bold";
			DividerDesc.vScale = { 0.45f, 0.45f };
			// OutLine
			DividerDesc.vPosition = _float2(m_vDividerPos.x - 1.f, m_vDividerPos.y);
			DividerDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);
			DividerDesc.vPosition = _float2(m_vDividerPos.x + 1.f, m_vDividerPos.y);
			DividerDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);
			DividerDesc.vPosition = _float2(m_vDividerPos.x, m_vDividerPos.y - 1.f);
			DividerDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);
			DividerDesc.vPosition = _float2(m_vDividerPos.x, m_vDividerPos.y + 1.f);
			DividerDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);
			// Origin
			DividerDesc.vPosition = m_vDividerPos;
			DividerDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(DividerDesc);

			// 현재 체력
			wstring strCurHP = to_wstring(_int(m_fCurHP));
			CRenderer::TEXT_DESC CurHPDesc;
			CurHPDesc.strText = strCurHP;
			CurHPDesc.strFontTag = L"Default_Bold";
			CurHPDesc.vScale = { 0.4f, 0.4f };

			_int iLength = (CurHPDesc.strText.length() - 1) * 10.f;
			m_vCurHPPos = _float2(g_iWinSizeX * 0.5f - 60.f - iLength, 768.f);

			// OutLine
			CurHPDesc.vPosition = _float2(m_vCurHPPos.x - 1.f, m_vCurHPPos.y);
			CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);
			CurHPDesc.vPosition = _float2(m_vCurHPPos.x + 1.f, m_vCurHPPos.y);
			CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);
			CurHPDesc.vPosition = _float2(m_vCurHPPos.x, m_vCurHPPos.y - 1.f);
			CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);
			CurHPDesc.vPosition = _float2(m_vCurHPPos.x, m_vCurHPPos.y + 1.f);
			CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);
			// Origin
			CurHPDesc.vPosition = m_vCurHPPos;
			CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);

			// 최대 체력
			wstring strMaxHP = to_wstring(_int(m_fMaxHP));
			CRenderer::TEXT_DESC MaxHPDesc;
			MaxHPDesc.strText = strMaxHP;
			MaxHPDesc.strFontTag = L"Default_Bold";
			MaxHPDesc.vScale = { 0.4f, 0.4f };
			// OutLine
			MaxHPDesc.vPosition = _float2(m_vMaxHPPos.x - 1.f, m_vMaxHPPos.y);
			MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
			MaxHPDesc.vPosition = _float2(m_vMaxHPPos.x + 1.f, m_vMaxHPPos.y);
			MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
			MaxHPDesc.vPosition = _float2(m_vMaxHPPos.x, m_vMaxHPPos.y - 1.f);
			MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
			MaxHPDesc.vPosition = _float2(m_vMaxHPPos.x, m_vMaxHPPos.y + 1.f);
			MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
			// Origin
			MaxHPDesc.vPosition = m_vMaxHPPos;
			MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
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

//	if (m_iPass == 18)
//	{
//		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveAcc, sizeof(_float))))
//			return E_FAIL;
//
//		if (FAILED(m_pShaderCom->Bind_RawValue("g_SinValue", &m_fSinValue, sizeof(_float))))
//			return E_FAIL;
//
//		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fRatio, sizeof(_float))))
//			return E_FAIL;
//
//		if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture")))
//			return E_FAIL;
//	}

	if (FAILED(m_pGlowTextureCom->Bind_ShaderResource(m_pShaderCom, "g_GlowTexture", m_iFXTexIndex)))
		return E_FAIL;

	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_LerpTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iGaugeIndex)))
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
