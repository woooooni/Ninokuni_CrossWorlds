#include "stdafx.h"
#include "UI_SetNickname_Textbox.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SetNickname_Textbox::CUI_SetNickname_Textbox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SetNickname_Textbox")
{
}

CUI_SetNickname_Textbox::CUI_SetNickname_Textbox(const CUI_SetNickname_Textbox& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SetNickname_Textbox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SetNickname_Textbox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_fAlpha = 0.9f;


	_float fSize = 64.f * 0.4f;
	_float fOffset = 180.f;
	Make_Child(fOffset, 0.f, fSize, fSize, TEXT("Prototype_GameObject_UI_SetNickname_Dice"));

	return S_OK;
}

void CUI_SetNickname_Textbox::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_SetNickname_Textbox::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		CRenderer::TEXT_DESC NickDesc = {};
		NickDesc.strText = m_strText;
		_int iLength = m_strText.length();

		NickDesc.strFontTag = L"Default_Bold";
		NickDesc.vScale = { 0.4f, 0.4f };
		NickDesc.vPosition = _float2((g_iWinSizeX * 0.5f)  - (iLength * 8.f), g_iWinSizeY * 0.5f - 8.f);
		NickDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(NickDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_SetNickname_Textbox::Render()
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

void CUI_SetNickname_Textbox::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
		CUI_Manager::GetInstance()->Set_MouseCursor(1);
	}
}

void CUI_SetNickname_Textbox::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CUI_Manager::GetInstance()->Set_Textable(true);
		}
	}
}

void CUI_SetNickname_Textbox::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		CUI_Manager::GetInstance()->Set_MouseCursor(0);
	}
}

HRESULT CUI_SetNickname_Textbox::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Textbox"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SetNickname_Textbox::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_SetNickname_Textbox::Bind_ShaderResources()
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

CUI_SetNickname_Textbox* CUI_SetNickname_Textbox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SetNickname_Textbox* pInstance = new CUI_SetNickname_Textbox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SetNickname_Textbox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SetNickname_Textbox::Clone(void* pArg)
{
	CUI_SetNickname_Textbox* pInstance = new CUI_SetNickname_Textbox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SetNickname_Textbox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SetNickname_Textbox::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
