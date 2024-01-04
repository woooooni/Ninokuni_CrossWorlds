#include "stdafx.h"
#include "UI_ImajinnSection_Vehicle.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_ImajinnSection_Vehicle::CUI_ImajinnSection_Vehicle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_ImajinnSection_Vehicle")
{
}

CUI_ImajinnSection_Vehicle::CUI_ImajinnSection_Vehicle(const CUI_ImajinnSection_Vehicle& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ImajinnSection_Vehicle::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Vehicle::Initialize(void* pArg)
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

void CUI_ImajinnSection_Vehicle::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_Vehicle::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_ImajinnSection_Vehicle::Render()
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

void CUI_ImajinnSection_Vehicle::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_ImajinnSection_Vehicle::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_ImajinnSection_Vehicle::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_ImajinnSection_Vehicle::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Vehicle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_ImajinnSection_Vehicle::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_Vehicle::Bind_ShaderResources()
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

void CUI_ImajinnSection_Vehicle::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 사용할 수 있는 이모티콘이 담긴 PopUp이 뜬다.
	}
}

CUI_ImajinnSection_Vehicle* CUI_ImajinnSection_Vehicle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ImajinnSection_Vehicle* pInstance = new CUI_ImajinnSection_Vehicle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_Vehicle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_Vehicle::Clone(void* pArg)
{
	CUI_ImajinnSection_Vehicle* pInstance = new CUI_ImajinnSection_Vehicle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_Vehicle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_Vehicle::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
