#include "stdafx.h"
#include "UI_Setting_Window.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Window::CUI_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Setting_Window")
{
}

CUI_Setting_Window::CUI_Setting_Window(const CUI_Setting_Window& rhs)
	: CUI(rhs)
{
}

void CUI_Setting_Window::Set_Active(_bool bActive)
{
	if (bActive) // 만약 Active상태라면
	{

	}
	else // Active가 false로 전환되었다면
	{
		// 최초의 사이즈를 MinSize로 설정한다.
		// _bool변수를 다시 false로 돌려놓는다

//		m_tInfo.fCX = m_vMinSize.x;
//		m_tInfo.fCY = m_vMinSize.y;
//
//		m_bApproach[0] = false;
//		m_bApproach[1] = false;
//
//		__super::Set_ChildActive(false);
//
//		m_bResize = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Setting_Window::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Window::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 1.f;

	// Min, Origin, Max Size를 설정한다.
//	m_vMinSize.x = m_tInfo.fCX;
//	m_vMinSize.y = m_tInfo.fCY;
//
//	m_vOriginSIze.x = 2000.f;
//	m_vOriginSIze.y = 1300.f; /// 원래 이미지의 size
//
//	m_vMaxSize.x = m_vOriginSIze.x * 1.1f;
//	m_vMaxSize.y = m_vOriginSIze.y * 1.1f; /// 최대로 커질 Size(효과를 위한 것)
//
//	// 최초 사이즈를 MinSize로 Setting한다.
//	m_tInfo.fCX = m_vMinSize.x;
//	m_tInfo.fCY = m_vMinSize.y; // 켜졌을때 가장 작은 size
//	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//
//	m_bApproach.resize(2);
//	m_bApproach[0] = false;
//	m_bApproach[1] = false;


	// 닫기 버튼
	_float fSize = 64.f * 0.5f;
	Make_Child(425.f, -245.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Common_Btn_Close"));

	// 메뉴 버튼
	_float2 vDefaultPos = _float2(-330.f, -110.f);
	_float2 vSize = _float2(220.f, 64.f);
	_float fOffset = 80.f;
	Make_Child(vDefaultPos.x, vDefaultPos.y, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Game"));
	Make_Child(vDefaultPos.x, vDefaultPos.y + fOffset, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Graphic"));
	Make_Child(vDefaultPos.x, vDefaultPos.y + (fOffset * 2.f), vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Icon_Setting_Audio"));

	return S_OK;
}

void CUI_Setting_Window::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (m_bResize) // 사이즈 변형이 완료되었으면 자식을 Update한다.
//		{
//			__super::Tick(fTimeDelta);
//		}
//		else // 사이즈 변형이 완료되지 않았으면 사이즈를 키운다.
//		{
//			m_tInfo.fCX += fTimeDelta * 1000.f;
//			m_tInfo.fCY += fTimeDelta * 1000.f; // MinSize인 상태
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Window::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (!m_bResize) // 변형이 완료되지 않은 상태라면
//		{
//			if (m_tInfo.fCX > m_vMaxSize.x)
//				m_bApproach[0] = true; //MaxSize보다 커진다면 true
//
//			if (m_tInfo.fCY > m_vMaxSize.y)
//				m_bApproach[1] = true; // MaxSize보다 커진다면 true
//
//
//			if (m_bApproach[0] && m_bApproach[1]) // 두개 다 원하는 사이즈까지 도달했다면
//			{
//				m_bResize = true; // Resize가 완료되었다.
//
//				m_tInfo.fCX = m_vOriginSIze.x; // 사용할 Window 사이즈로 만든다
//				m_tInfo.fCY = m_vOriginSIze.y;
//				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//
//				// 자식들을 활성화시킨다.
//				__super::Set_ChildActive(true);
//			}
//		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Setting_Window::Render()
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

HRESULT CUI_Setting_Window::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Window_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Window::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Window::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Setting_Window* CUI_Setting_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Setting_Window* pInstance = new CUI_Setting_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Window::Clone(void* pArg)
{
	CUI_Setting_Window* pInstance = new CUI_Setting_Window(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Window");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Window::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
