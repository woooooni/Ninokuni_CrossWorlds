#include "stdafx.h"
#include "UI_Minigame_TowerSelect.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Minigame_TowerSelect::CUI_Minigame_TowerSelect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TOWERTYPE eType)
	: CUI(pDevice, pContext, L"UI_Minigame_TowerSelect")
	, m_eType(eType)
{
}

CUI_Minigame_TowerSelect::CUI_Minigame_TowerSelect(const CUI_Minigame_TowerSelect& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Minigame_TowerSelect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_TowerSelect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_Minigame_TowerSelect::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_TowerSelect::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_TowerSelect::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Minigame_TowerSelect::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Minigame_TowerSelect::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Minigame_TowerSelect::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		Set_Click(false);

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Minigame_TowerSelect::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_TowerSelect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_TowerSelect::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_TowerSelect::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_TowerSelect::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		Set_Click(true);
	}
}

CUI_Minigame_TowerSelect* CUI_Minigame_TowerSelect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TOWERTYPE eType)
{
	CUI_Minigame_TowerSelect* pInstance = new CUI_Minigame_TowerSelect(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_TowerSelect::Clone(void* pArg)
{
	CUI_Minigame_TowerSelect* pInstance = new CUI_Minigame_TowerSelect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_TowerSelect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
