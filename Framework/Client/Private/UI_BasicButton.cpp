#include "stdafx.h"
#include "UI_BasicButton.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Action.h"

CUI_BasicButton::CUI_BasicButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType)
	: CUI(pDevice, pContext, strObjectTag), m_eType(eType)
{
}

CUI_BasicButton::CUI_BasicButton(const CUI_BasicButton& rhs)
	: CUI(rhs), m_eType(rhs.m_eType)
{
}

HRESULT CUI_BasicButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BasicButton::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vOriginSize.x = m_tInfo.fCX;
	m_vOriginSize.y = m_tInfo.fCY;
	m_vMinSize.x = m_vOriginSize.x * 0.95f;
	m_vMinSize.y = m_vOriginSize.y * 0.95f;

	if (BUTTON_CHANGESCENE == m_eType)
	{
		_float2 vSize = _float2(600.f, 52.f);
		_float fX = (g_iWinSizeX * 0.5f) - ((512.f * 0.4f) - 23.f);
		Make_Child(-fX, 170.f - g_iWinSizeY, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Lobby_Announce"));
	}

	if (BUTTON_SETNICKNAME == m_eType)
	{
		m_bCanClick = true;
	}

	return S_OK;
}

void CUI_BasicButton::Tick(_float fTimeDelta)
{
	if (m_bResizeStart)
	{
		// 시간을 누적한다.
		m_fTimeAcc += fTimeDelta;

		// MinSize로 Info를 변경해서 setting한다.
		m_tInfo.fCX = m_vMinSize.x;
		m_tInfo.fCY = m_vMinSize.y;
		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}

	__super::Tick(fTimeDelta);
}

void CUI_BasicButton::LateTick(_float fTimeDelta)
{
	if (BUTTON_CHANGESCENE == m_eType)
	{
		if (m_bCanClick)
		{
			Set_ChildActive(false);

			if (1.f < m_fTimeAcc) // 누적한 시간이 기준치 이상이되면
			{
				m_fTimeAcc = 0.f; // 0.f로 초기화하고,
				m_bFinish = true; // Finish를 true로 변경해준다
				// Finish가 true가 되면 Event가 발생한다.
			}
		}

		if (L"UI_Btn_Basic_GameStart" == Get_ObjectTag())
		{
			if (m_bFinish)
				if (FAILED(GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_EVERMORE, L""))))
					return;
		}

		__super::LateTick(fTimeDelta);
	}
	
	if (BUTTON_SETNICKNAME == m_eType)
	{
		if (m_bCanClick)
		{
			if (0.3f < m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				m_bFinish = true;
			}

			if (m_bFinish)
				CUI_Manager::GetInstance()->OnOff_NickNameWindow(false);
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_BasicButton::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	if (BUTTON_CHANGESCENE == m_eType)
		__super::Render();

	return S_OK;
}

void CUI_BasicButton::On_MouseEnter(_float fTimeDelta)
{
	// 사이즈 조절
	if (m_bCanClick)
	{
		m_tInfo.fCX = m_vMinSize.x;
		m_tInfo.fCY = m_vMinSize.y;

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		
	}
}

void CUI_BasicButton::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bCanClick)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				if (!m_bResizeStart)
					m_bResizeStart = true;

				if (BUTTON_CHANGESCENE == m_eType)
				{
					GI->Stop_Sound(CHANNELID::SOUND_UI);
					GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

					// 선택된 Player의 음성을 재생한다.
					_int iIndex = CUI_Manager::GetInstance()->Get_SelectedCharacter();

					if (iIndex == -1)
						return;

					switch (iIndex)
					{
					case 1: // 디스트로이어
						GI->Stop_Sound(CHANNELID::SOUND_VOICE_CHARACTER);
						switch (GI->RandomInt(0, 3))
						{
						case 0:
							GI->Play_Sound(TEXT("Destroyer_System_Character_Start_1_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;
						case 1:
							GI->Play_Sound(TEXT("Destroyer_System_Character_Start_2_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;

						case 2:
							GI->Play_Sound(TEXT("Destroyer_System_Character_Start_3_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;
						}
						break;

					case 3: // 엔지니어
						GI->Stop_Sound(CHANNELID::SOUND_VOICE_CHARACTER);
						switch (GI->RandomInt(0, 3))
						{
						case 0:
							GI->Play_Sound(TEXT("Engineer_System_Character_Start_1_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;

						case 1:
							GI->Play_Sound(TEXT("Engineer_System_Character_Start_2_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;

						case 2:
							GI->Play_Sound(TEXT("Engineer_System_Character_Start_3_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;
						}
						break;

					case 4: // 소드맨
						GI->Stop_Sound(CHANNELID::SOUND_VOICE_CHARACTER);
						switch (GI->RandomInt(0, 3))
						{
						case 0:
							GI->Play_Sound(TEXT("Swordsman_System_Character_Start_1_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;

						case 1:
							GI->Play_Sound(TEXT("Swordsman_System_Character_Start_2_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;

						case 2:
							GI->Play_Sound(TEXT("Swordsman_System_Character_Start_3_Short.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
							break;
						}
						break;
					}
				}

				else if (BUTTON_SETNICKNAME == m_eType)
				{
					//if (KEY_TAP(KEY::LBTN))
					{
						CUI_Manager::GetInstance()->Set_Textable(false);
						CUI_Manager::GetInstance()->Set_UserName();

						GI->Stop_Sound(CHANNELID::SOUND_UI);
						GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

						CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_CurCamera());
						if (nullptr != pActionCam)
						{
							pActionCam->Start_Action_Lobby();
						}
					}
				}
			}

			__super::On_Mouse(fTimeDelta);
		}
	}
}

void CUI_BasicButton::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bCanClick)
		{
			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_BasicButton::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case BUTTON_CHANGESCENE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Btn_Basic"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case BUTTON_SETNICKNAME:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Button"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_BasicButton::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BasicButton::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_BasicButton* CUI_BasicButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType)
{
	CUI_BasicButton* pInstance = new CUI_BasicButton(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BasicButton::Clone(void* pArg)
{
	CUI_BasicButton* pInstance = new CUI_BasicButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BasicButton::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
