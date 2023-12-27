#include "stdafx.h"
#include "UI_SubMenu_Equipment.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SubMenu_Equipment::CUI_SubMenu_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SUBEQUIP_TYPE eUIType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eUIType(eUIType)
{
}

CUI_SubMenu_Equipment::CUI_SubMenu_Equipment(const CUI_SubMenu_Equipment& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}

void CUI_SubMenu_Equipment::Set_Active(_bool bActive)
{
	if (EQUIPMENU_END == m_eUIType)
		return;

	if (bActive)
	{
		if (m_bUsable)
			m_fAlpha = 0.4f;

		m_tInfo.fY = m_vOnPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		m_bUpdateAlpha = false;
		m_bArrived = false;
	}
	else
	{

	}

	m_bActive = bActive;
}

HRESULT CUI_SubMenu_Equipment::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SubMenu_Equipment::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// 생성 위치는 좌표로 지정해준다.

	if (EQUIPMENU_END == m_eUIType)
		return E_FAIL;


	// 첫번째 열에 생성되는 Btn -> 기준점 좌표 EQUIPBTN_INVENTORY
	m_vOnPosition.x = 230.f;
	m_vOnPosition.y = 245.f;

	// 목표 위치는 초기 세팅위치이다.
	m_vGoalPosition.x = m_tInfo.fX;
	m_vGoalPosition.y = m_tInfo.fY; // Y값만 사용함

	m_bActive = false;

	return S_OK;
}

void CUI_SubMenu_Equipment::Tick(_float fTimeDelta)
{
	if (EQUIPMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_bArrived)
		{
			m_tInfo.fY = m_vGoalPosition.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else
		{
			if (m_tInfo.fY > m_vGoalPosition.y)
				m_bArrived = true;

			m_tInfo.fY += fTimeDelta * m_fSpeed;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SubMenu_Equipment::LateTick(_float fTimeDelta)
{
	if (EQUIPMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_bUsable)
		{
			if (!m_bUpdateAlpha)
			{
				// 알파값을 갱신한다.
				if (m_fAlpha < 0.9f)
				{
					m_fAlpha += fTimeDelta;
				}
				else if (0.9f <= m_fAlpha)
				{
					m_bUpdateAlpha = true;
					m_fAlpha = 0.9f;
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SubMenu_Equipment::Render()
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

void CUI_SubMenu_Equipment::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_SubMenu_Equipment::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			if (m_eUIType == EQUIPBTN_INVENTORY)
			{
				CUI_Manager::GetInstance()->OnOff_Inventory(true);
				// 만약에 이 버튼을 통해서 들어오면 Off할때 SubMenu가 떠있는 상태로 하는 것도 좋을 것 같음.
			}
		}
	}
}

void CUI_SubMenu_Equipment::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_SubMenu_Equipment::Reset_InitializeInfo()
{
//	if (BTN_CLICKED == m_eTextureType)
//	{
//		m_bActive = false;
//		m_bClicked = false;
//		m_bArrived = false;
//		m_bMoveStart = false;
//		m_bMoveEnd = false;
//	}
}

HRESULT CUI_SubMenu_Equipment::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	Ready_Buttons();

	if (m_bUsable) // 사용할 아이콘들 -> alpha값 증감이 있을 것이다.
	{
		m_fAlpha = 0.4f;
	}
	else // 사용하지 않을 아이콘들 -> alpha값이 유지된다.
	{
		m_fAlpha = 0.3f;
	}
	
	return S_OK;
}

HRESULT CUI_SubMenu_Equipment::Ready_Buttons()
{
	_float fStandard = 300.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_EquipmentTab"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_eUIType)
	{
	case EQUIPBTN_INVENTORY:
		m_iTextureIndex = 0;
		m_fSpeed = 0.f;
		m_bUsable = true;
		break;
	case EQUIPBTN_FORGE:
		m_iTextureIndex = 1;
		m_fSpeed = fStandard;
		m_bUsable = false;
		break;
	case EQUIPBTN_GEM:
		m_iTextureIndex = 2;
		m_fSpeed = fStandard * 2.f;
		m_bUsable = false;
		break;
	case EQUIPBTN_TINYGENIE:
		m_iTextureIndex = 3;
		m_fSpeed = fStandard * 3.f;
		m_bUsable = false;
		break;
	case EQUIPBTN_TRANSCEND:
		m_iTextureIndex = 4;
		m_fSpeed = fStandard * 4.f;
		m_bUsable = false;
		break;
	case EQUIPBTN_FISHINGROD:
		m_iTextureIndex = 5;
		m_fSpeed = fStandard * 5.f;
		m_bUsable = false;
		break;
	}

	return S_OK;
}

HRESULT CUI_SubMenu_Equipment::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SubMenu_Equipment::Bind_ShaderResources()
{
	if (m_iTextureIndex < 0 || 7 < m_iTextureIndex)
		return E_FAIL;

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

	return S_OK;
}

CUI_SubMenu_Equipment* CUI_SubMenu_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SUBEQUIP_TYPE eUIType)
{
	CUI_SubMenu_Equipment* pInstance = new CUI_SubMenu_Equipment(pDevice, pContext, strObjectTag, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SubMenu_Equipment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SubMenu_Equipment::Clone(void* pArg)
{
	CUI_SubMenu_Equipment* pInstance = new CUI_SubMenu_Equipment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SubMenu_Equipment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SubMenu_Equipment::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
