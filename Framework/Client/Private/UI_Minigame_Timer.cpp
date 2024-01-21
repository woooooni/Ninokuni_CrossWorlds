#include "stdafx.h"
#include "UI_Minigame_Timer.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Minigame_Timer::CUI_Minigame_Timer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MenuSeparator")
{
}

CUI_Minigame_Timer::CUI_Minigame_Timer(const CUI_Minigame_Timer& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Minigame_Timer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Timer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false; 
	m_fAlpha = 0.9f;
	
	return S_OK;
}

void CUI_Minigame_Timer::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_Timer::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_Timer::Render()
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

HRESULT CUI_Minigame_Timer::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Timer"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_Timer::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_Timer::Bind_ShaderResources()
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

CUI_Minigame_Timer* CUI_Minigame_Timer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_Timer* pInstance = new CUI_Minigame_Timer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Timer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Timer::Clone(void* pArg)
{
	CUI_Minigame_Timer* pInstance = new CUI_Minigame_Timer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Timer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Timer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
