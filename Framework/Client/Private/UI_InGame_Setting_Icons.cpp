#include "stdafx.h"
#include "UI_InGame_Setting_Icons.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_InGame_Setting_Icons::CUI_InGame_Setting_Icons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INGAMEETC eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Icons")
	, m_eType(eType)
{
}

CUI_InGame_Setting_Icons::CUI_InGame_Setting_Icons(const CUI_InGame_Setting_Icons& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_InGame_Setting_Icons::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Icons::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bUseMouse = true;

	if (CAMERA_DEFAULT == m_eType)
	{
		m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
		m_vMaxSize = _float2(m_tInfo.fCX * 1.2f, m_tInfo.fCY * 1.2f);
	}

	return S_OK;
}

void CUI_InGame_Setting_Icons::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_Icons::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_InGame_Setting_Icons::Render()
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

void CUI_InGame_Setting_Icons::On_MouseEnter(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (m_eType == CAMERA_DEFAULT)
		{
			m_tInfo.fCX = m_vMaxSize.x;
			m_tInfo.fCY = m_vMaxSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}
	}
}

void CUI_InGame_Setting_Icons::On_Mouse(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_InGame_Setting_Icons::On_MouseExit(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (m_eType == CAMERA_DEFAULT)
		{
			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_Icons::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case INGAME_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Arrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case CAMERA_DEFAULT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Camera_ResetBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_InGame_Setting_Icons::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Icons::Bind_ShaderResources()
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

void CUI_InGame_Setting_Icons::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (m_eType == CAMERA_DEFAULT)
		{
			CUI_Manager::GetInstance()->Reset_SettingCamera();
		}

		//CUI_Manager::GetInstance()->OnOff_InGameSetWindow(true);
	}
}

CUI_InGame_Setting_Icons* CUI_InGame_Setting_Icons::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_INGAMEETC eType)
{
	CUI_InGame_Setting_Icons* pInstance = new CUI_InGame_Setting_Icons(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Icons::Clone(void* pArg)
{
	CUI_InGame_Setting_Icons* pInstance = new CUI_InGame_Setting_Icons(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Icons::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
