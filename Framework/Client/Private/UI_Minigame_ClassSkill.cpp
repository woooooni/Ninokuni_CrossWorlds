#include "stdafx.h"
#include "UI_Minigame_ClassSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Skill_Manager.h"
#include "Skill.h"
#include "UI_Minigame_Basic.h"

CUI_Minigame_ClassSkill::CUI_Minigame_ClassSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIXSKILL eType)
	: CUI(pDevice, pContext, L"UI_Minigame_ClassSkill")
	, m_eType(eType)
{
}

CUI_Minigame_ClassSkill::CUI_Minigame_ClassSkill(const CUI_Minigame_ClassSkill& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Minigame_ClassSkill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_ClassSkill::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	if (m_eType != UI_GRANDPRIXSKILL::SKILL_SPECIAL)
	{
		CUI::UI_INFO UIDesc = {};
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
		UIDesc.fCX = 276.f * 0.3f;
		UIDesc.fCY = UIDesc.fCX;
		UIDesc.fX = m_tInfo.fX;
		UIDesc.fY = m_tInfo.fY;
		CGameObject* pFrame = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkillFrame"), _uint(LAYER_TYPE::LAYER_UI), &UIDesc);
		if (nullptr == pFrame)
			return E_FAIL;
		if (nullptr == dynamic_cast<CUI_Minigame_Basic*>(pFrame))
			return E_FAIL;
		m_pFrame = dynamic_cast<CUI_Minigame_Basic*>(pFrame);

		m_vTextPosition.x = m_tInfo.fX - 20.f;
		m_vTextPosition.y = m_tInfo.fY - 15.f;
		m_vScale = _float2(0.6f, 0.6f);
		m_vOffsetX = 9.f;
	}
	else
	{
		CUI::UI_INFO UIDesc = {};
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
		UIDesc.fCX = 276.f * 0.45f;
		UIDesc.fCY = UIDesc.fCX;
		UIDesc.fX = m_tInfo.fX;
		UIDesc.fY = m_tInfo.fY;
		CGameObject* pFrame = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkillFrame"), _uint(LAYER_TYPE::LAYER_UI), &UIDesc);
		if (nullptr == pFrame)
			return E_FAIL;
		if (nullptr == dynamic_cast<CUI_Minigame_Basic*>(pFrame))
			return E_FAIL;
		m_pFrame = dynamic_cast<CUI_Minigame_Basic*>(pFrame);

		m_vTextPosition.x = m_tInfo.fX - 30.f;
		m_vTextPosition.y = m_tInfo.fY - 20.f;
		m_vScale = _float2(1.f, 1.f);
		m_vOffsetX = 13.f;
	}

	m_fOriginCoolTime = 10.f;
	m_fCoolTime = 10.f;

	m_bUseMouse = true;

	return S_OK;
}

