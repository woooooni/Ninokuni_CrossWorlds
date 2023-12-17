#include "stdafx.h"
#include "UI_Setting_Window.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Window::CUI_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Setting_Window")
{
}

CUI_Setting_Window::CUI_Setting_Window(const CUI_Setting_Window& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Setting_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Window::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 1.f;

	// 닫기 버튼
	_float fSize = 64.f * 0.5f;
	Make_Child(425.f, -245.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Common_Btn_Close"));

	// 메뉴 버튼
	_float2 vDefaultPos = _float2(-330.f, -110.f);
	_float2 vSize = _float2(220.f, 64.f);
	_float fOffset = 80.f;
	Make_Child(vDefaultPos.x, vDefaultPos.y, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Game"));
	Make_Child(vDefaultPos.x, vDefaultPos.y + fOffset, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Graphic"));
	Make_Child(vDefaultPos.x, vDefaultPos.y + (fOffset * 2.f), vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Audio"));

	return S_OK;
}

void CUI_Setting_Window::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Window::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Setting_Window::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_Setting_Window::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Window_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Window::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Window::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Setting_Window* CUI_Setting_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Setting_Window* pInstance = new CUI_Setting_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Window::Clone(void* pArg)
{
	CUI_Setting_Window* pInstance = new CUI_Setting_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
