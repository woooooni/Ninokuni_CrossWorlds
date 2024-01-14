#include "stdafx.h"
#include "UI_PlayerHPBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_PlayerHPBar::CUI_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_PlayerHPBar")
{
}

CUI_PlayerHPBar::CUI_PlayerHPBar(const CUI_PlayerHPBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerHPBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bLerp = false;

	return S_OK;
}

void CUI_PlayerHPBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pPlayer)
		{
			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;

			CCharacter* pCurCharacter = pPlayer->Get_Character();
			if (nullptr == pCurCharacter)
				return;

			m_pPlayer = pCurCharacter;

			CCharacter::CHARACTER_STAT StatDesc = m_pPlayer->Get_Stat();
			m_fMaxHP = _float(StatDesc.iMaxHp);
			m_fPreHP = _float(StatDesc.iHp);
			m_fCurHP = m_fPreHP;
		}

		m_fTimeAcc += fTimeDelta * 0.1f;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 500.f;

			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_PlayerHPBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pPlayer)
			return;

		CCharacter::CHARACTER_STAT StatDesc = m_pPlayer->Get_Stat();

		m_fCurHP = _float(StatDesc.iHp);

		//if (CUI_Manager::GetInstance()->Is_FadeFinished())
		//{
			CRenderer::TEXT_DESC  DefaultDesc;
			DefaultDesc.strText = L"/";
			DefaultDesc.strFontTag = L"Default_Bold";
			DefaultDesc.vScale = { 0.35f, 0.35f };
			DefaultDesc.vPosition = m_vDefaultPosition;
			DefaultDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(DefaultDesc);

			CRenderer::TEXT_DESC CurHPDesc;
			wstring strCurHP = to_wstring(StatDesc.iHp);
			_float fOffsetX = (strCurHP.length() - 1) * 8.f;

			CurHPDesc.strText = strCurHP;
			CurHPDesc.strFontTag = L"Default_Bold";
			CurHPDesc.vScale = { 0.35f, 0.35f };
			CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - fOffsetX, m_vCurHPPosition.y);
			CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(CurHPDesc);

			CRenderer::TEXT_DESC MaxHPDesc;
			wstring strMaxHP = to_wstring(StatDesc.iMaxHp);

			MaxHPDesc.strText = to_wstring(StatDesc.iMaxHp);
			MaxHPDesc.strFontTag = L"Default_Bold";
			MaxHPDesc.vScale = { 0.35f, 0.35f };
			MaxHPDesc.vPosition = m_vMaxHPPosition;
			MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(MaxHPDesc);
		//}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PlayerHPBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar_Back"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_PlayerHPBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Bind_ShaderResources()
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

	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_LerpTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

//	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTimeAcc, sizeof(_float))))
//		return E_FAIL;

	return S_OK;
}

CUI_PlayerHPBar* CUI_PlayerHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerHPBar* pInstance = new CUI_PlayerHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerHPBar::Clone(void* pArg)
{
	CUI_PlayerHPBar* pInstance = new CUI_PlayerHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerHPBar::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
