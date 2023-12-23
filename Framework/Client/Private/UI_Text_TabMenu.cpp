#include "stdafx.h"
#include "UI_Text_TabMenu.h"
#include "GameInstance.h"

CUI_Text_TabMenu::CUI_Text_TabMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Text_TabMenu")
{
}

CUI_Text_TabMenu::CUI_Text_TabMenu(const CUI_Text_TabMenu& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Text_TabMenu::Set_TextType(UI_MENUTITLE eType)
{
	m_eType = eType;
	_float2 vOffset = _float2(80.f, 5.f);

	switch (m_eType)
	{
	case TITLE_COSTUME:
		m_tInfo.fCX = 200.f * 0.7f;
		m_tInfo.fCY = 93.f * 0.7f;
		m_tInfo.fX = m_tInfo.fCX * 0.5f + vOffset.x;
		m_tInfo.fY = m_tInfo.fCY * 0.5f + vOffset.y;
		m_bShowChild = false;
		break;

	case TITLE_WORLDMAP:
		m_tInfo.fCX = 200.f * 0.7f;
		m_tInfo.fCY = 93.f * 0.7f;
		m_tInfo.fX = m_tInfo.fCX * 0.5f + vOffset.x;
		m_tInfo.fY = m_tInfo.fCY * 0.5f + vOffset.y;
		m_bShowChild = true;
		break;

	default:
		break;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

}

HRESULT CUI_Text_TabMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL; 

	return S_OK;
}

HRESULT CUI_Text_TabMenu::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;
	
	m_bActive = false;

	_float2 vWorldMapScale = _float2(200.f * 0.7f, 93.f * 0.7f);
	_float2 vOffset = _float2(80.f, 5.f);

	_float2 fLeftSize = _float2(128.f * 0.8f, 64.f * 0.6f);
	Make_Child(-1.f * (vWorldMapScale.x * 0.7f),
		-1.f * vWorldMapScale.y * 0.3f + 2.f,
		fLeftSize.x, fLeftSize.y, TEXT("Prototype_GameObject_UI_Common_MenuDeco_Left"));

	_float2 fBtnSize = _float2(128.f * 0.6f, 128.f * 0.55f);
	Make_Child(-0.5f * fBtnSize.x - 67.f,
		-0.5 * fBtnSize.y + 36.f,
		fBtnSize.x, fBtnSize.y, TEXT("Prototype_GameObject_UI_Btn_Back"));

	return S_OK;
}

void CUI_Text_TabMenu::Tick(_float fTimeDelta)
{
	if (UITITLE_END == m_eType)
		return;

	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Text_TabMenu::LateTick(_float fTimeDelta)
{
	if (UITITLE_END == m_eType)
		return;

	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		if (m_bShowChild)
			__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Text_TabMenu::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	if (m_bShowChild)
		__super::Render();

	return S_OK;
}

HRESULT CUI_Text_TabMenu::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleText"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Text_TabMenu::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Text_TabMenu::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Text_TabMenu* CUI_Text_TabMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Text_TabMenu* pInstance = new CUI_Text_TabMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Text_TabMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Text_TabMenu::Clone(void* pArg)
{
	CUI_Text_TabMenu* pInstance = new CUI_Text_TabMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Text_TabMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Text_TabMenu::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
