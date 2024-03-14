#include "stdafx.h"
#include "UI_Basic.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Character_Manager.h"
#include "Player.h"
#include "UI_Fade.h"

CUI_Basic::CUI_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType)
	: CUI(pDevice, pContext, strObjectTag), m_eType(eType)
{
}

CUI_Basic::CUI_Basic(const CUI_Basic& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Basic::Set_Active(_bool bActive)
{
	if (m_eType == WORLDMAP_BACKCOLOR)
	{
		CPlayer* pPlayer = nullptr;
		pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;

		CCharacter* pCharacter = nullptr;
		pCharacter = pPlayer->Get_Character();
		if (nullptr == pCharacter)
			return;

		if (true == bActive)
		{
			pCharacter->Set_Skill_Input(false);
			pCharacter->Set_Attack_Input(false);
			pCharacter->Set_Move_Input(false);
		}
		else
		{
			pCharacter->Set_Skill_Input(true);
			pCharacter->Set_Attack_Input(true);
			pCharacter->Set_Move_Input(true);
		}
	}

	m_bActive = bActive;
}

void CUI_Basic::Set_AnnouncePosition(_float2 vBtnPos)
{
	if (COSTUME_INSTALL != m_eType)
		return;

	m_tInfo.fX = vBtnPos.x;
	m_tInfo.fY = vBtnPos.y;

	m_iPass = 9;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	if (!Get_Active())
		Set_Active(true);
}

void CUI_Basic::Set_WorldmapIcon()
{
	if (WORLDMAP_ICON != m_eType)
		return;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return;

	CCharacter* pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return;

	CHARACTER_TYPE eType = pCharacter->Get_CharacterType();
	switch (eType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_iTextureIndex = 0;
		break;

	case CHARACTER_TYPE::DESTROYER:
		m_iTextureIndex = 1;
		break;

	case CHARACTER_TYPE::ENGINEER:
		m_iTextureIndex = 2;
		break;
	}

//	m_tInfo.fCX = 108.f * 0.5f;
//	m_tInfo.fCY = 128.f * 0.5f;
//	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

void CUI_Basic::Update_Position(_float2 vPosition)
{
	if (WORLDMAP_ICON != m_eType)
		return;

	m_tInfo.fX = vPosition.x;
	m_tInfo.fY = vPosition.y - (m_tInfo.fCY * 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

HRESULT CUI_Basic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Basic::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (UIMAPNAME_EVERMORE == m_eType || UIMAPNAME_KINGDOM == m_eType ||
		UIMAPNAME_ICELAND == m_eType || UIMAPNAME_FOREST == m_eType)
	{
		// 알파값, Active조정이 필요한 Type
		m_bActive = false;
		m_bFade = true;
	}
	else if (UIQUEST_ACCEPT == m_eType || UIQUEST_FINISH == m_eType || COSTUME_INSTALL == m_eType)
	{
		m_bActive = false;
	}
	else if (UILOBBY_DICE == m_eType)
		Ready_Nickname();

	// UI Click Event
	if (UILOBBY_DICE == m_eType || UIQUEST_ACCEPT == m_eType || UIQUEST_FINISH == m_eType)
		m_bUseMouse = true;

	m_iTextureIndex = 0;

	return S_OK;
}

void CUI_Basic::Tick(_float fTimeDelta)
{
	if (UIBASIC_END == m_eType)
		return;

	if (m_bActive)
	{
		if (true == m_bFade)
			Tick_FadeObject(fTimeDelta);

		if (m_eType == UILOBBY_ANNOUNCE)
		{
			if (false == m_bAlpha)
			{
				if (m_fAlpha <= 0.7f)
				{
					m_fAlpha -= fTimeDelta * 0.5f;

					if (m_fAlpha <= 0.f)
					{
						m_bAlpha = true;
						m_fAlpha = 0.f;
					}
				}
			}
			else
			{
				m_fAlpha += fTimeDelta * 0.5f;

				if (m_fAlpha >= 0.7f)
				{
					m_bAlpha = false;
					m_fAlpha = 0.7f;
				}
			}
		}
		else if (m_eType == VEHICLE_FX) // 알파값을 조정한다.
		{
			if (true == m_bAlpha)
			{
				m_fAlpha -= fTimeDelta * 2.f;

				if (m_fAlpha <= 0.f)
				{
					m_bAlpha = false;
					m_fAlpha = 0.f;
				}
			}
			else
			{
				m_fAlpha += fTimeDelta * 2.f;

				if (1.f <= m_fAlpha)
				{
					m_bAlpha = true;
					m_fAlpha = 1.f;
				}
			}

			// 회전시킨다
			m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta * 2.f);
		}
		else if (m_eType == UI_ENDING)
		{
			if (false == m_bEnd)
			{
				if (false == m_bAlpha)
				{
					m_fAlpha += fTimeDelta * 0.5f;

					if (1.f <= m_fAlpha)
					{
						m_fAlpha = 1.f;
						m_bAlpha = true;
					}
				}
				else
				{
					// 시간을 누적한다.
					m_fTimeAcc += fTimeDelta;

					if (6.f < m_fTimeAcc)
					{
						m_bEnd = true;
						m_fTimeAcc = 0.f;
						CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 2.f, true);
					}
				}
			}
		}
		else if (m_eType == WORLDMAP_ICON)
			Set_WorldmapIcon();
		else if (m_eType == WORLDMAP_BACKCOLOR)
		{
			CPlayer* pPlayer = nullptr;
			pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;

			CCharacter* pCharacter = nullptr;
			pCharacter = pPlayer->Get_Character();
			if (nullptr == pCharacter)
				return;

			pCharacter->Set_Skill_Input(false);
			pCharacter->Set_Attack_Input(false);
			pCharacter->Set_Move_Input(false);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Basic::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bFade)
		{
			LateTick_FadeObject(fTimeDelta);
		}
		else
		{
			if (UILOBBY_ANNOUNCE == m_eType)
			{
				if(CUI_Manager::GetInstance()->Is_NicknameSettingComplete())
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
			}
			else
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}

		if (UILOBBY_NICKFRAME == m_eType)
		{
			m_strText = CGame_Manager::GetInstance()->Get_UserName();
			// 글씨를 추가한다.
			CRenderer::TEXT_DESC NickDesc = {};
			NickDesc.strText = m_strText;
			_int iTotalLength = m_strText.length();
			_int iSpace = CUI_Manager::GetInstance()->Count_WordSpacing(m_strText);
			_int iLength = iTotalLength - iSpace;

			NickDesc.strFontTag = L"Default_Bold";
			NickDesc.vScale = { 0.6f, 0.6f };
			NickDesc.vPosition = _float2(m_tInfo.fX - (iLength * 11.f + iSpace * 2.f), m_tInfo.fY - 5.f);
			NickDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(NickDesc);
		}

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Basic::Render()
{
	if (m_bActive)
	{
		if (m_bFade)
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(1);

			m_pVIBufferCom->Render();

			return S_OK;
		}
		else
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(m_iPass);

			m_pVIBufferCom->Render();

			return S_OK;
		}

		__super::Render();
	}

	return S_OK;
}

void CUI_Basic::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UILOBBY_DICE == m_eType)
		{
			CUI_Manager::GetInstance()->Set_MouseCursor(0);
		}
	}
}

