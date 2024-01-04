#include "stdafx.h"
#include "UI_ImajinnSection_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_ImajinnSection_Slot::CUI_ImajinnSection_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_IMAJINNSLOT eSlotType)
	: CUI(pDevice, pContext, L"UI_ImajinnSection_Slot")
	, m_eSlotType(eSlotType)
{
}

CUI_ImajinnSection_Slot::CUI_ImajinnSection_Slot(const CUI_ImajinnSection_Slot& rhs)
	: CUI(rhs)
	, m_eSlotType(rhs.m_eSlotType)
{
}

HRESULT CUI_ImajinnSection_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Slot::Initialize(void* pArg)
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

void CUI_ImajinnSection_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_ImajinnSection_Slot::Render()
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

void CUI_ImajinnSection_Slot::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_ImajinnSection_Slot::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_ImajinnSection_Slot::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_ImajinnSection_Slot::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Slot"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_ImajinnSection_Slot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_Slot::Bind_ShaderResources()
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

void CUI_ImajinnSection_Slot::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 이마진이 있는 상태, 이마진이 없는 상태를 나눈다.
	}
}

CUI_ImajinnSection_Slot* CUI_ImajinnSection_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_IMAJINNSLOT eSlotType)
{
	CUI_ImajinnSection_Slot* pInstance = new CUI_ImajinnSection_Slot(pDevice, pContext, eSlotType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_Slot::Clone(void* pArg)
{
	CUI_ImajinnSection_Slot* pInstance = new CUI_ImajinnSection_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
