#include "stdafx.h"
#include "UI_ImajinnSection_Emoticon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Emoticon_Window.h"

CUI_ImajinnSection_Emoticon::CUI_ImajinnSection_Emoticon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_ImajinnSection_Emoticon")
{
}

CUI_ImajinnSection_Emoticon::CUI_ImajinnSection_Emoticon(const CUI_ImajinnSection_Emoticon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ImajinnSection_Emoticon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Emoticon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

//	CGameObject* pEmoticonWindow = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_UI, TEXT("UI_Emoticon_Window"));
//	if (nullptr != pEmoticonWindow)
//		m_pEmoticonWindow = dynamic_cast<CUI_Emoticon_Window*>(pEmoticonWindow);
//		return E_FAIL;
//
//	m_pEmoticonWindow = dynamic_cast<CUI_Emoticon_Window*>(pEmoticonWindow);

	return S_OK;
}

void CUI_ImajinnSection_Emoticon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pEmoticonWindow)
		{
			CGameObject* pEmoticonWindow = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_UI, TEXT("UI_Emoticon_Window"));
			if (nullptr == pEmoticonWindow)
				return;

			m_pEmoticonWindow = dynamic_cast<CUI_Emoticon_Window*>(pEmoticonWindow);
		}

		if (!m_pEmoticonWindow->Get_Active())
			if (m_iTextureIndex == 1)
				m_iTextureIndex = 0;

		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_Emoticon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_ImajinnSection_Emoticon::Render()
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

void CUI_ImajinnSection_Emoticon::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_ImajinnSection_Emoticon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_ImajinnSection_Emoticon::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_ImajinnSection_Emoticon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Emoticon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_ImajinnSection_Emoticon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_Emoticon::Bind_ShaderResources()
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

void CUI_ImajinnSection_Emoticon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (m_iTextureIndex == 0)
		{
			CUI_Manager::GetInstance()->OnOff_EmoticonWindow(true);
			m_iTextureIndex = 1;
		}
		else if (m_iTextureIndex == 1)
		{
			CUI_Manager::GetInstance()->OnOff_EmoticonWindow(false);
			m_iTextureIndex = 0;
		}

	}
}

CUI_ImajinnSection_Emoticon* CUI_ImajinnSection_Emoticon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ImajinnSection_Emoticon* pInstance = new CUI_ImajinnSection_Emoticon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_Emoticon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_Emoticon::Clone(void* pArg)
{
	CUI_ImajinnSection_Emoticon* pInstance = new CUI_ImajinnSection_Emoticon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_Emoticon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_Emoticon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
