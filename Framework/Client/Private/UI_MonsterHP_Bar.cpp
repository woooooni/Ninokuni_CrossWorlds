#include "stdafx.h"
#include "UI_MonsterHP_Bar.h"
#include "GameInstance.h"

CUI_MonsterHP_Bar::CUI_MonsterHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHPBar")
{
}

CUI_MonsterHP_Bar::CUI_MonsterHP_Bar(const CUI_MonsterHP_Bar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MonsterHP_Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	//m_bActive = true;
	m_bLerp = false;

	// TestCode
	m_fMaxHP = 1000.f;
	m_fCurHP = 1000.f;
	m_fPreHP = 1000.f;

	return S_OK;
}

void CUI_MonsterHP_Bar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

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

void CUI_MonsterHP_Bar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 기준점
//		CRenderer::TEXT_DESC  MAXHPDesc;
//		MAXHPDesc.strText = L"/";
//		MAXHPDesc.strFontTag = L"Default_Medium";
//		MAXHPDesc.vScale = { 0.4f, 0.4f };
//		MAXHPDesc.vPosition = m_vDefaultPosition;
//		MAXHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
//		m_pRendererCom->Add_Text(MAXHPDesc);
//
//		// Todo : 체력을 받아오게끔 구조 변경 필요함.
//		// 현재 체력 숫자 외곽선
//		CRenderer::TEXT_DESC CurHPDesc;
//		CurHPDesc.strText = L"1234";
//		CurHPDesc.strFontTag = L"Default_Medium";
//		CurHPDesc.vScale = { 0.4f, 0.4f };
//		CurHPDesc.vPosition = m_vCurHPPosition;
//		CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
//		m_pRendererCom->Add_Text(CurHPDesc);
//
//		// 최대 체력 숫자 외곽선
//		CRenderer::TEXT_DESC MaxHPDesc;
//		MaxHPDesc.strText = L"1235";
//		MaxHPDesc.strFontTag = L"Default_Medium";
//		MaxHPDesc.vScale = { 0.4f, 0.4f };
//		MaxHPDesc.vPosition = m_vMaxHPPosition;
//		MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
//		m_pRendererCom->Add_Text(MaxHPDesc);

		// AddText ( Monster Max HP, Monster Cur HP , 몬스터 이름 )

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MonsterHP_Bar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_WhiteBar"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Bar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Bind_ShaderResources()
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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_MonsterHP_Bar* CUI_MonsterHP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_Bar* pInstance = new CUI_MonsterHP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MonsterHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_Bar::Clone(void* pArg)
{
	CUI_MonsterHP_Bar* pInstance = new CUI_MonsterHP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MonsterHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
