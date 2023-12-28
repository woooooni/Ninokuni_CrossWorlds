#include "stdafx.h"
#include "UI_SkillSection_SpecialSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_SpecialSkill::CUI_SkillSection_SpecialSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SPECIALSKILL eType)
	: CUI(pDevice, pContext, L"UI_SkillSection_SpecialSkill")
	, m_eType(eType)
{
}

CUI_SkillSection_SpecialSkill::CUI_SkillSection_SpecialSkill(const CUI_SkillSection_SpecialSkill& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_SkillSection_SpecialSkill::Set_CharacterType(CHARACTER_TYPE eType)
{
	m_eCurPlayerType = eType;
	Set_SkillType();
}

HRESULT CUI_SkillSection_SpecialSkill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_SpecialSkill::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	switch (m_eType)
	{
	case SKILL_FIRST:
		m_vTextPosition = _float2(g_iWinSizeX - 166.f, g_iWinSizeY - 73.f);
		break;

	case SKILL_SECOND:
		m_vTextPosition = _float2(g_iWinSizeX - 100.f, g_iWinSizeY - 101.f);
		break;

	case SKILL_THIRD:
		m_vTextPosition = _float2(g_iWinSizeX - 75.f, g_iWinSizeY - 165.f);
		break;
	}

	return S_OK;
}

void CUI_SkillSection_SpecialSkill::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UICLASSICSKILL_END == m_eType)
			return;

		if (m_bClicked)
		{
			m_fCoolTime -= fTimeDelta;
			m_iPass = 6;

			if (m_fCoolTime <= 0.f)
			{
				m_bClicked = false;

				m_iPass = 5;
				m_fCoolTime = m_fOriginCoolTime;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_SpecialSkill::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UICLASSICSKILL_END == m_eType)
			return;

		if (0 > m_iTextureIndex)
			return;

		if (m_bClicked)
		{
			if (_int(m_fCoolTime) + 1 > 0)
			{
				_int iTemp = _int(m_fCoolTime) + 1;

				CRenderer::TEXT_DESC TextDesc;

				TextDesc.strText = std::to_wstring(iTemp);
				TextDesc.strFontTag = L"Default_Bold";
				TextDesc.vScale = { 0.6f, 0.6f };
				TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };

				if (iTemp < 10)
					TextDesc.vPosition = _float2(m_vTextPosition.x + 10.f, m_vTextPosition.y);
				else
					TextDesc.vPosition = m_vTextPosition;

				m_pRendererCom->Add_Text(TextDesc);
			}
		}


		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SkillSection_SpecialSkill::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		// DefaultPAss는 5(마스크 이미지 사용함)
		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SkillSection_SpecialSkill::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_SkillSection_SpecialSkill::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_SkillSection_SpecialSkill::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_SkillSection_SpecialSkill::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_SpecialSkills"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_SpecialSkill_Mask"),
		TEXT("Com_MaskTexture"), (CComponent**)&m_pMaskTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CoolTime_MaskTexture"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_SpecialSkill::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_SpecialSkill::Bind_ShaderResources()
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

	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;

	if (m_bClicked)
	{
		// 선택되었다면, FXTexture를 던진다. + CoolTime과 관련된 것들을 함께 던져준다.

		_float fRatio = (m_fOriginCoolTime - m_fCoolTime) / m_fOriginCoolTime; // 값이 0이면 원 전체가 그려진다.

		if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_SkillSection_SpecialSkill::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_bClicked = true;
	}
}

void CUI_SkillSection_SpecialSkill::Set_SkillType()
{
	//	if (m_ePlayerType == eType)
	//		return;

	// CHaracterType에따른 SkillType을 설정한다 (For Texture Component)

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN: // 0, 1, 2
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 0;
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 1;
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 2;
		break;

	case CHARACTER_TYPE::DESTROYER: // 3, 4, 5
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 3;
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 4;
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 5;
		break;

	case CHARACTER_TYPE::ENGINEER: // 6, 7, 8
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
			m_iTextureIndex = 6;
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
			m_iTextureIndex = 7;
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
			m_iTextureIndex = 8;
		break;
	}

	m_fOriginCoolTime = 10.f; // Temp
	m_fCoolTime = m_fOriginCoolTime;
}

CUI_SkillSection_SpecialSkill* CUI_SkillSection_SpecialSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SPECIALSKILL eType)
{
	CUI_SkillSection_SpecialSkill* pInstance = new CUI_SkillSection_SpecialSkill(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_SpecialSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_SpecialSkill::Clone(void* pArg)
{
	CUI_SkillSection_SpecialSkill* pInstance = new CUI_SkillSection_SpecialSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_SpecialSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_SpecialSkill::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pTextureCom);
}
