#include "stdafx.h"
#include "UI_MonsterHP_Background.h"
#include "GameInstance.h"

CUI_MonsterHP_Background::CUI_MonsterHP_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHP_Background")
{
}

CUI_MonsterHP_Background::CUI_MonsterHP_Background(const CUI_MonsterHP_Background& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MonsterHP_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterHP_Background::Initialize(void* pArg)
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

void CUI_MonsterHP_Background::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		CRenderer::TEXT_DESC MaxHPDesc;
		MaxHPDesc.strText = L"¹ã¹Ù¿À¹ã";
		MaxHPDesc.strFontTag = L"Default_Medium";
		MaxHPDesc.vScale = { 0.4f, 0.4f };
		MaxHPDesc.vPosition = m_vNamePosition;
		MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MaxHPDesc);

		__super::Tick(fTimeDelta);
	}
}

void CUI_MonsterHP_Background::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MonsterHP_Background::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MonsterHP_Background::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MonsterHP_Background::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MonsterHP_Background::Bind_ShaderResources()
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

CUI_MonsterHP_Background* CUI_MonsterHP_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_Background* pInstance = new CUI_MonsterHP_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MonsterHP_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_Background::Clone(void* pArg)
{
	CUI_MonsterHP_Background* pInstance = new CUI_MonsterHP_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MonsterHP_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