void CUI_Minigame_ClassSkill::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (GRANDPRIXSKILL_END == m_eType)
			return;

		if (nullptr != m_pFrame)
			m_pFrame->Tick(fTimeDelta);

		if (m_bClicked)
		{
			m_fCoolTime -= fTimeDelta;
			m_iPass = 22;

			if (m_fCoolTime <= 0.f)
			{
				m_bClicked = false;

				m_iPass = 1;
				m_fCoolTime = m_fOriginCoolTime;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_ClassSkill::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (GRANDPRIXSKILL_END == m_eType)
			return;

		if (nullptr != m_pFrame)
			m_pFrame->LateTick(fTimeDelta);

		if (m_bClicked)
		{
			if (_int(m_fCoolTime) + 1 > 0)
			{
				_int iTemp = _int(m_fCoolTime) + 1;

				CRenderer::TEXT_DESC TextDesc;

				TextDesc.strText = std::to_wstring(iTemp);
				TextDesc.strFontTag = L"Default_Bold";
				TextDesc.vScale = m_vScale;

				if (iTemp < 10)
				{
					_float2 vPos = _float2(m_vTextPosition.x + m_vOffsetX, m_vTextPosition.y);

					// Outline
					TextDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
					TextDesc.vPosition = _float2(vPos.x - 1.f, vPos.y);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(vPos.x + 1.f, vPos.y);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(vPos.x, vPos.y - 1.f);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(vPos.x, vPos.y + 1.f);
					m_pRendererCom->Add_Text(TextDesc);

					// Original
					TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
					TextDesc.vPosition = vPos;
					m_pRendererCom->Add_Text(TextDesc);
				}
				else
				{
					// Outline
					TextDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
					TextDesc.vPosition = _float2(m_vTextPosition.x - 1.f, m_vTextPosition.y);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(m_vTextPosition.x + 1.f, m_vTextPosition.y);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y - 1.f);
					m_pRendererCom->Add_Text(TextDesc);
					TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y + 1.f);
					m_pRendererCom->Add_Text(TextDesc);
					
					//Original
					TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
					TextDesc.vPosition = m_vTextPosition;
					m_pRendererCom->Add_Text(TextDesc);
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_ClassSkill::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();

		if (nullptr != m_pFrame)
			m_pFrame->Render();
	}

	return S_OK;
}

void CUI_Minigame_ClassSkill::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Minigame_ClassSkill::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Minigame_ClassSkill::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Minigame_ClassSkill::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_eType != UI_GRANDPRIXSKILL::SKILL_SPECIAL)
	{
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_ClassSkills"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_BurstSkill"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_MaskTexture"),
		TEXT("Com_MaskTexture"), (CComponent**)&m_pFXTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_ClassSkill::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_ClassSkill::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_eType != UI_GRANDPRIXSKILL::SKILL_SPECIAL)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (m_bClicked)
	{
		// 선택되었다면, FXTexture를 던진다. + CoolTime과 관련된 것들을 함께 던져준다.

		_float fRatio = (m_fOriginCoolTime - m_fCoolTime) / m_fOriginCoolTime;

		if (FAILED(m_pFXTexture->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_Minigame_ClassSkill::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_bClicked = true;
	}
}

void CUI_Minigame_ClassSkill::Set_SkillType()
{
//	CSkill* pTemp = nullptr;
//
//	if (UI_GRANDPRIXSKILL::SKILL_FIRST == m_eType)
//	{
//		m_iTextureIndex = 0;
//		pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_PERFECT_BLADE);
//		if (nullptr == pTemp)
//			return;
//		m_pSkill = pTemp;
//	}
//	if (UI_GRANDPRIXSKILL::SKILL_SECOND == m_eType)
//	{
//		m_iTextureIndex = 1;
//		pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_SIPOHONINGLUNGE);
//		if (nullptr == pTemp)
//			return;
//		m_pSkill = pTemp;
//	}
//	if (UI_GRANDPRIXSKILL::SKILL_THIRD == m_eType)
//	{
//		m_iTextureIndex = 2;
//		pTemp = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_SPINNING_ASSAULT);
//		if (nullptr == pTemp)
//			return;
//		m_pSkill = pTemp;
//	}
//
//	m_fOriginCoolTime = m_pSkill->Get_CoolTime();
//	m_fCoolTime = m_fOriginCoolTime;
}

void CUI_Minigame_ClassSkill::Update_SkillInfo()
{
//
//	switch (m_eType)
//	{
//	case UI_GRANDPRIXSKILL::SKILL_FIRST:
//		m_pSkill = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_SPINNING_ASSAULT);
//		break;
//
//	case UI_GRANDPRIXSKILL::SKILL_SECOND:
//		m_pSkill = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_SIPOHONINGLUNGE);
//		break;
//
//	case UI_GRANDPRIXSKILL::SKILL_THIRD:
//		m_pSkill = CSkill_Manager::GetInstance()->Get_Skill(m_eCurPlayerType, SKILL_TYPE::SWORDMAN_PERFECT_BLADE);
//		break;
//	}
//
//	if (nullptr == m_pSkill)
//		return;
}

CUI_Minigame_ClassSkill* CUI_Minigame_ClassSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIXSKILL eType)
{
	CUI_Minigame_ClassSkill* pInstance = new CUI_Minigame_ClassSkill(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_ClassSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_ClassSkill::Clone(void* pArg)
{
	CUI_Minigame_ClassSkill* pInstance = new CUI_Minigame_ClassSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_ClassSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_ClassSkill::Free()
{
	__super::Free();

	Safe_Release(m_pFrame);
	Safe_Release(m_pFXTexture);
	Safe_Release(m_pTextureCom);
}
