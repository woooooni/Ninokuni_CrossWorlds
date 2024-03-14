#include "stdafx.h"
#include "UI_MainMenu.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"

CUI_MainMenu::CUI_MainMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_MAINMENU_TYPE eUIType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eUIType(eUIType)
{
}

CUI_MainMenu::CUI_MainMenu(const CUI_MainMenu& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}

void CUI_MainMenu::Set_Active(_bool bActive)
{
	if (UIMAINMENU_END == m_eUIType)
		return;

	if (MAINMENU_BG != m_eUIType)
	{
		if (bActive)
		{
			m_tInfo.fY = m_vOnPosition.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

			m_bArrived = false;
		}
		else
		{
			if (m_bEvent)
			{
				m_bEvent = false;
				//CUI_Manager::GetInstance()->Set_UIClicked(m_bEvent);
			}
		}

		m_bActive = bActive;
	}
	else
	{
		if (m_bEvent)
		{
			m_bEvent = false;
			//CUI_Manager::GetInstance()->Set_UIClicked(m_bEvent);
		}

		m_bActive = bActive;
	}
}

HRESULT CUI_MainMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MainMenu::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (MAINMENU_BG != m_eUIType)
	{
		// 생성 위치는 좌표로 지정해준다.
		if (MAINBTN_CHARACTER == m_eUIType || MAINBTN_EQUIPMENT == m_eUIType || MAINBTN_IMAJINN == m_eUIType ||
			MAINBTN_RECORD == m_eUIType || MAINBTN_GROWTH == m_eUIType)
		{
			// 좌측에 생성되는 Btn -> 기준점 좌표 MAINBTN_CHARACTER
			m_vOnPosition.x = 80.f;
			m_vOnPosition.y = 285.f;
		}
		else if (MAINBTN_COMMUNITY == m_eUIType || MAINBTN_DUNGEON == m_eUIType || MAINBTN_CHALLENGE == m_eUIType ||
			MAINBTN_BATTLE == m_eUIType || MAINBTN_SHOP == m_eUIType)
		{
			// 우측에 생성되는 Btn -> 기준점 좌표 MAINBTN_SHOP

			m_vOnPosition.x = 1520.f;
			m_vOnPosition.y = 625.f;
		}
		else
		{
			return E_FAIL;
		}

		// 목표 위치는 초기 세팅위치이다.
		m_vGoalPosition.x = m_tInfo.fX;
		m_vGoalPosition.y = m_tInfo.fY; // Y값만 사용함
	}

	m_bActive = false;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_MainMenu::Tick(_float fTimeDelta)
{
	if (UIMAINMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (MAINMENU_BG != m_eUIType)
		{
			if (m_bArrived)
			{
				m_tInfo.fY = m_vGoalPosition.y;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
			}
			else
			{
				if (MAINBTN_CHARACTER == m_eUIType || MAINBTN_EQUIPMENT == m_eUIType || MAINBTN_IMAJINN == m_eUIType ||
					MAINBTN_RECORD == m_eUIType || MAINBTN_GROWTH == m_eUIType)
				{
					if (m_tInfo.fY > m_vGoalPosition.y)
						m_bArrived = true;
				}
				else
				{
					if (m_tInfo.fY < m_vGoalPosition.y)
						m_bArrived = true;
				}

				m_tInfo.fY += fTimeDelta * m_fSpeed;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_MainMenu::LateTick(_float fTimeDelta)
{
	if (UIMAINMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MainMenu::Render()
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

void CUI_MainMenu::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_MainMenu::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI2);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI2, GI->Get_ChannelVolume(CHANNELID::SOUND_UI2));

			switch (m_eUIType)
			{
			case MAINBTN_CHARACTER:
				CUI_Manager::GetInstance()->OnOff_SubMenu(true, _uint(MAINBTN_CHARACTER));
				break;
			case MAINBTN_EQUIPMENT:
				CUI_Manager::GetInstance()->OnOff_SubMenu(true, _uint(MAINBTN_EQUIPMENT));
				break;
			case MAINBTN_IMAJINN:
				CUI_Manager::GetInstance()->OnOff_SubMenu(true, _uint(MAINBTN_IMAJINN));
				break;
			case MAINBTN_SHOP:
				CUI_Manager::GetInstance()->OnOff_SubMenu(true, _uint(MAINBTN_SHOP));
				break;
			}
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_MainMenu::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_MainMenu::Reset_InitializeInfo()
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

HRESULT CUI_MainMenu::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (MAINMENU_BG == m_eUIType)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MainMenu_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_iTextureIndex = 0;
	}
	else
	{
		Ready_Buttons();
	}

	m_bActive = false;
	
	return S_OK;
}

HRESULT CUI_MainMenu::Ready_Buttons()
{
	_float fStandard = 300.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MainMenu_Button"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_eUIType)
	{
	case MAINBTN_CHARACTER:
		m_iTextureIndex = 0;
		m_fSpeed = 0.f;
		m_fAlpha = 1.f;
		break;
	case MAINBTN_EQUIPMENT:
		m_iTextureIndex = 1;
		m_fSpeed = fStandard;
		m_fAlpha = 1.f;
		break;
	case MAINBTN_IMAJINN:
		m_iTextureIndex = 2;
		m_fSpeed = fStandard * 2.f;
		m_fAlpha = 1.f;
		break;
	case MAINBTN_RECORD:
		m_iTextureIndex = 3;
		m_fSpeed = fStandard * 3.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_GROWTH:
		m_iTextureIndex = 4;
		m_fSpeed = fStandard * 4.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_COMMUNITY:
		m_iTextureIndex = 5;
		m_fSpeed = fStandard * -4.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_DUNGEON:
		m_iTextureIndex = 6;
		m_fSpeed = fStandard * -3.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_CHALLENGE:
		m_iTextureIndex = 7;
		m_fSpeed = fStandard * -2.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_BATTLE:
		m_iTextureIndex = 8;
		m_fSpeed = fStandard * -1.f;
		m_fAlpha = 0.4f;
		break;
	case MAINBTN_SHOP:
		m_iTextureIndex = 9;
		m_fSpeed = 0.f;
		m_fAlpha = 1.f;
		break;
	}

	return S_OK;
}

HRESULT CUI_MainMenu::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MainMenu::Bind_ShaderResources()
{
	if (MAINMENU_BG == m_eUIType)
	{
		if (m_iTextureIndex != 0)
			return E_FAIL;
	}
	else
	{
		if (m_iTextureIndex < 0 || 9 < m_iTextureIndex)
			return E_FAIL;
	}


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

CUI_MainMenu* CUI_MainMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_MAINMENU_TYPE eUIType)
{
	CUI_MainMenu* pInstance = new CUI_MainMenu(pDevice, pContext, strObjectTag, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MainMenu::Clone(void* pArg)
{
	CUI_MainMenu* pInstance = new CUI_MainMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MainMenu::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
