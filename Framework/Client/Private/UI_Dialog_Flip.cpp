#include "stdafx.h"
#include "UI_Dialog_Flip.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Dialog_Flip::CUI_Dialog_Flip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DIALOGFLIP eType)
	: CUI(pDevice, pContext, L"UI_Dialog_Flip")
	, m_eType(eType)
{
}

CUI_Dialog_Flip::CUI_Dialog_Flip(const CUI_Dialog_Flip& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Dialog_Flip::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dialog_Flip::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	if (UIFLIP_ARROW == m_eType)
	{
		m_bArrived = false;
		m_fOriginY = m_tInfo.fY;
		m_fTargetY = m_tInfo.fY + 15.f;
	}
	
	return S_OK;
}

void CUI_Dialog_Flip::Tick(_float fTimeDelta)
{
	if (FLIPTYPE_END == m_eType)
		return;

	if (m_bActive)
	{
		if (UIFLIP_ARROW == m_eType)
		{
			if (!m_bArrived) // 다 내려가지 않았다
			{
				m_tInfo.fY += fTimeDelta * 15.f;

				if (m_fTargetY < m_tInfo.fY)
				{
					m_bArrived = true;
					m_tInfo.fY = m_fTargetY;
				}
			}
			else
			{
				m_tInfo.fY -= fTimeDelta * 8.f;

				if (m_fOriginY > m_tInfo.fY)
				{
					m_bArrived = false;
					m_tInfo.fY = m_fOriginY;
				}
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}


		if (UIFLIP_BUTTON == m_eType && m_bClicked)
		{
			m_fTimeAcc += fTimeDelta;
			m_iPass = 7; // FX 텍스처를 더하는 Pass로 변경한다.

			if (m_fTimeAcc > 0.2f)
			{
				m_bClicked = false;
				m_fTimeAcc = 0.f;
				m_iPass = 1;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Dialog_Flip::LateTick(_float fTimeDelta)
{
	if (FLIPTYPE_END == m_eType)
		return;

	if (m_bActive)
	{
		if (UIFLIP_ARROW == m_eType)
		{

		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Dialog_Flip::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Dialog_Flip::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Dialog_Flip::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_Dialog_Flip::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Dialog_Flip::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UIFLIP_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipArrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UIFLIP_BUTTON:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipBtn_Selected"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Dialog_Flip::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Dialog_Flip::Bind_ShaderResources()
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

	if (UIFLIP_BUTTON == m_eType)
	{
		if (7 == m_iPass)
		{
			if (nullptr == m_pFXTextureCom)
				return E_FAIL;

			if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
				return E_FAIL;
		}
	}

	return S_OK;
}

void CUI_Dialog_Flip::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (UIFLIP_BUTTON == m_eType && !m_bClicked)
			m_bClicked = true;

//		if (!m_bResize)
//		{
//			m_bResize = true;
//
//			m_tInfo.fCX = m_vMinSize.x;
//			m_tInfo.fCY = m_vMinSize.y;
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//		}
	}
}

CUI_Dialog_Flip* CUI_Dialog_Flip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DIALOGFLIP eType)
{
	CUI_Dialog_Flip* pInstance = new CUI_Dialog_Flip(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Dialog_Flip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog_Flip::Clone(void* pArg)
{
	CUI_Dialog_Flip* pInstance = new CUI_Dialog_Flip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Dialog_Flip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog_Flip::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
