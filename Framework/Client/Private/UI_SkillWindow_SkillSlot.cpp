#include "stdafx.h"
#include "UI_SkillWindow_SkillSlot.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillWindow_SkillSlot::CUI_SkillWindow_SkillSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SKILLMENU_SECTION eSection, UI_SKILLMENU_SLOT eType)
	: CUI(pDevice, pContext, L"UI_SkillWindow_SkillSlot")
	, m_eSection(eSection)
	, m_eType(eType)
{
}

CUI_SkillWindow_SkillSlot::CUI_SkillWindow_SkillSlot(const CUI_SkillWindow_SkillSlot& rhs)
	: CUI(rhs)
	, m_eSection(rhs.m_eSection)
	, m_eType(rhs.m_eType)
{
}

void CUI_SkillWindow_SkillSlot::Set_SkillSlot(CHARACTER_TYPE eType)
{
	switch (eType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_iClassTexIndex = 0; // 0, 1, 2
		m_iBurstTexIndex = 0; // 0 ~ 4 무기 속성에 따라 접근할 수 있게 수정할 예정임
		m_iActiveTexIndex = 0; // 0, 1, 2
		break;

	case CHARACTER_TYPE::DESTROYER:
		m_iClassTexIndex = 3; // 3, 4, 5
		m_iBurstTexIndex = 5; // 5 ~ 9
		m_iActiveTexIndex = 3; // 3, 4, 5
		break;

	case CHARACTER_TYPE::ENGINEER:
		m_iClassTexIndex = 6; // 6, 7, 8
		m_iBurstTexIndex = 10; // 10 ~ 14
		m_iActiveTexIndex = 6; // 6, 7, 8
		break;
	}
}

void CUI_SkillWindow_SkillSlot::Set_Active(_bool bActive)
{
	if (SKILLSLOT_END == m_eType)
		return;

	if (bActive)
	{
		m_iPass = 9;
		m_fTimeAcc = 0.f;
		m_fAlpha = 0.3f;
		m_bArrived = false;
		m_bClicked = false;
		m_bAlpha = false;
		m_tInfo.fX = m_vStartPosition.x;
		m_tInfo.fY = m_vStartPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}
	else
	{

	}

	m_bActive = bActive;

	Set_SkillSlot(CHARACTER_TYPE::ENGINEER);
}

HRESULT CUI_SkillWindow_SkillSlot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillWindow_SkillSlot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vStartPosition = _float2(m_vArrivedPosition.x + 150.f, m_vArrivedPosition.y);

	m_fSpeed = 500.f;
	m_fLimit = m_tInfo.fX + (m_tInfo.fCX * 0.5f);

	m_tInfo.fX = m_vStartPosition.x;
	m_tInfo.fY = m_vStartPosition.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	m_bActive = false;

	return S_OK;
}

void CUI_SkillWindow_SkillSlot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SKILLSLOT_END == m_eType)
			return;

		if (!m_bArrived)
		{
			m_tInfo.fX -= fTimeDelta * m_fSpeed;

			if (m_tInfo.fX < m_vArrivedPosition.x)
			{
				m_bArrived = true;
				m_tInfo.fX = m_vArrivedPosition.x;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
		else
		{
			//도착했을때
//			if (m_bClicked)
//				m_fTimeAcc += fTimeDelta;
		}

		if (!m_bAlpha)
		{
			m_fAlpha += fTimeDelta;
			if (m_fAlpha >= 1.f)
			{
				m_bAlpha = true;
				m_fAlpha = 1.f;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillWindow_SkillSlot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SKILLSLOT_END == m_eType)
			return;

		if (m_bClicked)
			m_iPass = 7;
		else
			m_iPass = 9;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SkillWindow_SkillSlot::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		// 움직일때는 9
		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SkillWindow_SkillSlot::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_SkillWindow_SkillSlot::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_SkillWindow_SkillSlot::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_SkillWindow_SkillSlot::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_eSection == UI_SKILLMENU_SECTION::SKILL_CLASS)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ClassSkillSlot"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iFXTexIndex = 0;
	}
	else if (m_eSection == UI_SKILLMENU_SECTION::SKILL_BURST)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_BurstSkillSlot"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iFXTexIndex = 1;
	}
	else if (m_eSection == UI_SKILLMENU_SECTION::SKILL_ACTIVE)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ActiveSkillSlot"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iFXTexIndex = 0;
	}
	else if (m_eSection == UI_SKILLMENU_SECTION::SKILL_SEPARATOR)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ClassSkill_Separator"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iFXTexIndex = -1;
	}

	if (m_eSection != UI_SKILLMENU_SECTION::SKILL_SEPARATOR)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_SkillSlot_FXTexture"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTexture)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_SkillWindow_SkillSlot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillWindow_SkillSlot::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (!m_bArrived)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LimitX", &m_fLimit, sizeof(_float))))
			return E_FAIL;
	}

	switch (m_eSection)
	{
	case UI_SKILLMENU_SECTION::SKILL_CLASS:
		if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_FIRST)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iClassTexIndex)))
				return E_FAIL;
		}
		else if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_SECOND)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iClassTexIndex + 1)))
				return E_FAIL;
		}
		else if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_THIRD)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iClassTexIndex + 2)))
				return E_FAIL;
		}
		else
			return E_FAIL;
		break;

	case UI_SKILLMENU_SECTION::SKILL_BURST:
		if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_FIRST)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iBurstTexIndex)))
				return E_FAIL;
		}
		break;

	case UI_SKILLMENU_SECTION::SKILL_ACTIVE:
		if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_FIRST)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iActiveTexIndex)))
				return E_FAIL;
		}
		else if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_SECOND)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iActiveTexIndex + 1)))
				return E_FAIL;
		}
		else if (m_eType == UI_SKILLMENU_SLOT::SKILLSLOT_THIRD)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iActiveTexIndex + 2)))
				return E_FAIL;
		}
		else
			return E_FAIL;
		break;

	case UI_SKILLMENU_SECTION::SKILL_SEPARATOR:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
		break;

	}

	if (m_bArrived && m_bClicked && m_iPass == 7)
	{
		if (FAILED(m_pFXTexture->Bind_ShaderResource(m_pShaderCom, "g_FXTexture", m_iFXTexIndex)))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_SkillWindow_SkillSlot::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bClicked && m_bArrived)
		{
			CUI_Manager::GetInstance()->Update_SkillSlotState(m_eSection, m_eType);
			// UIManager를 통해서 m_bClicked = true; 를 행한다.
		}
	}
}

CUI_SkillWindow_SkillSlot* CUI_SkillWindow_SkillSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SKILLMENU_SECTION eSection, UI_SKILLMENU_SLOT eType)
{
	CUI_SkillWindow_SkillSlot* pInstance = new CUI_SkillWindow_SkillSlot(pDevice, pContext, eSection, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillWindow_SkillSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillWindow_SkillSlot::Clone(void* pArg)
{
	CUI_SkillWindow_SkillSlot* pInstance = new CUI_SkillWindow_SkillSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillWindow_SkillSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillWindow_SkillSlot::Free()
{
	__super::Free();

	Safe_Release(m_pFXTexture);
	Safe_Release(m_pTextureCom);
}
