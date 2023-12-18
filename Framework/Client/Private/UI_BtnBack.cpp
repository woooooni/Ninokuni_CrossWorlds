#include "stdafx.h"
#include "UI_BtnBack.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_BtnBack::CUI_BtnBack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnBack")
{
}

CUI_BtnBack::CUI_BtnBack(const CUI_BtnBack& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BtnBack::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnBack::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	
	return S_OK;
}

void CUI_BtnBack::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnBack::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnBack::Render()
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

void CUI_BtnBack::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_BtnBack::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_BtnBack::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_BtnBack::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_BackBtn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnBack::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnBack::Bind_ShaderResources()
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

void CUI_BtnBack::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		CUI_Manager::GetInstance()->Using_CloseButton();
	}
}

CUI_BtnBack* CUI_BtnBack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnBack* pInstance = new CUI_BtnBack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnBack::Clone(void* pArg)
{
	CUI_BtnBack* pInstance = new CUI_BtnBack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnBack::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
