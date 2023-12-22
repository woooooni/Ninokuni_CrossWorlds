#include "stdafx.h"
#include "UI_BtnQuickQuest.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_BtnQuickQuest::CUI_BtnQuickQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnQuickQuest")
{
}

CUI_BtnQuickQuest::CUI_BtnQuickQuest(const CUI_BtnQuickQuest& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BtnQuickQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// 자식 UI로 MiniQuestWindow를 생성한다.
	// MiniQuestWindow -> QuestManager에서 현재 진행중인 퀘스트를 받아와서 Quest가 있을 경우에만 Active한다.
	

	return S_OK;
}

void CUI_BtnQuickQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnQuickQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnQuickQuest::Render()
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

void CUI_BtnQuickQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_BtnQuickQuest::On_Mouse(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CUI_BtnQuickQuest::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_BtnQuickQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowQuest"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnQuickQuest::Bind_ShaderResources()
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

void CUI_BtnQuickQuest::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (0 == m_iTextureIndex)
			m_iTextureIndex = 1;
		else if (1 == m_iTextureIndex)
			m_iTextureIndex = 0;
		else
		{
			return;
		}

		if (m_iTextureIndex)
		{
			CUI_Manager::GetInstance()->OnOff_QuestPopup(true);
		}
		else
		{
			CUI_Manager::GetInstance()->OnOff_QuestPopup(false);
		}
	}
}

CUI_BtnQuickQuest* CUI_BtnQuickQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnQuickQuest* pInstance = new CUI_BtnQuickQuest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnQuickQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnQuickQuest::Clone(void* pArg)
{
	CUI_BtnQuickQuest* pInstance = new CUI_BtnQuickQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnQuickQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnQuickQuest::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
