#include "stdafx.h"
#include "UI_PlayerEXPBar.h"
#include "GameInstance.h"

CUI_PlayerEXPBar::CUI_PlayerEXPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EXPTYPE eType)
	: CUI(pDevice, pContext, L"UI_PlayerEXPBar")
	, m_eType(eType)
{
}

CUI_PlayerEXPBar::CUI_PlayerEXPBar(const CUI_PlayerEXPBar& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_PlayerEXPBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vHidePosition.x = m_tInfo.fX;
	m_vHidePosition.y = g_iWinSizeY + m_tInfo.fCY * 0.5f;

	return S_OK;
}

void CUI_PlayerEXPBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 현재 경험치를 받아와서 갱신한다.
		// 경험치가 늘어난만큼 왼쪽으로 이동해야한다.
		Movement_BasedOnHiding(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_PlayerEXPBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PlayerEXPBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UIEXP_BACKGROUND:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UIEXP_BAR:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Bar"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Bind_ShaderResources()
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

void CUI_PlayerEXPBar::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (m_tInfo.fY >= m_vHidePosition.y)
			{
				m_bHideFinish = true;
				m_tInfo.fY = m_vHidePosition.y;
			}
			else
			{
				m_tInfo.fY += fTimeDelta * m_fHideSpeed;
			}
		}
		else // 드러낸다
		{
			if (m_tInfo.fY <= m_vOriginPosition.y)
			{
				m_bHideFinish = true;
				m_tInfo.fY = m_vOriginPosition.y;
			}
			else
			{
				m_tInfo.fY -= fTimeDelta * m_fHideSpeed;
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_PlayerEXPBar* CUI_PlayerEXPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EXPTYPE eType)
{
	CUI_PlayerEXPBar* pInstance = new CUI_PlayerEXPBar(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PlayerEXPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerEXPBar::Clone(void* pArg)
{
	CUI_PlayerEXPBar* pInstance = new CUI_PlayerEXPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PlayerEXPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerEXPBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
