#include "stdafx.h"
#include "UI_ImajinnSection_PlayerSwitching.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Character.h"
#include "Character_Manager.h"

CUI_ImajinnSection_PlayerSwitching::CUI_ImajinnSection_PlayerSwitching(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PLAYERSWITCH eSlotType)
	: CUI(pDevice, pContext, L"UI_ImajinnSection_PlayerSwitching")
	, m_eSlotType(eSlotType)
{
}

CUI_ImajinnSection_PlayerSwitching::CUI_ImajinnSection_PlayerSwitching(const CUI_ImajinnSection_PlayerSwitching& rhs)
	: CUI(rhs)
	, m_eSlotType(rhs.m_eSlotType)
{
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	CCharacter* pCharacter = nullptr;
	pCharacter = CUI_Manager::GetInstance()->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	m_fAlpha = 1.f;
	m_bIsUsable = false;
	m_bActive = false;
	m_bUseMouse = true;
	
	return S_OK;
}

void CUI_ImajinnSection_PlayerSwitching::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		CCharacter* pCharacter = nullptr;
		pCharacter = CUI_Manager::GetInstance()->Get_Character();
		if (nullptr == pCharacter)
			return;
		
		switch (m_eSlotType)
		{
		case SLOT_SWORDMAN:
			pCharacter = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
			if (nullptr == pCharacter)
			{
				m_bIsUsable = false;
				return;
			}
			if (m_bIsUsable != pCharacter->Is_Useable())
				m_bIsUsable = pCharacter->Is_Useable();
			break;

		case SLOT_ENGINEER:
			pCharacter = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER);
			if (nullptr == pCharacter)
			{
				m_bIsUsable = false;
				return;
			}
			if (m_bIsUsable != pCharacter->Is_Useable())
				m_bIsUsable = pCharacter->Is_Useable();
			break;

		case SLOT_DESTROYER:
			pCharacter = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER);
			if (nullptr == pCharacter)
			{
				m_bIsUsable = false;
				return;
			}
			if (m_bIsUsable != pCharacter->Is_Useable())
				m_bIsUsable = pCharacter->Is_Useable();
			break;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_PlayerSwitching::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eSlotType == SWITCH_END)
			return;

		if (true == m_bIsUsable)
			m_iPass = 1;
		else
			m_iPass = 16;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Render()
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

void CUI_ImajinnSection_PlayerSwitching::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_ImajinnSection_PlayerSwitching::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_ImajinnSection_PlayerSwitching::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_PlayerSlots"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_PlayerSwitching::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eSlotType))))
		return E_FAIL;

	return S_OK;
}

CUI_ImajinnSection_PlayerSwitching* CUI_ImajinnSection_PlayerSwitching::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_PLAYERSWITCH eSlotType)
{
	CUI_ImajinnSection_PlayerSwitching* pInstance = new CUI_ImajinnSection_PlayerSwitching(pDevice, pContext, eSlotType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_PlayerSwitching");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_PlayerSwitching::Clone(void* pArg)
{
	CUI_ImajinnSection_PlayerSwitching* pInstance = new CUI_ImajinnSection_PlayerSwitching(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_PlayerSwitching");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_PlayerSwitching::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
