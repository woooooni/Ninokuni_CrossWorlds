#include "stdafx.h"
#include "UI_SkillSection_Interaction.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_Interaction::CUI_SkillSection_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSeection_Interaction_Icon")
{
}

CUI_SkillSection_Interaction::CUI_SkillSection_Interaction(const CUI_SkillSection_Interaction& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillSection_Interaction::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_Interaction::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_eType = INTERACTION_KICK;

	return S_OK;
}

void CUI_SkillSection_Interaction::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (INTERACTION_END == m_eType)
			return;

		if (m_bRotate)
		{
			m_fTimeAcc += fTimeDelta;
			//m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 1.f), fTimeDelta * 50.f); //TextCode
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_Interaction::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (INTERACTION_END == m_eType)
			return;

		if (1.f < m_fTimeAcc)
			m_bRotate = false;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SkillSection_Interaction::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(15);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SkillSection_Interaction::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_SkillSection_Interaction::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_SkillSection_Interaction::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_SkillSection_Interaction::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionIcon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_Interaction::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_Interaction::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_eType)))
		return E_FAIL;

	return S_OK;
}

void CUI_SkillSection_Interaction::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_bRotate = true;
	}
}

CUI_SkillSection_Interaction* CUI_SkillSection_Interaction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SkillSection_Interaction* pInstance = new CUI_SkillSection_Interaction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_Interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_Interaction::Clone(void* pArg)
{
	CUI_SkillSection_Interaction* pInstance = new CUI_SkillSection_Interaction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_Interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_Interaction::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
