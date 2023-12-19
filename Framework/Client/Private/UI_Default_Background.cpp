#include "stdafx.h"
#include "UI_Default_Background.h"
#include "UI_Default_BackStars.h"
#include "GameInstance.h"

CUI_Default_Background::CUI_Default_Background(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Default_Background")
{
}

CUI_Default_Background::CUI_Default_Background(const CUI_Default_Background& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Default_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_Background::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_bActive = false;

	// 효과를 생성한다.
	CGameObject* pStar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Common_Default_BackStars"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pStar)
		return E_FAIL;
	m_pEffect = dynamic_cast<CUI_Default_BackStars*>(pStar);
	m_pEffect->Set_Active(false);

	// Cloud
	Make_Child(0.f, 20.f, g_iWinSizeX, g_iWinSizeY, TEXT("Prototype_GameObject_UI_DefaultBackground_Cloud"));

	// Title Line
	_float fCY = 84.f;
	Make_Child(0.f, -(g_iWinSizeY * 0.5f) + fCY * 0.5f, g_iWinSizeX, 84.f, TEXT("Prototype_GameObject_UI_Common_TitleLine"));

	_float2 fLeftSize = _float2(128.f * 0.8f, 64.f * 0.6f);
	Make_Child(-(g_iWinSizeX * 0.5f) + fLeftSize.x * 0.5f, -(g_iWinSizeY * 0.5f) + fLeftSize.y * 0.5f,
		fLeftSize.x, fLeftSize.y, TEXT("Prototype_GameObject_UI_Common_MenuDeco_Left"));

	_float2 fRightSize = _float2(64.f * 0.6f, 64.f * 0.6f);
	Make_Child((g_iWinSizeX * 0.5f) - fRightSize.x * 0.5f, -(g_iWinSizeY * 0.5f) + fRightSize.y * 0.5f,
		fRightSize.x, fRightSize.y, TEXT("Prototype_GameObject_UI_Common_MenuDeco_Right"));

	_float2 fBtnSize = _float2(128.f * 0.6f, 128.f * 0.55f);
	Make_Child(-(g_iWinSizeX * 0.5f) + (fBtnSize.x * 0.5f + 3.f), -(g_iWinSizeY * 0.5f) + fBtnSize.y * 0.5f,
		fBtnSize.x, fBtnSize.y, TEXT("Prototype_GameObject_UI_Btn_Back"));

	return S_OK;
}

void CUI_Default_Background::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_pEffect->Get_Active())
			m_pEffect->Set_Active(true);
		else
			m_pEffect->Tick(fTimeDelta);

		
		__super::Tick(fTimeDelta);
	}
}

void CUI_Default_Background::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_pEffect->Get_Active())
			m_pEffect->LateTick(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Default_Background::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();

		if (m_pEffect->Get_Active())
			m_pEffect->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_Default_Background::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_DefaultBackground_WithCloud"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Default_Background::Ready_State()
{
	m_tInfo.fCX = g_iWinSizeX;
	m_tInfo.fCY = g_iWinSizeY;

	m_tInfo.fX = g_iWinSizeX * 0.5f;
	m_tInfo.fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Default_Background::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Default_Background * CUI_Default_Background::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Default_Background*	pInstance = new CUI_Default_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Default_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Default_Background::Clone(void* pArg)
{
	CUI_Default_Background*	pInstance = new CUI_Default_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Default_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Default_Background::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
	Safe_Release(m_pTextureCom);
}
