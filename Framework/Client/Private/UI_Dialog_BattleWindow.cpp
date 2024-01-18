#include "stdafx.h"
#include "UI_Dialog_BattleWindow.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Dialog_BattleWindow::CUI_Dialog_BattleWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Dialog_BattleWindow")
{
}

CUI_Dialog_BattleWindow::CUI_Dialog_BattleWindow(const CUI_Dialog_BattleWindow& rhs)
	: CUI(rhs)
{
}

void CUI_Dialog_BattleWindow::Set_Contents(const wstring& strContents)
{
//	if (m_strContents == strContents)
//		return;

	m_strContents = strContents;
	m_fTimeAcc = 0.f;
	m_iMaxCount = m_strContents.length();
	m_iTextCount = 0;

	Replace_DialogPosition();
	//Set_Active(true);
}

HRESULT CUI_Dialog_BattleWindow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dialog_BattleWindow::Initialize(void* pArg)
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

void CUI_Dialog_BattleWindow::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Tick_Text(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Dialog_BattleWindow::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		//Contents
		_int iTotalLength = m_iTextCount + 6;
		_int iMaxLength = 15;
		_uint iDestIndex = 0;
		_int iOffsetY = 0;

		TCHAR sTempText[MAX_PATH];
		ZeroMemory(sTempText, sizeof(TCHAR) * MAX_PATH);

		for (_uint i = 0; i < iTotalLength; i++)
		{
			if (iDestIndex > m_strContents.length() + iOffsetY)
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
		TextDesc.vScale = { 0.4f, 0.4f };
		TextDesc.vColor = _float4(0.133f, 0.345f, 0.337f, 1.f);
		TextDesc.vPosition = _float2(m_tInfo.fX - (m_tInfo.fCX * 0.5f) + 120.f,
			m_tInfo.fY - (m_tInfo.fCY * 0.5) + 70.f - iOffsetY * 10.f);
		m_pRendererCom->Add_Text(TextDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Dialog_BattleWindow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1); 

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Dialog_BattleWindow::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniDialogue_Battle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Dialog_BattleWindow::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Dialog_BattleWindow::Bind_ShaderResources()
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

void CUI_Dialog_BattleWindow::Replace_DialogPosition()
{
	_int iQuestNum = CUI_Manager::GetInstance()->Get_QuestNum();

	if (0 > iQuestNum || 4 < iQuestNum)
	{
		Set_Active(false);
		return;
	}

	_float fOffsetY = 70.f;

	switch (iQuestNum)
	{
	case 0:
		m_tInfo.fY = 250.f;
		break;

	case 1:
		m_tInfo.fY = 250.f + fOffsetY;
		break;

	case 2:
		m_tInfo.fY = 250.f + fOffsetY * 2.f;
		break;

	case 3:
		m_tInfo.fY = 250.f + fOffsetY * 3.f;
		break;

	case 4:
		m_tInfo.fY = 250.f + fOffsetY * 4.f;
		break;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

void CUI_Dialog_BattleWindow::Tick_Text(_float fTimeDelta)
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

CUI_Dialog_BattleWindow* CUI_Dialog_BattleWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dialog_BattleWindow* pInstance = new CUI_Dialog_BattleWindow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Dialog_BattleWindow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog_BattleWindow::Clone(void* pArg)
{
	CUI_Dialog_BattleWindow* pInstance = new CUI_Dialog_BattleWindow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Dialog_BattleWindow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog_BattleWindow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
