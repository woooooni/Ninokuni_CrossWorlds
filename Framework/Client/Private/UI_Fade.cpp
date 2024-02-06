#include "stdafx.h"
#include "UI_Fade.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Fade::CUI_Fade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Fade")
{
}

CUI_Fade::CUI_Fade(const CUI_Fade& rhs)
	: CUI(rhs)
{
}

void CUI_Fade::Set_DefaultSetting()
{
	m_bIsComplete = true;
	m_fFadeTime = 0.f;
}

void CUI_Fade::Set_Fade(const _bool& pIsFadeOut, const _float& pFadeTime, const _bool& bIsWhite)
{
	// Initialize에 관련 코드 추가하기
	if (false == m_bIsComplete)
		return;

	// 02/06 텍스트 관련 디버깅 필요 -> 그랑프리 Intro에서 오작동
	CUI_Manager::GetInstance()->OnOff_TextUI(false);

	m_bIsFadeOut = pIsFadeOut;
	m_fAlpha = !(float)pIsFadeOut;
	m_fFadeTime = pFadeTime;
	m_bIsComplete = false;

	Set_White(bIsWhite);
}

void CUI_Fade::Set_Finish()
{
	m_bIsComplete = true;
	m_fFadeTime = 0.f;
	if (m_bIsFadeOut)
		m_fAlpha = 1.f;
	else
		m_fAlpha = 0.f;

	CUI_Manager::GetInstance()->OnOff_TextUI(true);
}

HRESULT CUI_Fade::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Fade::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_Fade::Tick(_float fTimeDelta)
{
	if (true == m_bIsComplete)
		return;

	if (false == m_bIsFadeOut && m_fAlpha > 0.f)
	{
		m_fAlpha -= fTimeDelta / m_fFadeTime;

		if (m_fAlpha <= 0.f)
			m_fAlpha = 0.f;
	}
	else if (false == m_bIsFadeOut)
	{
		Set_Finish();
	}
	else if (true == m_bIsFadeOut && m_fAlpha < 1.f)
	{
		m_fAlpha += fTimeDelta / m_fFadeTime;
		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;
	}
	else if (true == m_bIsFadeOut)
	{
		Set_Finish();
	}

	__super::Tick(fTimeDelta);
}

void CUI_Fade::LateTick(_float fTimeDelta)
{
//	if (true == m_bIsComplete)
//		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
}

HRESULT CUI_Fade::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Fade::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veils"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Fade::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Fade::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_bIsWhite))))
		return E_FAIL;

	return S_OK;
}

CUI_Fade* CUI_Fade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Fade* pInstance = new CUI_Fade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Fade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Fade::Clone(void* pArg)
{
	CUI_Fade* pInstance = new CUI_Fade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Fade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fade::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
