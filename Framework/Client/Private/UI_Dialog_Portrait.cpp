#include "stdafx.h"
#include "UI_Dialog_Portrait.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Dialog_Portrait::CUI_Dialog_Portrait(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PORTRAIT eType)
	: CUI(pDevice, pContext, L"UI_Dialog_Portrait")
	, m_eType(eType)
{
}

CUI_Dialog_Portrait::CUI_Dialog_Portrait(const CUI_Dialog_Portrait& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Dialog_Portrait::Set_Active(_bool bActive)
{
	if (PORTRAIT_FRAME == m_eType)
	{
		if (bActive) // true 켜진다
		{
			m_tInfo.fCX = m_vStartSize.x;
			m_tInfo.fCY = m_vStartSize.y;

			m_tInfo.fX = m_vStartPosition.x;
			m_tInfo.fY = m_vStartPosition.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

			m_bArrived = false;
			m_bResize = false;
			m_bAlpha = false;

			m_fAlpha = 0.3f;
			m_fSpeed = 80.f;
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_Dialog_Portrait::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dialog_Portrait::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	if (PORTRAIT_FRAME == m_eType)
	{
		m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_tInfo.fX - 100.f, m_tInfo.fY + 100.f);

		m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
		m_vStartSize = _float2(m_tInfo.fCX * 0.3f, m_tInfo.fCY * 0.3f);
	}

	return S_OK;
}

void CUI_Dialog_Portrait::Tick(_float fTimeDelta)
{
	if (PORTRAIT_END == m_eType)
		return;

	if (m_bActive)
	{
		if (PORTRAIT_FRAME == m_eType)
			Tick_Frame(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Dialog_Portrait::LateTick(_float fTimeDelta)
{
	if (PORTRAIT_END == m_eType)
		return;

	if (m_bActive)
	{
		if (PORTRAIT_FRAME == m_eType)
			LateTick_Frame(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		//__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Dialog_Portrait::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		//__super::Render();
	}

	return S_OK;
}

void CUI_Dialog_Portrait::Tick_Frame(_float fTimeDelta)
{
	if (!m_bAlpha)
	{
		m_fAlpha += fTimeDelta;

		if (m_fAlpha > 1.f)
		{
			m_bAlpha = true;
			m_fAlpha = 1.f;
		}
	}

	if (!m_bResize) // 아직 Resize가 완료되지 않았다.
	{
		m_fSpeed += fTimeDelta * (400.f + (fTimeDelta * 10000));

		m_tInfo.fCX += fTimeDelta * m_fSpeed;
		m_tInfo.fCY += fTimeDelta * m_fSpeed;
		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		// 사이즈를 조절한다.
		
		// 사이즈에 대한 부분
		if (m_tInfo.fCX > m_vOriginSize.x)
		{
			// 사이즈가 더 커지는 것을 막는다.
			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

			m_bResize = true;
		}
	}
}

void CUI_Dialog_Portrait::LateTick_Frame(_float fTimeDelta)
{
	if (!m_bArrived) // 아직 목표하는 곳에 도착하지 않았다.
	{
		m_tInfo.fX += fTimeDelta * 400.f;
		m_tInfo.fY -= fTimeDelta * 400.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		// 위치에 대한 부분
		if (m_tInfo.fX > m_vOriginPosition.x)
		{
			// 더 멀리 가는 것을 막는다.
			m_tInfo.fX = m_vOriginPosition.x;
			m_tInfo.fY = m_vOriginPosition.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

			m_bArrived = true;
		}
	}
}

HRESULT CUI_Dialog_Portrait::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_PORTRAIT::PORTRAIT_FRAME:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_PortraitFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		m_iTextureIndex = 0;
		break;

//	case UI_PORTRAIT::PORTRAIT_CHARACTER:
//		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_Window"),
//			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
//			return E_FAIL;
//		break;
	}

	
	return S_OK;
}

HRESULT CUI_Dialog_Portrait::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Dialog_Portrait::Bind_ShaderResources()
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

CUI_Dialog_Portrait* CUI_Dialog_Portrait::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PORTRAIT eType)
{
	CUI_Dialog_Portrait* pInstance = new CUI_Dialog_Portrait(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Dialog_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog_Portrait::Clone(void* pArg)
{
	CUI_Dialog_Portrait* pInstance = new CUI_Dialog_Portrait(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Dialog_Portrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog_Portrait::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
