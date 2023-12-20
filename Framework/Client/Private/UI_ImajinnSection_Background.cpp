#include "stdafx.h"
#include "UI_ImajinnSection_Background.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_ImajinnSection_Background::CUI_ImajinnSection_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSection_Background")
{
}

CUI_ImajinnSection_Background::CUI_ImajinnSection_Background(const CUI_ImajinnSection_Background& rhs)
	: CUI(rhs)
{
}

void CUI_ImajinnSection_Background::Set_Active(_bool bActive)
{
	if (bActive)
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}
	}
	else
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_ImajinnSection_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Background::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true; 
	m_fAlpha = 0.95f;

	_float fSize = 128.f * 0.55f;
	_float2 vOffset = _float2(fSize + 3.f, -2.f);

	Make_Child(-1.f * vOffset.x, vOffset.y, fSize, fSize, TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_First")); // Section 1
	Make_Child(0.f, vOffset.y, fSize, fSize, TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_Second")); // Section 2
	Make_Child(vOffset.x, vOffset.y, fSize, fSize, TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_Third")); // Section 3

	fSize = 64.f * 0.6f;
	vOffset = _float2(175.f, -2.f);
	Make_Child(-1.f * vOffset.x, vOffset.y, fSize, fSize, TEXT("Prototype_GameObject_UI_ImajinnSection_Emoticon")); // 이모티콘 Button
	Make_Child(vOffset.x, vOffset.y, fSize, fSize, TEXT("Prototype_GameObject_UI_ImajinnSection_Vehicle")); // 탈 것 Button

	return S_OK;
}

void CUI_ImajinnSection_Background::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_Background::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta); // ChilUI가 있으면 LateTick도 돌려야함.
	}
}

HRESULT CUI_ImajinnSection_Background::Render()
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

HRESULT CUI_ImajinnSection_Background::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_ImajinnSection_Background::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_Background::Bind_ShaderResources()
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

CUI_ImajinnSection_Background* CUI_ImajinnSection_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ImajinnSection_Background* pInstance = new CUI_ImajinnSection_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_Background::Clone(void* pArg)
{
	CUI_ImajinnSection_Background* pInstance = new CUI_ImajinnSection_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
