#include "stdafx.h"
#include "UI_Dialog_MiniWindow.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Dialog_MiniWindow::CUI_Dialog_MiniWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Dialog_MiniWindow")
{
}

CUI_Dialog_MiniWindow::CUI_Dialog_MiniWindow(const CUI_Dialog_MiniWindow& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Dialog_MiniWindow::Set_Name(const wstring& strName)
{
	if (m_strName == strName)
		return;

	m_strName = strName;
}

void CUI_Dialog_MiniWindow::Set_Contents(const wstring& strText)
{
	if (m_strContents == strText)
		return;

	m_strContents = strText;

	if (false == Get_Active())
		Set_Active(true);
}

HRESULT CUI_Dialog_MiniWindow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dialog_MiniWindow::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bUseMouse = false;
	
	return S_OK;
}

void CUI_Dialog_MiniWindow::Tick(_float fTimeDelta)
{
	if (MINIDIALOG_END == m_eType)
		return;

	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Dialog_MiniWindow::LateTick(_float fTimeDelta)
{
	if (MINIDIALOG_END == m_eType)
		return;

	if (m_bActive)
	{
		CRenderer::TEXT_DESC NameDesc = {};
		_int iLength = m_strName.length();
		_int iOffset = (iLength - 1) * 10;

		NameDesc.strText = m_strName;
		NameDesc.strFontTag = L"Default_Bold";
		NameDesc.vScale = { 0.45f, 0.45f };
		NameDesc.vColor = _float4(0.812f, 0.796f, 0.569f, 1.f);
		NameDesc.vPosition = _float2(m_tInfo.fX - (m_tInfo.fCX * 0.5f) + 64.f - iOffset,
			m_tInfo.fY - (m_tInfo.fCY * 0.5) + 10.f);
		m_pRendererCom->Add_Text(NameDesc);

		//Contents
		_int iTotalLength = m_strContents.length() + 4;
		_int iMaxLength = 23;
		_uint iDestIndex = 0;
		_int iOffsetY = 0;

		TCHAR sTempText[MAX_PATH];
		ZeroMemory(sTempText, sizeof(TCHAR) * MAX_PATH);

		for (_uint i = 0; i < iTotalLength; i++)
		{
			if (iDestIndex > m_strContents.length())
			{
				break;
			}

			sTempText[iDestIndex++] = m_strContents[i];

			if ((i + 1) % iMaxLength == 0)
			{
				sTempText[iDestIndex++] = L'\n';
				iOffsetY++;
			}
		}

		CRenderer::TEXT_DESC TextDesc = {};
		TextDesc.strText = sTempText;
		TextDesc.strFontTag = L"Default_Bold";
		TextDesc.vScale = { 0.45f, 0.45f };
		TextDesc.vColor = _float4(0.133f, 0.345f, 0.337f, 1.f);
		TextDesc.vPosition = _float2(m_tInfo.fX - (m_tInfo.fCX * 0.5f) + 50.f,
			m_tInfo.fY - (m_tInfo.fCY * 0.5) + 80.f - iOffsetY * 10.f);
		m_pRendererCom->Add_Text(TextDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Dialog_MiniWindow::Render()
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
HRESULT CUI_Dialog_MiniWindow::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_MiniWindow"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Dialog_MiniWindow::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Dialog_MiniWindow::Bind_ShaderResources()
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

CUI_Dialog_MiniWindow* CUI_Dialog_MiniWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dialog_MiniWindow* pInstance = new CUI_Dialog_MiniWindow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Dialog_MiniWindow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog_MiniWindow::Clone(void* pArg)
{
	CUI_Dialog_MiniWindow* pInstance = new CUI_Dialog_MiniWindow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Dialog_MiniWindow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog_MiniWindow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
