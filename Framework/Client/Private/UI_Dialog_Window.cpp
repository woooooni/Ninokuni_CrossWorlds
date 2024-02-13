#include "stdafx.h"
#include "UI_Dialog_Window.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Dialog_Window::CUI_Dialog_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Dialog_MiniWindow")
{
	ZeroMemory(m_szName, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(m_szInfoText, sizeof(TCHAR) * MAX_PATH);
}

CUI_Dialog_Window::CUI_Dialog_Window(const CUI_Dialog_Window& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
	lstrcpy(m_szName, rhs.m_szName);
	lstrcpy(m_szInfoText, rhs.m_szInfoText);
}

HRESULT CUI_Dialog_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dialog_Window::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	_float2 vButtonSize = _float2(192.f, 61.f);
	Make_Child(355.f, -103.f, vButtonSize.x * 0.8f, vButtonSize.y * 0.8f, TEXT("Prototype_GameObject_UI_Dialog_FlipButton"));
	_float2 vArrowSize = _float2(64.f, 64.f);
	Make_Child(0.f, 100.f, vArrowSize.x * 0.5f, vArrowSize.y * 0.5f, TEXT("Prototype_GameObject_UI_Dialog_FlipArrow"));

	return S_OK;
}

void CUI_Dialog_Window::Tick(_float fTimeDelta)
{
	if (UIDIALOG_END == m_eType)
		return;

	if (m_bActive)
	{
		Tick_Text(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Dialog_Window::LateTick(_float fTimeDelta)
{
	if (UIDIALOG_END == m_eType)
		return;

	if (m_bActive)
	{
		Add_Text();

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Dialog_Window::Render()
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

void CUI_Dialog_Window::Set_Name(_tchar* pszName)
{
	if (!lstrcmp(m_szName, pszName))
		return;

	ZeroMemory(m_szName, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(m_szName, pszName);
}

void CUI_Dialog_Window::Set_Text(_tchar* pszText)
{
	if (!lstrcmp(m_szInfoText, pszText))
		return;

	ZeroMemory(m_szInfoText, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(m_szInfoText, pszText);

	wstring sTemp = pszText;
	m_iMaxCount = sTemp.size();
	m_iTextCount = 0;
	m_fTimeAcc = 0.f;
}

HRESULT CUI_Dialog_Window::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_Window"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Dialog_Window::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Dialog_Window::Bind_ShaderResources()
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

void CUI_Dialog_Window::Tick_Text(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_fTimeAcc += fTimeDelta;

		if (0.01f < m_fTimeAcc)
		{
			++m_iTextCount;
			if (MAX_PATH <= m_iTextCount)
				m_iTextCount = MAX_PATH - 1;

			m_fTimeAcc = 0.f;
		}
	}
	else
	{
		m_iTextCount = 0.f;
		m_fTimeAcc = 0.f;
	}
}

void CUI_Dialog_Window::Add_Text()
{
	// Name
	CRenderer::TEXT_DESC NameDesc = {};
	_int iLength = wcslen(m_szName) - 1;
	_int iSpaceCount = CUI_Manager::GetInstance()->Count_WordSpacing(m_szName);
	_int iOffset = (iLength - iSpaceCount) * 10 + (iSpaceCount * 6); // 글자 수 오프셋 + 띄어쓰기 오프셋

	NameDesc.strText = m_szName;
	NameDesc.strFontTag = L"Default_Bold";
	NameDesc.vScale = { 0.6f, 0.6f };
	NameDesc.vColor = _float4(0.812f, 0.796f, 0.569f, 1.f);
	NameDesc.vPosition = _float2(m_tInfo.fX - (m_tInfo.fCX * 0.5f) + 115.f - iOffset,
		m_tInfo.fY - (m_tInfo.fCY * 0.5) + 25.f);
	m_pRendererCom->Add_Text(NameDesc);

	//Contents
	_int iTotalLength = m_iTextCount + 4;
	_int iMaxLength = 47; // 44 -> 48
	_uint iDestIndex = 0;

	_int iEnter = 0; // Enter하는 곳을 판별하기 위한 수단
	_bool bIsMark = false;

	TCHAR sTempText[MAX_PATH];
	ZeroMemory(sTempText, sizeof(TCHAR) * MAX_PATH);

	for (_int i = 0; i < iTotalLength; i++)
	{
		if (iDestIndex > m_iMaxCount)
		{
			break;
		}

		sTempText[iDestIndex++] = m_szInfoText[i];

		// 20240213 수정중
		wstring strTemp(&m_szInfoText[i]);
		bIsMark = CUI_Manager::GetInstance()->Is_PunctuationMarks(strTemp);
		if (false == bIsMark) // 
			iEnter++;
		//

		if ((iEnter + 1) % iMaxLength == 0) 		//if ((i + 1) % iMaxLength == 0)
		{
			sTempText[iDestIndex++] = '\n';
		}
	}

	CRenderer::TEXT_DESC TextDesc = {};
	TextDesc.strText = sTempText;
	TextDesc.strFontTag = L"Default_Bold";
	TextDesc.vScale = { 0.6f, 0.6f };
	TextDesc.vColor = _float4(0.133f, 0.345f, 0.337f, 1.f);
	TextDesc.vPosition = _float2(m_tInfo.fX - (m_tInfo.fCX * 0.5f) + 70.f,
		m_tInfo.fY - (m_tInfo.fCY * 0.5) + 80.f);
	m_pRendererCom->Add_Text(TextDesc);
}

CUI_Dialog_Window* CUI_Dialog_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dialog_Window* pInstance = new CUI_Dialog_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Dialog_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog_Window::Clone(void* pArg)
{
	CUI_Dialog_Window* pInstance = new CUI_Dialog_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Dialog_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
