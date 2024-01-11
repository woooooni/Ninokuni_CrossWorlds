#include "stdafx.h"
#include "UI_WeaponSection_Selected.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Weapon_Manager.h"
#include "Player.h"
#include "Weapon.h"

CUI_WeaponSection_Selected::CUI_WeaponSection_Selected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSection_Selected")
{
}

CUI_WeaponSection_Selected::CUI_WeaponSection_Selected(const CUI_WeaponSection_Selected& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WeaponSection_Selected::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Selected::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMaxSize = _float2(m_vOriginSize.x * 1.3f, m_vOriginSize.y * 1.3f);

	m_bActive = true; 

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);

	return S_OK;
}

void CUI_WeaponSection_Selected::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		if (!m_bResize)
		{
			m_tInfo.fCX += fTimeDelta * m_fSpeed;
			m_tInfo.fCY += fTimeDelta * m_fSpeed;

			if (m_tInfo.fCX >= m_vMaxSize.x)
			{
				m_bResize = true;
				m_tInfo.fCX = m_vMaxSize.x;
				m_tInfo.fCY = m_vMaxSize.y;
			}

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}
		else
		{
			m_tInfo.fCX -= fTimeDelta * m_fSpeed;
			m_tInfo.fCY -= fTimeDelta * m_fSpeed;

			if (m_tInfo.fCX <= m_vOriginSize.x)
			{
				m_bResize = false;
				m_tInfo.fCX = m_vOriginSize.x;
				m_tInfo.fCY = m_vOriginSize.y;
			}

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_WeaponSection_Selected::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_WeaponSection_Selected::Render()
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

void CUI_WeaponSection_Selected::Update_Position(_uint iSlotNum)
{
	if (2 < iSlotNum)
		return;

	switch (iSlotNum)
	{
	case 0:
		m_tInfo.fX = m_vPosition[0].x;
		m_tInfo.fY = m_vPosition[0].y;
		break;

	case 1:
		m_tInfo.fX = m_vPosition[1].x;
		m_tInfo.fY = m_vPosition[1].y;
		break;

	case 2:
		m_tInfo.fX = m_vPosition[2].x;
		m_tInfo.fY = m_vPosition[2].y;
		break;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

void CUI_WeaponSection_Selected::Change_Weapon(_uint iSlotNum)
{
	if (2 < iSlotNum)
		return;

	CModel* pCurrentModel = nullptr;
	CModel* pWeaponModel = nullptr;
	ELEMENTAL_TYPE eElementalType = ELEMENTAL_TYPE::ELEMENTAL_END;

	_bool bChange = true;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return;
	CWeapon* pWeapon = pCharacter->Get_Weapon();
	if (nullptr == pWeapon)
		return;

	pCurrentModel = pWeapon->Get_WeaponModelCom();
	if (pCurrentModel == nullptr)
		return;
	wstring strCurWeapon = pCurrentModel->Get_Name();

	bChange = Isit_PossibleToChange(strCurWeapon);
	// 가능하면 Set_WeaponModelCom을 수행한다.

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		switch (iSlotNum)
		{
		case 0:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Sword_Fire02"));
			eElementalType = ELEMENTAL_TYPE::FIRE;
			break;
		case 1:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Sword_Water02"));
			eElementalType = ELEMENTAL_TYPE::WATER;
			break;
		case 2:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Sword_Wood02"));
			eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		switch (iSlotNum)
		{
		case 0:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Hammer_Fire02"));
			eElementalType = ELEMENTAL_TYPE::FIRE;
			break;
		case 1:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Hammer_Water02"));
			eElementalType = ELEMENTAL_TYPE::WATER;
			break;
		case 2:
			pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayerType, TEXT("Hammer_Wood02"));
			eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		break;
	}

	if (bChange)
	{
		if (nullptr == pWeaponModel)
			return;

		pWeapon->Set_WeaponModelCom(pWeaponModel);
	}

	if (ELEMENTAL_TYPE::ELEMENTAL_END <= eElementalType)
		return;
	pCharacter->Set_ElementalType(eElementalType);
	CUI_Manager::GetInstance()->Change_ElementalType(eElementalType);
}

_bool CUI_WeaponSection_Selected::Isit_PossibleToChange(const wstring& strModelTag)
{
	_bool bCanChange = true;

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (TEXT("Sword_Flower01") == strModelTag || TEXT("Sword_QQSuits") == strModelTag)
			bCanChange = false;
		break;

	case CHARACTER_TYPE::DESTROYER:
		break;

	case CHARACTER_TYPE::ENGINEER:
		break;
	}

	return bCanChange;
}

HRESULT CUI_WeaponSection_Selected::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Selected"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponSection_Selected::Ready_State()
{
	_float fSize = 128.f * 0.6f;

	_float2 vDefaultOffset = _float2(17.f, -170.f);
	_float fOffset = 85.f;

	m_tInfo.fCX = fSize;
	m_tInfo.fCY = fSize;

	_float fParentfCX = 400.f * 0.75f;
	_float fParentfCY = 400.f * 0.75f;
	_float fParentfX = g_iWinSizeX - fParentfCX * 0.5f;
	_float fParentfY = g_iWinSizeY - fParentfCY * 0.5f - 5.f;

	m_tInfo.fX = fParentfX + (vDefaultOffset.x - fOffset);
	m_tInfo.fY = fParentfY + vDefaultOffset.y;

	m_vPosition[0] = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vPosition[1] = _float2(m_tInfo.fX + fOffset, m_tInfo.fY);
	m_vPosition[2] = _float2(m_tInfo.fX + fOffset * 2.f, m_tInfo.fY);

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WeaponSection_Selected::Bind_ShaderResources()
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

void CUI_WeaponSection_Selected::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
				m_bHideFinish = true;
			else
				m_tInfo.fX += (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 1.2f);
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
					m_tInfo.fX -= (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 1.2f);
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_WeaponSection_Selected* CUI_WeaponSection_Selected::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WeaponSection_Selected* pInstance = new CUI_WeaponSection_Selected(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WeaponSection_Selected");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WeaponSection_Selected::Clone(void* pArg)
{
	CUI_WeaponSection_Selected* pInstance = new CUI_WeaponSection_Selected(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WeaponSection_Selected");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WeaponSection_Selected::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
