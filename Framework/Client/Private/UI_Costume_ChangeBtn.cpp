#include "stdafx.h"
#include "UI_Costume_ChangeBtn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Costume_ChangeBtn::CUI_Costume_ChangeBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Costume_ChangeBtn")
{
}

CUI_Costume_ChangeBtn::CUI_Costume_ChangeBtn(const CUI_Costume_ChangeBtn& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Costume_ChangeBtn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Costume_ChangeBtn::Initialize(void* pArg)
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

void CUI_Costume_ChangeBtn::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Costume_ChangeBtn::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (1 < m_iTextureIndex ||
			0 > m_iTextureIndex)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Costume_ChangeBtn::Render()
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

void CUI_Costume_ChangeBtn::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Costume_ChangeBtn::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_Costume_ChangeBtn::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Costume_ChangeBtn::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_ChangeBtn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Costume_ChangeBtn::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Costume_ChangeBtn::Bind_ShaderResources()
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

void CUI_Costume_ChangeBtn::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (0 == m_iTextureIndex)
			CUI_Manager::GetInstance()->Set_CostumeModel();
	}
}

CUI_Costume_ChangeBtn* CUI_Costume_ChangeBtn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Costume_ChangeBtn* pInstance = new CUI_Costume_ChangeBtn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Costume_ChangeBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Costume_ChangeBtn::Clone(void* pArg)
{
	CUI_Costume_ChangeBtn* pInstance = new CUI_Costume_ChangeBtn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Costume_ChangeBtn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Costume_ChangeBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
