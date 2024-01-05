#include "stdafx.h"
#include "UI_Costume_Btn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Costume_Btn::CUI_Costume_Btn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_COSTUMEBTN_TYPE eButtonType, UI_COSTUMEBTN eUIType)
	: CUI(pDevice, pContext, L"UI_Costume_Button")
	, m_eUIType(eUIType)
	, m_eButtonType(eButtonType)
{
}

CUI_Costume_Btn::CUI_Costume_Btn(const CUI_Costume_Btn& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
	, m_eButtonType(rhs.m_eButtonType)
{
}

void CUI_Costume_Btn::Set_Active(_bool bActive)
{
	if (COSTUMEBTN_END == m_eUIType)
		return;

	if (false == bActive)
	{
		if (m_bEvent)
			m_bEvent = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Costume_Btn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Costume_Btn::Initialize(void* pArg)
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

void CUI_Costume_Btn::Tick(_float fTimeDelta)
{
	if (COSTUMEBTN_END == m_eUIType)
		return;

	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Costume_Btn::LateTick(_float fTimeDelta)
{
	if (COSTUMEBTN_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (COSTUMEBTN_UNCLICKED == m_eButtonType)
		{
			if (COSTUME_CLOTH == m_eUIType)
			{
				m_fAlpha = 1.f;
			}
			else if (COSTUME_HAIRACC == m_eUIType)
			{
				m_fAlpha = 1.f;
			}
			else
				m_fAlpha = 0.3f;
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Costume_Btn::Render()
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

void CUI_Costume_Btn::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_Costume_Btn::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			if (COSTUME_CLOTH == m_eUIType || COSTUME_HAIRACC == m_eUIType)
			{
				CUI_Manager::GetInstance()->Update_CostumeBtnState(_uint(m_eUIType));
				CUI_Manager::GetInstance()->OnOff_CostumeSlot(_uint(m_eUIType), true);
			}
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Costume_Btn::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Costume_Btn::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eButtonType)
	{
	case UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Unclicked_Buttons"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Clicked_Buttons"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Costume_Btn::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f)); // Z값 1로 설정하면 Background에 가려짐

	return S_OK;
}

HRESULT CUI_Costume_Btn::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eUIType))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Costume_Btn* CUI_Costume_Btn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_COSTUMEBTN_TYPE eButtonType, UI_COSTUMEBTN eUIType)
{
	CUI_Costume_Btn* pInstance = new CUI_Costume_Btn(pDevice, pContext, eButtonType, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Costume_Btn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Costume_Btn::Clone(void* pArg)
{
	CUI_Costume_Btn* pInstance = new CUI_Costume_Btn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Costume_Btn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Costume_Btn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
