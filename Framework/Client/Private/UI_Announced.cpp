#include "stdafx.h"
#include "UI_Announced.h"
#include "GameInstance.h"
#include "UI_Manager.h"

// FXTextureCom-> Alpha
CUI_Announced::CUI_Announced(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_ANNNOUNCE_TYPE eType)
	: CUI(pDevice, pContext, L"UI_Announced")
	, m_eType(eType)
{
}

CUI_Announced::CUI_Announced(const CUI_Announced& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Announced::Set_Active(_bool bActive, _int iMagicNum)
{
	if (bActive)
	{
		if (iMagicNum == m_iTextureIndex)
			return;

		m_iTextureIndex = iMagicNum;
		m_fAlpha = 0.7f;
		m_bAlpha = false;
	}
	else
	{
		m_fTimeAcc = 0.f;
	}

	m_bActive = bActive;
}

HRESULT CUI_Announced::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Announced::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.7f;
	m_bAlpha = false;

//	if (ANNOUNCE_CAMERA == m_eType)
//	{
//		m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
//		m_vStartPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + 100.f);
//	}

	return S_OK;
}

void CUI_Announced::Tick(_float fTimeDelta)
{
	if (ANNOUNCED_END == m_eType)
		return;

	if (m_bActive)
	{
		if (ANNOUNCE_CAMERA == m_eType)
			Tick_CameraWindow(fTimeDelta);
		else if (ANNOUNCE_SKILL == m_eType)
			Tick_SkillWindow(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Announced::LateTick(_float fTimeDelta)
{
	if (ANNOUNCED_END == m_eType)
		return;

	if (m_bActive)
	{
		if (ANNOUNCE_CAMERA == m_eType)
			LateTick_CameraWindow(fTimeDelta);
		else if (ANNOUNCE_SKILL == m_eType)
			LateTick_SkillWindow(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Announced::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(7);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Announced::Tick_CameraWindow(_float fTimeDelta)
{
	if (!m_bAlpha)
	{
		m_fAlpha -= fTimeDelta;

		if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_bAlpha = true;
			m_bActive = false;
		}
	}
}

void CUI_Announced::Tick_SkillWindow(_float fTimeDelta)
{
	if (false == m_bAlpha)
	{
		m_fTimeAcc += fTimeDelta;

		if (1.f < m_fTimeAcc)
		{
			m_fAlpha -= fTimeDelta;

			if (m_fAlpha < 0.f)
			{
				m_fAlpha = 0.f;
				m_bAlpha = true;
				Set_Active(false);
			}
		}
	}
}

void CUI_Announced::LateTick_CameraWindow(_float fTimeDelta)
{

}

void CUI_Announced::LateTick_SkillWindow(_float fTimeDelta)
{
}

HRESULT CUI_Announced::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_ANNNOUNCE_TYPE::ANNOUNCE_CAMERA:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CameraPopup"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_CameraPopup"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
			return E_FAIL;
		break;

	case UI_ANNNOUNCE_TYPE::ANNOUNCE_SKILL:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillPopup"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_CameraPopup"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
			return E_FAIL;
		break;

	}

	
	return S_OK;
}

HRESULT CUI_Announced::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Announced::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (ANNOUNCE_CAMERA == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
		if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
		if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
	}


	return S_OK;
}

CUI_Announced* CUI_Announced::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_ANNNOUNCE_TYPE eType)
{
	CUI_Announced* pInstance = new CUI_Announced(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Announced");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Announced::Clone(void* pArg)
{
	CUI_Announced* pInstance = new CUI_Announced(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Announced");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Announced::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
