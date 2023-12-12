#include "stdafx.h"
#include "UI_Number.h"
#include "GameInstance.h"

CUI_Number::CUI_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Number")
{
}

CUI_Number::CUI_Number(const CUI_Number& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Number::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Number::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// Origin Scale과 Max Scale을 저장해둔다.
	m_fMaxScale = m_tInfo.fCX + 50.f; // Temp
	m_fOriginScale = m_tInfo.fCX;

	// UI_NUMBERINFO에서 받아온 iNumber를 m_iNumber에 세팅한다.
	Set_Number(m_iNumber);

	return S_OK;
}

void CUI_Number::Tick(_float fTimeDelta)
{
	if (m_bResize)
	{
		if (m_fOriginScale > m_tInfo.fCX)
		{
			m_bResize = false;

			m_tInfo.fCX = m_fOriginScale;
			m_tInfo.fCY = m_fOriginScale;
		}
		else
		{
			m_tInfo.fCX -= fTimeDelta * 100.f;
			m_tInfo.fCY -= fTimeDelta * 100.f;
		}
	}

	__super::Tick(fTimeDelta);
}

void CUI_Number::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Number::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Number::Set_Number(_uint iNumber)
{
	if (0 > m_iNumber)
		return;

	m_sNumber = to_string(m_iNumber);
	Resize_Scale();
}

void CUI_Number::Resize_Scale()
{
	// Resize를 시작한다.

	m_bResize = true;

	m_tInfo.fCX = m_fMaxScale;
	m_tInfo.fCY = m_fMaxScale;
}

HRESULT CUI_Number::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Number::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Number::Bind_ShaderResources()
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

CUI_Number* CUI_Number::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Number* pInstance = new CUI_Number(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Number");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Number::Clone(void* pArg)
{
	CUI_Number* pInstance = new CUI_Number(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Number");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Number::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
