#include "stdafx.h"
#include "UI_BasicButton.h"
#include "GameInstance.h"

CUI_BasicButton::CUI_BasicButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType)
	: CUI(pDevice, pContext, strObjectTag), m_eType(eType)
{
}

CUI_BasicButton::CUI_BasicButton(const CUI_BasicButton& rhs)
	: CUI(rhs), m_eType(rhs.m_eType)
{
}

HRESULT CUI_BasicButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BasicButton::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;


	return S_OK;
}

void CUI_BasicButton::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CUI_BasicButton::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_BasicButton::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_BasicButton::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_TEST, TEXT("Prototype_Component_Texture_Lobby_NicknameFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BasicButton::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BasicButton::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_BasicButton* CUI_BasicButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType)
{
	CUI_BasicButton* pInstance = new CUI_BasicButton(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BasicButton::Clone(void* pArg)
{
	CUI_BasicButton* pInstance = new CUI_BasicButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BasicButton::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
