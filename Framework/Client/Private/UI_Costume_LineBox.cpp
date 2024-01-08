#include "stdafx.h"
#include "UI_Costume_LineBox.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Costume_LineBox::CUI_Costume_LineBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Costume_LineBox")
{
}

CUI_Costume_LineBox::CUI_Costume_LineBox(const CUI_Costume_LineBox& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Costume_LineBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Costume_LineBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.85f;

//	_float2 vButtonSize = _float2(300.f * 0.5f, 153.f * 0.5f);
//	Make_Child(70.f, 280.f, vButtonSize.x * 0.8f, vButtonSize.y * 0.8f, TEXT("Prototype_GameObject_UI_Costume_ChangeBtn"));

	return S_OK;
}

void CUI_Costume_LineBox::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Costume_LineBox::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);

		//CUI_Manager::GetInstance()->LateTick_Dummy(fTimeDelta);
	}
}

HRESULT CUI_Costume_LineBox::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();

		//CUI_Manager::GetInstance()->Render_Dummy();
	}

	return S_OK;
}

HRESULT CUI_Costume_LineBox::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_LineBox"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Costume_LineBox::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Costume_LineBox::Bind_ShaderResources()
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

CUI_Costume_LineBox* CUI_Costume_LineBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Costume_LineBox* pInstance = new CUI_Costume_LineBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Costume_LineBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Costume_LineBox::Clone(void* pArg)
{
	CUI_Costume_LineBox* pInstance = new CUI_Costume_LineBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Costume_LineBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Costume_LineBox::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
