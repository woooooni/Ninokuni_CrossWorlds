#include "stdafx.h"
#include "UI_PopupQuest.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_PopupQuest::CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType)
	: CUI(pDevice, pContext, L"UI_PopupQuest")
	, m_eType(eType)
{
}

CUI_PopupQuest::CUI_PopupQuest(const CUI_PopupQuest& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_PopupQuest::Set_Active(_bool bActive)
{
	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType)
	{
		if (bActive)
		{
			m_fAppearProg = 0.f; // 초기화해준다.
		}
		else
		{

		}
	}
	else
	{
		if (m_bEvent)
		{
			m_bEvent = false;
			//CUI_Manager::GetInstance()->Set_UIClicked(m_bEvent);
		}
	}

	m_bActive = bActive;
}

void CUI_PopupQuest::Set_Contents(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents)
{
	if (CUI_PopupQuest::POPUPWINDOW != m_eType)
		return;

	/*
	_float4(0.957f, 0.784f, 0.067f, 1.f) 메인
	_float4(0.165f, 0.984f, 0.957f, 1.f) 명성
	_float4(0.373f, 0.863f, 0.647f, 1.f) 제비
	_float4(0.898f, 0.624f, 0.333f, 1.f) 토벌
	_float4(0.914f, 0.443f, 0.392f, 1.f) 수배

	_float4(0.804f, 0.843f, 0.741f, 1.f) 내용
	*/

	m_bProgressing = true; // 퀘스트가 완료되면 false로 전환할 수 있는 매개가 필요함.

	m_strType = strQuestType;
	m_vTypeColor = _float4(0.957f, 0.784f, 0.067f, 1.f);

	m_strTitle = strTitle;
	m_strContents = strContents;
	m_vTextColor = _float4(0.804f, 0.843f, 0.741f, 1.f);
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

	m_bActive = false;

	Set_Contents(TEXT("[메인]"), TEXT("쿠우를 찾아라"), TEXT("네가 쿠우냐? 나도 쿠우다"));

	return S_OK;
}

void CUI_PopupQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		if (m_fAppearProg <= 1.f)
			m_fAppearProg += fTimeDelta * 10.f;

		__super::Tick(fTimeDelta);
	}
}

void CUI_PopupQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (QUESTPOPUP_END == m_eType)
			return;

		if (POPUPWINDOW == m_eType)
		{
			//AddText
			if (m_bProgressing)
			{
				CRenderer::TEXT_DESC TypeDesc;

				TypeDesc.strText = m_strType;
				TypeDesc.strFontTag = L"Default_Bold";
				TypeDesc.vScale = { 0.4f, 0.4f };
				TypeDesc.vColor = m_vTypeColor;
				TypeDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY - 20.f);

				m_pRendererCom->Add_Text(TypeDesc);

				CRenderer::TEXT_DESC TitleDesc;

				TitleDesc.strText = m_strTitle;
				TitleDesc.strFontTag = L"Default_Bold";
				TitleDesc.vScale = { 0.4f, 0.4f };
				TitleDesc.vColor = m_vTextColor;
				TitleDesc.vPosition = _float2(m_tInfo.fX - 50.f, m_tInfo.fY - 20.f);

				m_pRendererCom->Add_Text(TitleDesc);

				CRenderer::TEXT_DESC ContentsDesc;

				ContentsDesc.strText = m_strContents;
				ContentsDesc.strFontTag = L"Default_Bold";
				ContentsDesc.vScale = { 0.4f, 0.4f };
				ContentsDesc.vColor = m_vTextColor;
				ContentsDesc.vPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY);

				m_pRendererCom->Add_Text(ContentsDesc);
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PopupQuest::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(8);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_PopupQuest::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_PopupQuest::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_PopupQuest::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_PopupQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case POPUPFRAME_TOP:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUPFRAME_BOTTOM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case POPUPWINDOW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Window"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.4f;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_PopupQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

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

	if (POPUPFRAME_TOP == m_eType || POPUPFRAME_BOTTOM == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
	}
	else if (POPUPWINDOW == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fAppearProg, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_PopupQuest::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_PopupQuest* CUI_PopupQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType)
{
	CUI_PopupQuest* pInstance = new CUI_PopupQuest(pDevice, pContext, eType);

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
