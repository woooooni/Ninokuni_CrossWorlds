#include "stdafx.h"
#include "UI_Emoticon_Button.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Emoticon_Button::CUI_Emoticon_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EMOTICON_BTN eType)
	: CUI(pDevice, pContext, L"UI_Emoticon_Button")
	, m_eType(eType)
{
}

CUI_Emoticon_Button::CUI_Emoticon_Button(const CUI_Emoticon_Button& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_Emoticon_Button::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Emoticon_Button::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_fAlpha = 0.9f;

	m_bUseMouse = true;

	return S_OK;
}

void CUI_Emoticon_Button::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Emoticon_Button::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Emoticon_Button::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

void CUI_Emoticon_Button::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Emoticon_Button::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			CUI_Manager::GetInstance()->Set_EmoticonType(_uint(m_eType));
			CUI_Manager::GetInstance()->OnOff_EmoticonBalloon(true);

			switch (m_eType)
			{
			case EMOTIONBTN_FIRST:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Worry_1.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_SECOND:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Happy_3.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_THIRD:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Angry_2.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_FOURTH:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Laugh_2.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_FIFTH:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Sad_5_2.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_SIXTH:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Answer_2.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_SEVENTH:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Cast_2.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;

			case EMOTIONBTN_EIGHTH:
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("Funya_V_Emo_Surprise_1.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
				break;
			}
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Emoticon_Button::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Emoticon_Button::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_Button"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Emoticon_Button::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Emoticon_Button::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
		return E_FAIL;

	return S_OK;
}

CUI_Emoticon_Button* CUI_Emoticon_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EMOTICON_BTN eType)
{
	CUI_Emoticon_Button* pInstance = new CUI_Emoticon_Button(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Emoticon_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Emoticon_Button::Clone(void* pArg)
{
	CUI_Emoticon_Button* pInstance = new CUI_Emoticon_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Emoticon_Button");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Emoticon_Button::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
