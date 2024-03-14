#include "stdafx.h"
#include "UI_Loading_Character.h"
#include "GameInstance.h"

CUI_Loading_Character::CUI_Loading_Character(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Loading_Character")
{
}

CUI_Loading_Character::CUI_Loading_Character(const CUI_Loading_Character& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading_Character::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Character::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.1f;

	m_bArrived = false;
	m_bAlpha = false;
	m_bHover = false;
	m_bTurn = false;

	m_vArrivedPos.x = m_tInfo.fX;
	m_vArrivedPos.y = m_tInfo.fY;

	m_vStartPos.x = m_tInfo.fX - (m_tInfo.fCX * 0.5f);
	m_vStartPos.y = m_vArrivedPos.y;

	m_tInfo.fX = m_vStartPos.x;
	m_tInfo.fY = m_vStartPos.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	m_vHoverPos.x = m_vArrivedPos.x + 50.f;
	m_vHoverPos.y = m_vArrivedPos.y + 50.f;

	m_vTurnPos.x = m_vArrivedPos.x - 50.f;
	m_vTurnPos.y = m_vArrivedPos.y + 50.f;

	return S_OK;
}

void CUI_Loading_Character::Tick(_float fTimeDelta)
{
	if (0 > m_iTextureIndex || 4 < m_iTextureIndex)
		return;

	if (false == m_bArrived)
	{
		m_tInfo.fX += fTimeDelta * m_fSpeed;

		if (false == m_bAlpha)
		{
			m_fAlpha += fTimeDelta;

			if (1.f < m_fAlpha)
			{
				m_fAlpha = 1.f;
				m_bAlpha = true;
			}
		}

		if (m_tInfo.fX > m_vArrivedPos.x)
		{
			m_tInfo.fX = m_vArrivedPos.x;
			m_bArrived = true;
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}

	__super::Tick(fTimeDelta);

}

void CUI_Loading_Character::LateTick(_float fTimeDelta)
{
	if (0 > m_iTextureIndex || 4 < m_iTextureIndex)
		return;

	if (true == m_bArrived)
		Move_ToHoverPosition(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_Character::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading_Character::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Characters_New"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_Character::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_Character::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_Loading_Character::Move_ToHoverPosition(_float fTimeDelta)
{
	if (false == m_bTurn)
	{
		if (false == m_bHover)
		{
			if (m_tInfo.fX > m_vHoverPos.x)
			{
				m_bHover = true;
				m_tInfo.fX = m_vHoverPos.x;
			}
			else
			{
				m_tInfo.fX += fTimeDelta * 30.f;
				m_tInfo.fY += fTimeDelta * 30.f;
			}
		}
		else
		{
			if (m_tInfo.fX < m_vArrivedPos.x)
			{
				m_bHover = false;
				m_bTurn = true;
				m_tInfo.fX = m_vArrivedPos.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * 30.f;
				m_tInfo.fY -= fTimeDelta * 30.f;
			}
		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}
	else
	{
		if (false == m_bHover)
		{
			if (m_tInfo.fX < m_vTurnPos.x)
			{
				m_bHover = true;
				m_tInfo.fX = m_vTurnPos.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * 30.f;
				m_tInfo.fY += fTimeDelta * 30.f;
			}
		}
		else
		{
			if (m_tInfo.fX > m_vArrivedPos.x)
			{
				m_bHover = false;
				m_bTurn = false;
				m_tInfo.fX = m_vArrivedPos.x;
			}
			else
			{
				m_tInfo.fX += fTimeDelta * 30.f;
				m_tInfo.fY -= fTimeDelta * 30.f;
			}
		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}
}

CUI_Loading_Character* CUI_Loading_Character::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Loading_Character* pInstance = new CUI_Loading_Character(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Loading_Character");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Loading_Character::Clone(void* pArg)
{
	CUI_Loading_Character* pInstance = new CUI_Loading_Character(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Loading_Character");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_Character::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
