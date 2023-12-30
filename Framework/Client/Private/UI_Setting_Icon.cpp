#include "stdafx.h"
#include "UI_Setting_Icon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Icon::CUI_Setting_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_ICONTYPE eType)
	: CUI(pDevice, pContext, L"UI_Setting_Icon")
	, m_eIconType(eType)
{
}

CUI_Setting_Icon::CUI_Setting_Icon(const CUI_Setting_Icon& rhs)
	: CUI(rhs)
	, m_eIconType(rhs.m_eIconType)
{
}

HRESULT CUI_Setting_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Icon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	if (SETICON_AUDIO == m_eIconType)
	{
		_float2 m_vDefaultPos = _float2((g_iWinSizeX * 0.5f) + 330.f, (g_iWinSizeY * 0.5f) - 50.f);
		Make_Child(m_vDefaultPos.x, m_vDefaultPos.y, 957.f, 580.f, TEXT("Prototype_GameObject_UI_Setting_Section_Audio"));

		// 메뉴 버튼
//		_float2 vDefaultPos = _float2(-330.f, -110.f);
//		_float2 vSize = _float2(220.f, 64.f);
//		_float fOffset = 80.f;
//		Make_Child(vDefaultPos.x, vDefaultPos.y, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Game"));
//		Make_Child(vDefaultPos.x, vDefaultPos.y + fOffset, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Graphic"));
//		Make_Child(vDefaultPos.x, vDefaultPos.y + (fOffset * 2.f), vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Audio"));
	}
	
	return S_OK;
}

void CUI_Setting_Icon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bClicked)
			m_iPass = 7; // 선택이 되었으면 FXTexture를 같이 던진다.
		else
			m_iPass = 1;

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Icon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		if (m_bClicked)
			__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Setting_Icon::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();

		if (m_bClicked)
			__super::Render();
	}

	return S_OK;
}

void CUI_Setting_Icon::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Setting_Icon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_Setting_Icon::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Setting_Icon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Button"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Selected"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Icon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eIconType))))
		return E_FAIL;

	if (m_bClicked && m_iPass == 7)
	{
		if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_Setting_Icon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bClicked)
		{
			m_bClicked = true;

			__super::Set_ChildActive(true);
		}
		else
		{
			m_bClicked = false;

			__super::Set_ChildActive(false);
		}
	}
}

CUI_Setting_Icon* CUI_Setting_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_ICONTYPE eType)
{
	CUI_Setting_Icon* pInstance = new CUI_Setting_Icon(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Icon::Clone(void* pArg)
{
	CUI_Setting_Icon* pInstance = new CUI_Setting_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
