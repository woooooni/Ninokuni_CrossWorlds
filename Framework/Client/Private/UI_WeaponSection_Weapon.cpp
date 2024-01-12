#include "stdafx.h"
#include "UI_WeaponSection_Weapon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_WeaponSection_Weapon::CUI_WeaponSection_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_WEAPONSLOT eType, UI_WEAPONSLOT_TYPE eSlotType)
	: CUI(pDevice, pContext, L"UI_WeaponSection_Weapon")
	, m_eType(eType)
	, m_eSlotType(eSlotType)
{
}

CUI_WeaponSection_Weapon::CUI_WeaponSection_Weapon(const CUI_WeaponSection_Weapon& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
	, m_eSlotType(rhs.m_eSlotType)
{
}

void CUI_WeaponSection_Weapon::Set_TextureIndex(CHARACTER_TYPE eType)
{
	switch (eType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (UI_WEAPONSLOT::WEAPON_END <= m_eType)
			return;

		if (UI_WEAPONSLOT::WEAPON_FIRST == m_eType)
		{
			m_iTextureIndex = 0;
			m_iElementalIndex = 0;
		}
		else if (UI_WEAPONSLOT::WEAPON_SECOND == m_eType)
		{
			m_iTextureIndex = 1;
			m_iElementalIndex = 1;
		}
		else
		{
			m_iTextureIndex = 2;
			m_iElementalIndex = 2;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		if (UI_WEAPONSLOT::WEAPON_END <= m_eType)
			return;

		if (UI_WEAPONSLOT::WEAPON_FIRST == m_eType)
		{
			m_iTextureIndex = 3;
			m_iElementalIndex = 0;
		}
		else if (UI_WEAPONSLOT::WEAPON_SECOND == m_eType)
		{
			m_iTextureIndex = 4;
			m_iElementalIndex = 1;
		}
		else
		{
			m_iTextureIndex = 5;
			m_iElementalIndex = 2;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		if (UI_WEAPONSLOT::WEAPON_END <= m_eType)
			return;

		if (UI_WEAPONSLOT::WEAPON_FIRST == m_eType)
		{
			m_iTextureIndex = 6;
			m_iElementalIndex = 0;
		}
		else if (UI_WEAPONSLOT::WEAPON_SECOND == m_eType)
		{
			m_iTextureIndex = 7;
			m_iElementalIndex = 1;
		}
		else
		{
			m_iTextureIndex = 8;
			m_iElementalIndex = 2;
		}
		break;
	}
}

HRESULT CUI_WeaponSection_Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Weapon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	if (nullptr == m_pCharacter)
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return E_FAIL;

		m_pCharacter = pPlayer->Get_Character();
		if (nullptr == m_pCharacter)
			return E_FAIL;
	}

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	
	return S_OK;
}

void CUI_WeaponSection_Weapon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_WeaponSection_Weapon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_WeaponSection_Weapon::Render()
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

void CUI_WeaponSection_Weapon::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_WeaponSection_Weapon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_WeaponSection_Weapon::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_WeaponSection_Weapon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch(m_eSlotType)
	{
	case UI_WEAPONSLOT_TYPE::SLOT_WEAPON:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SKillSection_Weapon_Icons"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_WEAPONSLOT_TYPE::SLOT_ELEMENTAL:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_WeaponSection_Weapon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WeaponSection_Weapon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	switch (m_eSlotType)
	{
	case UI_WEAPONSLOT_TYPE::SLOT_WEAPON:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
		break;

	case UI_WEAPONSLOT_TYPE::SLOT_ELEMENTAL:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iElementalIndex)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

void CUI_WeaponSection_Weapon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{

	}
}

void CUI_WeaponSection_Weapon::Movement_BasedOnHiding(_float fTimeDelta)
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

CUI_WeaponSection_Weapon* CUI_WeaponSection_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_WEAPONSLOT eType, UI_WEAPONSLOT_TYPE eSlotType)
{
	CUI_WeaponSection_Weapon* pInstance = new CUI_WeaponSection_Weapon(pDevice, pContext, eType, eSlotType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WeaponSection_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WeaponSection_Weapon::Clone(void* pArg)
{
	CUI_WeaponSection_Weapon* pInstance = new CUI_WeaponSection_Weapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WeaponSection_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WeaponSection_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
