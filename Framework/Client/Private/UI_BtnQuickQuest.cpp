#include "stdafx.h"
#include "UI_BtnQuickQuest.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_BtnQuickQuest::CUI_BtnQuickQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnQuickQuest")
{
}

CUI_BtnQuickQuest::CUI_BtnQuickQuest(const CUI_BtnQuickQuest& rhs)
	: CUI(rhs)
{
}

void CUI_BtnQuickQuest::Set_Click(_bool bClick)
{
	if (bClick)
	{
		m_iTextureIndex = 1;
		CUI_Manager::GetInstance()->OnOff_QuestPopup(true);
	}
	else
	{
		m_iTextureIndex = 0;
		CUI_Manager::GetInstance()->OnOff_QuestPopup(false);
	}

	m_bClick = bClick;
}

HRESULT CUI_BtnQuickQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Initialize(void* pArg)
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
	m_vHidePosition.x = -1.f * m_tInfo.fCX * 0.5f;
	m_vHidePosition.y = m_tInfo.fY;

	m_bUseMouse = true;

	return S_OK;
}

void CUI_BtnQuickQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);
		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnQuickQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnQuickQuest::Render()
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

void CUI_BtnQuickQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_BtnQuickQuest::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_BtnQuickQuest::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_BtnQuickQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowQuest"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Bind_ShaderResources()
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

void CUI_BtnQuickQuest::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (0 == m_iTextureIndex)
		{
			if (0 < CUI_Manager::GetInstance()->Get_QuestNum())
			{
				Set_Click(true);
			}
		}
		else if (1 == m_iTextureIndex)
		{
			Set_Click(false);
		}
		else
		{
			return;
		}
	}
}

void CUI_BtnQuickQuest::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (m_tInfo.fX <= m_vHidePosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vHidePosition.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * m_fHideSpeed;

				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;
				else
					m_fAlpha -= fTimeDelta;
			}
		}
		else // 드러낸다
		{
			if (m_tInfo.fX >= m_vOriginPosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vOriginPosition.x;
			}
			else
			{
				m_tInfo.fX += fTimeDelta * m_fHideSpeed;

				if (m_fAlpha >= 1.f)
					m_fAlpha = 1.f;
				else
					m_fAlpha += fTimeDelta;
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_BtnQuickQuest* CUI_BtnQuickQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnQuickQuest* pInstance = new CUI_BtnQuickQuest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnQuickQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnQuickQuest::Clone(void* pArg)
{
	CUI_BtnQuickQuest* pInstance = new CUI_BtnQuickQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnQuickQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnQuickQuest::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
