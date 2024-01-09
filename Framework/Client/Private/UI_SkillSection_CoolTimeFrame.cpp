#include "stdafx.h"
#include "UI_SkillSection_CoolTimeFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SKill_Manager.h"
#include "Skill.h"

CUI_SkillSection_CoolTimeFrame::CUI_SkillSection_CoolTimeFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COOLFRAME_TYPE eType)
	: CUI(pDevice, pContext, L"UI_SkillSection_CoolTimeFrame")
	, m_eFrameType(eType)
{
}

CUI_SkillSection_CoolTimeFrame::CUI_SkillSection_CoolTimeFrame(const CUI_SkillSection_CoolTimeFrame& rhs)
	: CUI(rhs)
	, m_eFrameType(rhs.m_eFrameType)
{
}

void CUI_SkillSection_CoolTimeFrame::Set_CharacterType(CHARACTER_TYPE eType)
{
	m_eCurPlayerType = eType;
	SetUp_FrameColor();
}

void CUI_SkillSection_CoolTimeFrame::Use_Skill()
{
	m_bUsable = false;
	m_fCurGauge = 0.f;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	m_bActive = true; 
	m_fCurGauge = 0.f;

	return S_OK;
}

void CUI_SkillSection_CoolTimeFrame::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_bUsable)
		{
			m_iPass = 19;

			m_fCurGauge += fTimeDelta;

			if (m_fMaxGauge <= m_fCurGauge)
			{
				m_bUsable = true;
				m_fCurGauge = m_fMaxGauge;
			}
		}
		else
		{
			m_iPass = 1;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_CoolTimeFrame::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	}
}

HRESULT CUI_SkillSection_CoolTimeFrame::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame_Mask"),
		TEXT("Com_MaskTexture"), (CComponent**)&m_pMaskTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_Skill()
{
	// CurPlayerType 잘 들어오는지 확인해야함.
	CSkill* pTemp = nullptr;

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::SWORDMAN_BURST_MEGA_SLASH);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;

	case CHARACTER_TYPE::DESTROYER:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::DESTROYER_BURST_HYPERSTRIKE);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;

	case CHARACTER_TYPE::ENGINEER:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::ENGINNER_BURST_DESTRUCTION);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;
	}

	m_fMaxGauge = m_pSkill->Get_CoolTime();

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eElementalType))))
		return E_FAIL;

	if (!m_bUsable && m_iPass == 19)
	{
		// 아직 스킬을 사용하지 않은 상태 -> 게이지가 오른다.
		_float fRatio = 1 - ((m_fMaxGauge - m_fCurGauge) / m_fMaxGauge); // 값이 0이면 원 전체가 그려진다.

		if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_SkillSection_CoolTimeFrame::SetUp_FrameColor()
{

	if (UI_COOLFRAME_TYPE::FRAME_END <= m_eFrameType)
		return;

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;
	}
}

CUI_SkillSection_CoolTimeFrame* CUI_SkillSection_CoolTimeFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COOLFRAME_TYPE eType)
{
	CUI_SkillSection_CoolTimeFrame* pInstance = new CUI_SkillSection_CoolTimeFrame(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_CoolTimeFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_CoolTimeFrame::Clone(void* pArg)
{
	CUI_SkillSection_CoolTimeFrame* pInstance = new CUI_SkillSection_CoolTimeFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_CoolTimeFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_CoolTimeFrame::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pTextureCom);
}
