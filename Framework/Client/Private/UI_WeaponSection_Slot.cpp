#include "stdafx.h"
#include "UI_WeaponSection_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_WeaponSection_DefaultWeapon.h"
#include "UI_SkillSection_CoolTimeFrame.h"
#include "Game_Manager.h"
#include "Player.h"

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

void CUI_WeaponSection_Slot::Set_CharacterType(CHARACTER_TYPE eType)
{
	m_eCurPlayerType = eType;

	if (nullptr != m_pNoWeapon)
		m_pNoWeapon->Set_CharacterType(m_eCurPlayerType);
}

void CUI_WeaponSection_Slot::Use_BurstSkill()
{
	if (nullptr == m_pFrame)
		return;

	m_pFrame->Use_Skill();
}

void CUI_WeaponSection_Slot::Hide_UI(_bool bHide)
{
	m_bHide = bHide; m_bHideFinish = false;

	m_pFrame->Hide_UI(bHide);
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

	CGameObject* pNoWeapon = nullptr;
	pNoWeapon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_SkillSection_DefaultAttack"), LAYER_TYPE::LAYER_UI, &UIDesc);
	if (nullptr == pNoWeapon)
		return E_FAIL;

	m_pNoWeapon = dynamic_cast<CUI_WeaponSection_DefaultWeapon*>(pNoWeapon);
	m_pNoWeapon->Set_Resizable(false);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = m_tInfo.fCX;
	UIDesc.fCY = m_tInfo.fCY;
	UIDesc.fX = ParentDesc.fX + m_tInfo.fX;
	UIDesc.fY = ParentDesc.fY + m_tInfo.fY;
	CGameObject* pFrame = nullptr;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return E_FAIL;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return E_FAIL;

	CHARACTER_TYPE eCharacterType = pCharacter->Get_CharacterType();
	if (CHARACTER_TYPE::CHARACTER_END == eCharacterType)
		return E_FAIL;

	m_eCurPlayerType = eCharacterType;

	switch (m_eSlotType)
	{
	case UI_WEAPONSLOT::WEAPONSLOT_FIRST:
		pFrame = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_First"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pFrame)
			return E_FAIL;
		m_pFrame = dynamic_cast<CUI_SkillSection_CoolTimeFrame*>(pFrame);
		m_pFrame->Set_CharacterType(m_eCurPlayerType);
		break;

	case UI_WEAPONSLOT::WEAPONSLOT_SECOND:
		pFrame = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_Second"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pFrame)
			return E_FAIL;
		m_pFrame = dynamic_cast<CUI_SkillSection_CoolTimeFrame*>(pFrame);
		m_pFrame->Set_CharacterType(m_eCurPlayerType);
		break;

	case UI_WEAPONSLOT::WEAPONSLOT_THIRD:
		pFrame = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_Third"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pFrame)
			return E_FAIL;
		m_pFrame = dynamic_cast<CUI_SkillSection_CoolTimeFrame*>(pFrame);
		m_pFrame->Set_CharacterType(m_eCurPlayerType);
		break;
	}

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);

	return S_OK;
}

void CUI_WeaponSection_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		if (nullptr != m_pFrame)
			m_pFrame->Tick(fTimeDelta);

		if (m_bWear) // 무기를 착용하지 않은상태라면
		{
			if (nullptr != m_pNoWeapon)
				m_pNoWeapon->Tick(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_WeaponSection_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pFrame)
			m_pFrame->LateTick(fTimeDelta);

		if (m_bWear) // 무기를 착용하지 않은상태라면
		{
			if (nullptr != m_pNoWeapon)
				m_pNoWeapon->LateTick(fTimeDelta);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_WeaponSection_Slot::Render()
{
	if (m_bActive)
	{
		if (nullptr != m_pFrame)
			m_pFrame->Render();

		if (m_bWear)
		{
			if (nullptr != m_pNoWeapon)
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

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_WeaponSection_Slot::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
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
		m_bClick = true;
		CUI_Manager::GetInstance()->Update_WeaponSelectionIcon(m_eSlotType);
	}
}

void CUI_WeaponSection_Slot::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
				m_bHideFinish = true;
			else
				m_tInfo.fX += (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 0.05f);
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
					m_tInfo.fX -= (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 0.05f);
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
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

	Safe_Release(m_pFrame);
	Safe_Release(m_pNoWeapon);
	Safe_Release(m_pTextureCom);
}