void CUI_Basic::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UILOBBY_DICE == m_eType)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("UI_Fx_Comm_ItemLooting_Appear_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

				Set_RandomNickname();
			}
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Basic::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UILOBBY_DICE == m_eType)
		{
			CUI_Manager::GetInstance()->Set_MouseCursor(1);
		}

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Basic::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UILOBBY_NICKFRAME: // Enum값 LEVEL_LOBBY
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_NicknameFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UILOBBY_TITLE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_TitleText"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UILOBBY_BTNTEXT:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_BtnText"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UILOBBY_ANNOUNCE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Announce"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		m_bAlpha = false;
		break;

	case UILOBBY_DICE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Dice"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UIMAPNAME_EVERMORE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_Evermore"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_KINGDOM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_KingdomHall"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_ICELAND:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_IceLand"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_FOREST:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_WitchForest"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIQUEST_ACCEPT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestAccept"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIQUEST_FINISH:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestFinish"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case TARGETARROW_LEFT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Left"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case TARGETARROW_RIGHT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Right"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case COSTUME_INSTALL:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Announce_Install"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case WORLDMAP_ICON:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapPortrait"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UISTATIC_TITLELINE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleLine"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UISTATIC_MENUDECO_L:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenuLeft"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		break;

	case UISTATIC_MENUDECO_R:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenRight"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		break;

	case VEHICLE_FX:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Vehicle_FXTexture"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bAlpha = true;
		m_fAlpha = 1.f;
		break;

	case UI_ENDING:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Ending_Logo"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bEnd = false;
		m_bAlpha = false;
		m_bActive = false;
		m_fAlpha = 0.f;
		m_fTimeAcc = 0.f;
		break;

	case WORLDMAP_BACKCOLOR:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMap_BackColor"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		m_bActive = false;
		break;

	default:
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Basic::Ready_State()
{
	if (WORLDMAP_ICON == m_eType)
	{
		m_tInfo.fCX = 108.f * 0.5f;
		m_tInfo.fCY = 128.f * 0.5f;
		m_tInfo.fX = g_iWinSizeX * 0.5f;
		m_tInfo.fY = g_iWinSizeY * 0.5f;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Basic::Bind_ShaderResources()
{
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

void CUI_Basic::Tick_FadeObject(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Basic::LateTick_FadeObject(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

void CUI_Basic::Ready_Nickname()
{
	m_iMaxNick = 5;

	m_RandomNickname.reserve(m_iMaxNick);
	
	m_RandomNickname.push_back(TEXT("너네선비형"));
	m_RandomNickname.push_back(TEXT("욕반장"));
	m_RandomNickname.push_back(TEXT("게살버거형"));
	m_RandomNickname.push_back(TEXT("나는김태원"));
	m_RandomNickname.push_back(TEXT("준엽2"));
//	m_RandomNickname.push_back(TEXT("Binary는호남선"));
//	m_RandomNickname.push_back(TEXT("NumRowColChar"));
}

void CUI_Basic::Set_RandomNickname()
{
	// 주사위만 사용하는 함수
	_int iRandom;

	do {
		iRandom = GI->RandomInt(0, m_iMaxNick - 1);

	} while (m_iRandomNum == iRandom);
	// 같지 않은 숫자가 나오면 빠져나온다.

	// Text를 세팅해주고 랜덤넘버를 저장한다.
	CUI_Manager::GetInstance()->Set_RandomNick(m_RandomNickname[iRandom]);
	m_iRandomNum = iRandom;
}

CUI_Basic* CUI_Basic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType)
{
	CUI_Basic* pInstance = new CUI_Basic(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Basic::Clone(void* pArg)
{
	CUI_Basic* pInstance = new CUI_Basic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Basic::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
