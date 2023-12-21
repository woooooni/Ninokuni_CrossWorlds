#include "stdafx.h"
#include "UI_WeaponSection_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_WeaponSection_DefaultWeapon.h"

CUI_WeaponSection_Slot::CUI_WeaponSection_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_WEAPONSLOT eSlotType)
	: CUI(pDevice, pContext, L"UI_WeaponSection_Slot")
	, m_eSlotType(eSlotType)
{
}

CUI_WeaponSection_Slot::CUI_WeaponSection_Slot(const CUI_WeaponSection_Slot& rhs)
	: CUI(rhs)
	, m_eSlotType(rhs.m_eSlotType)
	, m_pNoWeapon(rhs.m_pNoWeapon)
{
}

HRESULT CUI_WeaponSection_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Slot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	// No Weapon Texture를 생성한다.
	_float fSize = 128.f * 0.2f;

	UI_INFO ParentDesc;
	ZeroMemory(&ParentDesc, sizeof(CUI::UI_INFO));

	ParentDesc.fCX = 400.f * 0.75f;
	ParentDesc.fCY = 400.f * 0.75f;
	ParentDesc.fX = g_iWinSizeX - ParentDesc.fCX * 0.5f;
	ParentDesc.fY = g_iWinSizeY - ParentDesc.fCY * 0.5f - 5.f;

	UI_INFO	UIDesc;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = fSize;
	UIDesc.fCY = fSize;
	UIDesc.fX = ParentDesc.fX + m_tInfo.fX;
	UIDesc.fY = ParentDesc.fY + m_tInfo.fY;
//	UIDesc.pParent = this;
//	if (nullptr != pChildArg)
//		UIDesc.pDesc = pChildArg;

	CGameObject* pNoWeapon = nullptr;
	pNoWeapon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_SkillSection_DefaultAttack"), LAYER_TYPE::LAYER_UI, &UIDesc);
	if (nullptr == pNoWeapon)
		return E_FAIL;

	m_pNoWeapon = dynamic_cast<CUI_WeaponSection_DefaultWeapon*>(pNoWeapon);
	m_pNoWeapon->Set_Resizable(false);

	return S_OK;
}

void CUI_WeaponSection_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_bWear) // 무기를 착용하지 않은상태라면
		{
			m_pNoWeapon->Tick(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_WeaponSection_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		if (!m_bWear) // 무기를 착용하지 않은상태라면
		{
			m_pNoWeapon->LateTick(fTimeDelta);
		}
	}
}

HRESULT CUI_WeaponSection_Slot::Render()
{
	if (m_bActive)
	{
		if (!m_bWear) // 무기를 착용하지 않은상태라면
		{
			m_pNoWeapon->Render();
		}

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_WeaponSection_Slot::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_WeaponSection_Slot::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_WeaponSection_Slot::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_WeaponSection_Slot::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Slot"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_WeaponSection_Slot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WeaponSection_Slot::Bind_ShaderResources()
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

void CUI_WeaponSection_Slot::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_WeaponSection_Slot* CUI_WeaponSection_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_WEAPONSLOT eSlotType)
{
	CUI_WeaponSection_Slot* pInstance = new CUI_WeaponSection_Slot(pDevice, pContext, eSlotType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WeaponSection_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WeaponSection_Slot::Clone(void* pArg)
{
	CUI_WeaponSection_Slot* pInstance = new CUI_WeaponSection_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WeaponSection_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WeaponSection_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pNoWeapon);
	Safe_Release(m_pTextureCom);
}
