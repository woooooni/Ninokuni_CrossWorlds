#include "stdafx.h"
#include "UI_PopupQuest.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_PopupQuest::CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_PopupQuest")
{
}

CUI_PopupQuest::CUI_PopupQuest(const CUI_PopupQuest& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PopupQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PopupQuest::Initialize(void* pArg)
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

void CUI_PopupQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_PopupQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PopupQuest::Render()
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

void CUI_PopupQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_PopupQuest::On_Mouse(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CUI_PopupQuest::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_PopupQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

//	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowQuest"),
//		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
//		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_PopupQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PopupQuest::Bind_ShaderResources()
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

void CUI_PopupQuest::Key_Input(_float fTimeDelta)
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
	}
}

CUI_PopupQuest* CUI_PopupQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PopupQuest::Clone(void* pArg)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PopupQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PopupQuest::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
