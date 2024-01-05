#include "stdafx.h"
#include "UI_Setting_BtnVolume.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Setting_Slider.h"

CUI_Setting_BtnVolume::CUI_Setting_BtnVolume(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_BTNTYPE eType)
	: CUI(pDevice, pContext, L"UI_Setting_Button")
	, m_eType(eType)
{
}

CUI_Setting_BtnVolume::CUI_Setting_BtnVolume(const CUI_Setting_BtnVolume& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Setting_BtnVolume::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_BtnVolume::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	m_vOriginSize.x = m_tInfo.fCX;
	m_vOriginSize.y = m_tInfo.fCY;

	m_vMinSize.x = m_vOriginSize.x * 0.8f;
	m_vMinSize.y = m_vOriginSize.y * 0.8f;

	if (BTNTYPE_END == m_eType)
		return E_FAIL;
	
	return S_OK;
}

void CUI_Setting_BtnVolume::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bResize)
			m_fTimeAcc += fTimeDelta;

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_BtnVolume::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_fTimeAcc > 0.1f)
		{
			m_bResize = false;
			m_fTimeAcc = 0.f;

			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Setting_BtnVolume::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Setting_BtnVolume::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Setting_BtnVolume::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Setting_BtnVolume::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Setting_BtnVolume::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case SETBTN_MINUS:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_MinusBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case SETBTN_PLUS:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_PlusBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Setting_BtnVolume::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_BtnVolume::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Setting_BtnVolume::Key_Input(_float fTimeDelta)
{
	if (BTNTYPE_END == m_eType)
		return;

	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bResize)
		{
			m_bResize = true;

			m_tInfo.fCX = m_vMinSize.x;
			m_tInfo.fCY = m_vMinSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		if (SETBTN_PLUS == m_eType)
		{
			// Plus버튼을 눌렀을 때
		}
		else
		{
			// Minus버튼을 눌렀을 때
		}
	}
}

CUI_Setting_BtnVolume* CUI_Setting_BtnVolume::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_BTNTYPE eType)
{
	CUI_Setting_BtnVolume* pInstance = new CUI_Setting_BtnVolume(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_BtnVolume");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_BtnVolume::Clone(void* pArg)
{
	CUI_Setting_BtnVolume* pInstance = new CUI_Setting_BtnVolume(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_BtnVolume");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_BtnVolume::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
