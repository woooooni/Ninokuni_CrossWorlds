#include "stdafx.h"
#include "UI_Minimap_Frame.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Minimap_Frame::CUI_Minimap_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIMAP eType)
	: CUI(pDevice, pContext, L"UI_Minimap_Frame")
	, m_eType(eType)
{
}

CUI_Minimap_Frame::CUI_Minimap_Frame(const CUI_Minimap_Frame& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Minimap_Frame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minimap_Frame::Initialize(void* pArg)
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

void CUI_Minimap_Frame::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minimap_Frame::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Minimap_Frame::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

void CUI_Minimap_Frame::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_Minimap_Frame::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
		Key_Input(fTimeDelta);
}

void CUI_Minimap_Frame::On_MouseExit(_float fTimeDelta)
{

}

HRESULT CUI_Minimap_Frame::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch(m_eType)
	{
	case UI_MINIMAP::MINIMAP_FRAME:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIMAP::MINIMAP_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_FrameArrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Minimap_Frame::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minimap_Frame::Bind_ShaderResources()
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

void CUI_Minimap_Frame::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (UI_MINIMAP::MINIMAP_FRAME == m_eType)
		{
			CUI_Manager::GetInstance()->OnOff_WorldMap(true);
		}
	}
}

CUI_Minimap_Frame* CUI_Minimap_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIMAP eType)
{
	CUI_Minimap_Frame* pInstance = new CUI_Minimap_Frame(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minimap_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minimap_Frame::Clone(void* pArg)
{
	CUI_Minimap_Frame* pInstance = new CUI_Minimap_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minimap_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minimap_Frame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
