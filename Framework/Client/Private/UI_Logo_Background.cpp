#include "stdafx.h"
#include "UI_Logo_Background.h"
#include "GameInstance.h"

CUI_Logo_Background::CUI_Logo_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Logo_Background")
{
}

CUI_Logo_Background::CUI_Logo_Background(const CUI_Logo_Background& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Logo_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Logo_Background::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// Temp : 임시로 사이즈 별도지정함
	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_Logo_Background::Tick(_float fTimeDelta)
{
	m_fFrame += 30.f * fTimeDelta;

	if (m_fFrame >= 247.f)
	{
		m_fFrame = 247.f;
		m_bTextActive = true;
	}

	__super::Tick(fTimeDelta);
}

void CUI_Logo_Background::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Logo_Background::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	if (m_bTextActive)
	{
		 // Temp

		CRenderer::TEXT_DESC TextDesc = {};
		ZeroMemory(&TextDesc, sizeof(CRenderer::TEXT_DESC));

		TextDesc.strText = L"Press Any Key";
		TextDesc.strFontTag = L"Default_Bold";
		TextDesc.vPosition = _float2(g_iWinSizeX * 0.5f - 100.f, g_iWinSizeY * 0.5f + 200.f);
		TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		TextDesc.fAngle = 0.f;
		TextDesc.vOrigin = { 0.f, 0.f };
		TextDesc.vScale = { 0.8f, 0.8f };

		m_pRendererCom->Add_Text(TextDesc);
	}

	return S_OK;
}

HRESULT CUI_Logo_Background::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Background_Frames"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
//	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Background"),
//		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
//		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Logo_Background::Ready_State()
{
	// 1920 * 1080 -> 원본은 WinSize의 1.2배
	// 크기 조절해도 비율 문제 없음

	m_tInfo.fCX = g_iWinSizeX;
	m_tInfo.fCY = g_iWinSizeY;

	m_tInfo.fX = g_iWinSizeX * 0.5f;
	m_tInfo.fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Logo_Background::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;
//	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
//		return E_FAIL;

	return S_OK;
}

CUI_Logo_Background* CUI_Logo_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Logo_Background* pInstance = new CUI_Logo_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Logo_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Logo_Background::Clone(void* pArg)
{
	CUI_Logo_Background* pInstance = new CUI_Logo_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Logo_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
