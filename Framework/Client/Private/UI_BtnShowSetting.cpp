#include "stdafx.h"
#include "UI_BtnShowSetting.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_BtnShowSetting::CUI_BtnShowSetting(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnShowSetting")
{
}

CUI_BtnShowSetting::CUI_BtnShowSetting(const CUI_BtnShowSetting& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BtnShowSetting::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnShowSetting::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_BtnShowSetting::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnShowSetting::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnShowSetting::Render()
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

void CUI_BtnShowSetting::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_BtnShowSetting::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			// 베일을 On한다.
			// 옵션창을 On한다.

			CUI_Manager::GetInstance()->OnOff_SettingWindow(true);
//			CUI_Manager::GetInstance()->OnOff_MainMenu(true);
//			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
//			CUI_Manager::GetInstance()->OnOff_CloseButton(true);

			// 닫기 버튼을 활성화한다.
//			m_bActive = false;
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_BtnShowSetting::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_BtnShowSetting::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Icon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnShowSetting::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnShowSetting::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_BtnShowSetting* CUI_BtnShowSetting::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnShowSetting* pInstance = new CUI_BtnShowSetting(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnShowSetting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnShowSetting::Clone(void* pArg)
{
	CUI_BtnShowSetting* pInstance = new CUI_BtnShowSetting(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnShowSetting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnShowSetting::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
