#include "stdafx.h"
#include "UI_SkillSection_SpecialSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Skill_Manager.h"
#include "Skill.h"
#include "Effect_Manager.h"

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

void CUI_SkillSection_SpecialSkill::Set_Clicked(_bool bClick)
{
	if (true == bClick)
	{
		m_fOriginCoolTime = m_pSkill->Get_CoolTime();	// 스킬 쿨타임을 받아온다.
		m_fCoolTime = m_fOriginCoolTime;	// 대입한다.
	}

	m_bClicked = bClick;

	// Effect Create
	_matrix WorldMatrix = XMMatrixIdentity();
	_float4x4 Worldfloat4x4;
	XMStoreFloat4x4(&Worldfloat4x4, WorldMatrix);
	Worldfloat4x4.m[3][0] = m_tInfo.fX;
	Worldfloat4x4.m[3][1] = m_tInfo.fY;
	Worldfloat4x4.m[3][2] = 0.f;
	WorldMatrix = XMLoadFloat4x4(&Worldfloat4x4);
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_UI_SkillUse"), WorldMatrix);
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

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);

	return S_OK;
}

void CUI_SkillSection_SpecialSkill::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UICLASSICSKILL_END == m_eType)
			return;

		Movement_BasedOnHiding(fTimeDelta);

		if (m_bClicked)
		{
//			m_fCoolTime -= fTimeDelta;
			m_fCoolTime = m_fOriginCoolTime - (m_pSkill->Get_CurrCoolTime());
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
					TextDesc.vPosition = _float2(m_vTextPosition.x + 9.f, m_vTextPosition.y);
				else
					TextDesc.vPosition = m_vTextPosition;

				// Outline
				CRenderer::TEXT_DESC OutlineDesc;
				OutlineDesc.strText = TextDesc.strText;
				OutlineDesc.strFontTag = TextDesc.strFontTag;
				OutlineDesc.vScale = TextDesc.vScale;
				OutlineDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
				OutlineDesc.vPosition = _float2(TextDesc.vPosition.x - 1.2f, TextDesc.vPosition.y);
				m_pRendererCom->Add_Text(OutlineDesc);
				OutlineDesc.vPosition = _float2(TextDesc.vPosition.x + 1.2f, TextDesc.vPosition.y);
				m_pRendererCom->Add_Text(OutlineDesc);
				OutlineDesc.vPosition = _float2(TextDesc.vPosition.x, TextDesc.vPosition.y - 1.2f);
				m_pRendererCom->Add_Text(OutlineDesc);
				OutlineDesc.vPosition = _float2(TextDesc.vPosition.x, TextDesc.vPosition.y + 1.2f);
				m_pRendererCom->Add_Text(OutlineDesc);

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

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_SkillSection_SpecialSkill::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
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
		//m_bClicked = true;
	}
}

void CUI_SkillSection_SpecialSkill::Set_SkillType()
{
	CSkill* pTemp = nullptr;
	// CHaracterType에따른 SkillType을 설정한다 (For Texture Component)
	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN: // 0, 1, 2
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
		{
			m_iTextureIndex = 0;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_ACANE_BARRIER);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
		{
			m_iTextureIndex = 1;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_FROZEN_STORM);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
		{
			m_iTextureIndex = 2;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_SWORD_TEMPEST);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		break;

	case CHARACTER_TYPE::DESTROYER: // 3, 4, 5
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
		{
			m_iTextureIndex = 3;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::DESTROYER_FRENGE_CHARGE);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
		{
			m_iTextureIndex = 4;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::DESTROYER_BATTLE_CRY);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
		{
			m_iTextureIndex = 5;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::DESTROYER_IGNORE_PAIN);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		break;

	case CHARACTER_TYPE::ENGINEER: // 6, 7, 8
		if (UI_SPECIALSKILL::SKILL_FIRST == m_eType)
		{
			m_iTextureIndex = 6;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::ENGINEER_FLASH_HEAL);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_SECOND == m_eType)
		{
			m_iTextureIndex = 7;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::ENGINEER_TIMELAB);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		if (UI_SPECIALSKILL::SKILL_THIRD == m_eType)
		{
			m_iTextureIndex = 8;

			pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::ENGINEER_HEALINGTREE);
			if (nullptr == pTemp)
				return;
			m_pSkill = pTemp;
		}
		break;
	}

	if (nullptr == m_pSkill)
		return;

	m_fOriginCoolTime = m_pSkill->Get_CoolTime();
	m_fCoolTime = m_fOriginCoolTime;
}

void CUI_SkillSection_SpecialSkill::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
				m_bHideFinish = true;
			else
			{
				m_tInfo.fX += (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG());

				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;
				else
					m_fAlpha -= fTimeDelta;
			}
		}
		else // 드러낸다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= fTimeDelta * m_fHideSpeed;

					if (m_fAlpha >= 1.f)
						m_fAlpha = 1.f;
					else
						m_fAlpha += fTimeDelta;
				}
			}
			else
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG());

					if (m_fAlpha >= 1.f)
						m_fAlpha = 1.f;
					else
						m_fAlpha += fTimeDelta;
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
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
