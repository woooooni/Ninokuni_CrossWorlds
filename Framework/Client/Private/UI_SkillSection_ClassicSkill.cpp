#include "stdafx.h"
#include "UI_SkillSection_ClassicSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_ClassicSkill::CUI_SkillSection_ClassicSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CLASSICSKILL eType)
	: CUI(pDevice, pContext, L"UI_SkillSection_ClassicSkill")
	, m_eType(eType)
{
}

CUI_SkillSection_ClassicSkill::CUI_SkillSection_ClassicSkill(const CUI_SkillSection_ClassicSkill& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_SkillSection_ClassicSkill::Set_SkillType(CHARACTER_TYPE eType)
{
//	if (m_ePlayerType == eType)
//		return;

	// CHaracterType에따른 SkillType을 설정한다 (For Texture Component)
	m_ePlayerType = eType;
	switch (m_ePlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN: // 0, 1, 2
		if (UI_CLASSICSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 0;
		if (UI_CLASSICSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 1;
		if (UI_CLASSICSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 2;
		break;

	case CHARACTER_TYPE::DESTROYER: // 3, 4, 5
		if (UI_CLASSICSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 3;
		if (UI_CLASSICSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 4;
		if (UI_CLASSICSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 5;
		break;

	case CHARACTER_TYPE::ENGINEER: // 6, 7, 8
		if (UI_CLASSICSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 6;
		if (UI_CLASSICSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 7;
		if (UI_CLASSICSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 8;
		break;
	}


}

HRESULT CUI_SkillSection_ClassicSkill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_ClassicSkill::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	return S_OK;
}

void CUI_SkillSection_ClassicSkill::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UICLASSICSKILL_END == m_eType)
			return;

		if (SKILL_SECOND == m_eType)
		{
//			m_tInfo.fCX = 256.f * 0.26f;
//			m_tInfo.fCY = 256.f * 0.26f;
//			m_tInfo.fX = g_iWinSizeX - (220.f);
//			m_tInfo.fY = g_iWinSizeY - (230.f);
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_ClassicSkill::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UICLASSICSKILL_END == m_eType)
			return;

		if (0 > m_iTextureIndex)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SkillSection_ClassicSkill::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SkillSection_ClassicSkill::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_SkillSection_ClassicSkill::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_SkillSection_ClassicSkill::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_SkillSection_ClassicSkill::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_ClassicSkills"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_ClassicSkill::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_ClassicSkill::Bind_ShaderResources()
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

void CUI_SkillSection_ClassicSkill::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_SkillSection_ClassicSkill* CUI_SkillSection_ClassicSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CLASSICSKILL eType)
{
	CUI_SkillSection_ClassicSkill* pInstance = new CUI_SkillSection_ClassicSkill(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_ClassicSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_ClassicSkill::Clone(void* pArg)
{
	CUI_SkillSection_ClassicSkill* pInstance = new CUI_SkillSection_ClassicSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_ClassicSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_ClassicSkill::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
