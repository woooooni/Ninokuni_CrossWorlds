#include "stdafx.h"
#include "UI_Tutorial_Window.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_Tutorial_Window::CUI_Tutorial_Window(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Tutorial_Window")
{
}

CUI_Tutorial_Window::CUI_Tutorial_Window(const CUI_Tutorial_Window& rhs)
	: CUI(rhs)
{
}

void CUI_Tutorial_Window::Set_Active(_bool bActive)
{
	if (bActive)
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;

		pPlayer->Get_Character()->Set_Skill_Input(false);
		pPlayer->Get_Character()->Set_Attack_Input(false);
		pPlayer->Get_Character()->Set_Move_Input(false);

		m_fAlpha = 0.8f;
		m_bActive = true;

		m_bActive = true;
		m_bFadeOut = false;
	}
	else
	{
		m_bFadeOut = true;
	}
}

HRESULT CUI_Tutorial_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Tutorial_Window::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.6f;
	m_bActive = false;

	return S_OK;
}

void CUI_Tutorial_Window::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (true == m_bFadeOut)
		{
			m_fAlpha -= fTimeDelta;

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				m_bFadeOut = false;
				m_bActive = false;

				CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
				if (nullptr == pPlayer)
					return;
				pPlayer->Get_Character()->Set_Skill_Input(true);
				pPlayer->Get_Character()->Set_Attack_Input(true);
				pPlayer->Get_Character()->Set_Move_Input(true);
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Tutorial_Window::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
	}
}

HRESULT CUI_Tutorial_Window::Render()
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

HRESULT CUI_Tutorial_Window::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_KeyInput_TutorialWindow"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Tutorial_Window::Ready_State()
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

HRESULT CUI_Tutorial_Window::Bind_ShaderResources()
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

CUI_Tutorial_Window * CUI_Tutorial_Window::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Tutorial_Window*	pInstance = new CUI_Tutorial_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Tutorial_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Tutorial_Window::Clone(void* pArg)
{
	CUI_Tutorial_Window*	pInstance = new CUI_Tutorial_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Tutorial_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Tutorial_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
