#include "stdafx.h"
#include "UI_Emoticon_Window.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Emoticon_Window::CUI_Emoticon_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Emoticon_Window")
{
}

CUI_Emoticon_Window::CUI_Emoticon_Window(const CUI_Emoticon_Window& rhs)
	: CUI(rhs)
{
}

void CUI_Emoticon_Window::Set_Active(_bool bActive)
{
	if (bActive) // true
	{
		m_Resize[0] = false;
		m_Resize[1] = true;
		m_fValue = 0.f;
		m_fAlpha = 0.9f;

		m_tInfo.fX = m_vOriginPosition.x;
		m_tInfo.fY = m_vOriginPosition.y;
		m_tInfo.fCX = m_vOriginSize.x;
		m_tInfo.fCY = m_vOriginSize.y;

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		m_bActive = bActive;
		
		Set_ChildActive(true);
	}
	else // false
	{
		m_fValue = 0.f;
		m_Resize[1] = false;
		Set_ChildActive(false);
	}
}

HRESULT CUI_Emoticon_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Emoticon_Window::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.9f;
	m_fLimitY = 790.f;

	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMaxSize = _float2(m_tInfo.fCX, m_tInfo.fCY + 40.f);
	m_vMinSize = _float2(m_tInfo.fCX, m_tInfo.fCY - 100.f);

	// 여기서 Emoticon Button을 만든다
	_float2 vOffset = _float2(74.f, 74.f);
	_float2 vButtonSize = _float2(77.f * 0.8f, 77.f * 0.8f);
	Make_Child(-110.f, -45.f, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_First"));
	Make_Child(-110.f + vOffset.x, -45.f, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Second"));
	Make_Child(-110.f + (vOffset.x * 2.f), -45.f, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Third"));
	Make_Child(-110.f + (vOffset.x * 3.f), -45.f, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Fourth"));
	Make_Child(-110.f, -45.f + vOffset.y, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Fifth"));
	Make_Child(-110.f + vOffset.x, -45.f + vOffset.y, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Sixth"));
	Make_Child(-110.f + (vOffset.x * 2.f), -45.f + vOffset.y, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Seventh"));
	Make_Child(-110.f + (vOffset.x * 3.f), -45.f + vOffset.y, vButtonSize.x, vButtonSize.y, TEXT("Prototype_GameObject_UI_Emoticon_Btn_Eighth"));

	// 닫기 버튼을 만든다
	_float fSize = 64.f * 0.4f;
	Make_Child(133.f, -96.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Common_Btn_Close"));

	m_Resize.reserve(2);
	m_Resize.push_back(false);
	m_Resize.push_back(true);

	return S_OK;
}

void CUI_Emoticon_Window::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_Resize[0]) // 처음 커질 때 bool변수 -> flase
		{
			m_fValue += fTimeDelta * 100.f; // 활성화 -> 비활성화보다 빠르게 진행한다.
			m_tInfo.fCY += m_fValue;

			if (m_tInfo.fCY >= m_vMaxSize.y)
			{
				m_tInfo.fCY = m_vMaxSize.y;
				m_Resize[0] = true;
			}
		}
		else
		{
			m_tInfo.fCY = m_vOriginSize.y;
		}

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		__super::Tick(fTimeDelta);
	}
}

void CUI_Emoticon_Window::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		if (!m_Resize[1])
		{
			m_fValue += fTimeDelta * 500.f;
			m_tInfo.fCY -= m_fValue;

			m_tInfo.fY += (fTimeDelta * m_fValue * 50.f);

			m_fAlpha -= fTimeDelta;
			if (m_fAlpha <= 0.f)
				m_fAlpha = 0.f;

			if (m_tInfo.fCY <= m_vMinSize.y)
			{
				m_tInfo.fCY = m_vMinSize.y;
				m_Resize[1] = true;
				m_bActive = false;
			}

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Emoticon_Window::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(12);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_Emoticon_Window::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_Window"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Emoticon_Window::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Emoticon_Window::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_LimitY", &m_fLimitY, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Emoticon_Window* CUI_Emoticon_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Emoticon_Window* pInstance = new CUI_Emoticon_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Emoticon_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Emoticon_Window::Clone(void* pArg)
{
	CUI_Emoticon_Window* pInstance = new CUI_Emoticon_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Emoticon_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Emoticon_Window::Free()
{
	__super::Free();

	m_Resize.clear();
	Safe_Release(m_pTextureCom);
}
