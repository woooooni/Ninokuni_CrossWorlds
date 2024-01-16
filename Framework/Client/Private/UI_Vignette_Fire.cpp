#include "stdafx.h"
#include "UI_Vignette_Fire.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Vignette_Fire::CUI_Vignette_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Vignette_Ice")
{
}

CUI_Vignette_Fire::CUI_Vignette_Fire(const CUI_Vignette_Fire& rhs)
	: CUI(rhs)
{
}

void CUI_Vignette_Fire::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		m_fAlpha = 0.f;
		m_bSetAlpha = false;
	}
	else
	{
		CUI_Manager::GetInstance()->Update_IceVignette();
	}

	m_bActive = bActive;
}

HRESULT CUI_Vignette_Fire::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Vignette_Fire::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Vignette_Fire::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (true == m_bSetAlpha)
		{
			// 알파값이 줄어들다가 m_bActive false로 돌린다
			m_fAlpha -= fTimeDelta * 1.2f;

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				Set_Active(false);
			}
		}
		else
		{
			m_fAlpha += fTimeDelta * 1.2f;

			if(m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bSetAlpha = true;
			}
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_Vignette_Fire::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_EFFECT_BLEND, this);
	}
}

HRESULT CUI_Vignette_Fire::Render()
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

HRESULT CUI_Vignette_Fire::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Vignette_Fire"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Vignette_Fire::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Vignette_Fire::Bind_ShaderResources()
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

CUI_Vignette_Fire* CUI_Vignette_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Vignette_Fire* pInstance = new CUI_Vignette_Fire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Vignette_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Vignette_Fire::Clone(void* pArg)
{
	CUI_Vignette_Fire* pInstance = new CUI_Vignette_Fire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Vignette_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Vignette_Fire::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
