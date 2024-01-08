#include "stdafx.h"
#include "UI_Boss_NameTag.h"
#include "GameInstance.h"

CUI_Boss_NameTag::CUI_Boss_NameTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BossHP_Background")
{
}

CUI_Boss_NameTag::CUI_Boss_NameTag(const CUI_Boss_NameTag& rhs)
	: CUI(rhs)
{
}

void CUI_Boss_NameTag::Set_Active(_bool bActive)
{
	if (false == bActive)
	{
		//m_fTimeAcc = 0.f;
		//m_iTextureIndex = 0;
		//m_bDone = false;
		//m_fAlpha = 1.f;
	}
	else
	{
		m_fTimeAcc = 0.f;
		m_iTextureIndex = 0;
		m_bDone = false;
		m_fAlpha = 1.f;
	}
	m_bActive = bActive;
}

HRESULT CUI_Boss_NameTag::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_NameTag::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_iTextureIndex = 0;
	m_bActive = false;

	return S_OK;
}

void CUI_Boss_NameTag::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_iTextureIndex > 6)
			return;

		if (!m_bDone)
		{
			m_fTimeAcc += fTimeDelta;

			if (m_fTimeAcc >= 0.2f)
			{
				if (6 <= m_iTextureIndex)
				{
					m_bDone = true;

					m_fTimeAcc = 0.f;
					m_iTextureIndex = 6;
				}
				else
				{
					m_fTimeAcc = 0.f;
					m_iTextureIndex++;
				}
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Boss_NameTag::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_iTextureIndex > 6)
			return;

		if (m_bDone)
		{
			if (m_fTimeAcc > 1.f)
			{
				m_fAlpha -= fTimeDelta * 0.5f;

				if (0 >= m_fAlpha)
				{
					m_fTimeAcc = 0.f;

					m_fAlpha = 0.f;
					m_bActive = false;
				}
			}
			else
			{
				m_fTimeAcc += fTimeDelta;
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Boss_NameTag::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Boss_NameTag::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_NameTag"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_NameTag::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Boss_NameTag::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_Boss_NameTag* CUI_Boss_NameTag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_NameTag* pInstance = new CUI_Boss_NameTag(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Boss_NameTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Boss_NameTag::Clone(void* pArg)
{
	CUI_Boss_NameTag* pInstance = new CUI_Boss_NameTag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Boss_NameTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_NameTag::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
