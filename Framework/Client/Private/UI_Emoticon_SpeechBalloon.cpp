#include "stdafx.h"
#include "UI_Emoticon_SpeechBalloon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Game_Manager.h"
#include "UI_Emoticon_BalloonEmoticon.h"

CUI_Emoticon_SpeechBalloon::CUI_Emoticon_SpeechBalloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Emoticon_SpeechBalloon")
{
}

CUI_Emoticon_SpeechBalloon::CUI_Emoticon_SpeechBalloon(const CUI_Emoticon_SpeechBalloon& rhs)
	: CUI(rhs)
{
}

void CUI_Emoticon_SpeechBalloon::Set_Active(_bool bActive)
{
	if (bActive) // true
	{
		m_bAlpha = false;
		m_fAlpha = 0.9f;

		// 플레이어 찾는것 바꿔야함.
		//if (_uint(LEVELID::LEVEL_TEST) == GI->Get_CurrentLevel())
		{
			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;
			CCharacter* pCharacter = pPlayer->Get_Character();
			if (nullptr == pCharacter)
				return;

			CTransform* pTargetTransform = pCharacter->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTargetTransform)
				return;
			
			m_pCharacterTrans = pTargetTransform;
		}

//			GI->Stop_Sound(CHANNELID::SOUND_UI);
//			GI->Play_Sound(TEXT("UI_Fx_MainHud_Btn_Chat_1.mp3"), CHANNELID::SOUND_UI,
//				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}
	else
	{

	}
	
	m_bActive = bActive;
}

void CUI_Emoticon_SpeechBalloon::Set_EmoticonType(_uint iIndex)
{
	for (auto& pChildUI : m_pChild)
	{
		if (pChildUI != nullptr)
		{
			if (TEXT("UI_Emoticon_BalloonEmoticon") == pChildUI->Get_ObjectTag())
				dynamic_cast<CUI_Emoticon_BalloonEmoticon*>(pChildUI)->Set_TextureIndex(iIndex);
		}
	}
}

HRESULT CUI_Emoticon_SpeechBalloon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Emoticon_SpeechBalloon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 0.9f;

	// 자식으로 이모티콘을 가진다
	_float2 vIconSize = _float2(128.f * 0.6f, 128.f * 0.6f);
	Make_Child(0.f, -5.f, vIconSize.x, vIconSize.y, TEXT("Prototype_GameObject_UI_Emoticon_BallonEmoticon"));

	return S_OK;
}

void CUI_Emoticon_SpeechBalloon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (GI->Get_CurrentLevel() == LEVELID::LEVEL_TEST)
		{
			// 카메라와의 Distance를 계산해서 Y를 조정한다.
			// Todo(Wonhye) InGame Camera 확정되면 수정하기

			_float4 vCamPos4 = GI->Get_CamPosition();
			_vector vTempForDistance = m_pCharacterTrans->Get_Position() - XMLoadFloat4(&vCamPos4);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));
		}

		_float4 vPosition;
		XMStoreFloat4(&vPosition, m_pTransformCom->Get_Position());
		
		__super::Tick(fTimeDelta);

		Set_ChildPosition(_float3(vPosition.x, vPosition.y + 5.f, vPosition.z));
	}
}

void CUI_Emoticon_SpeechBalloon::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pCharacterTrans)
		return;

	if (m_bActive)
	{
		if (nullptr == m_pCharacterTrans)
			return;

		_float2 vPosition = Get_ProjectionPosition(m_pCharacterTrans);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(vPosition.x - g_iWinSizeX * 0.5f, -((vPosition.y - 400.f) - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (m_bAlpha)
		{
			m_fAlpha -= fTimeDelta;
			Set_ParentsAlpha(m_fAlpha);

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				Set_Active(false);
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);

		_float4 vBallonPosition;
		XMStoreFloat4(&vBallonPosition, m_pTransformCom->Get_Position());
		Set_ChildPosition(_float3(vBallonPosition.x, vBallonPosition.y + 5.f, vBallonPosition.z));
	}
}

HRESULT CUI_Emoticon_SpeechBalloon::Render()
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

HRESULT CUI_Emoticon_SpeechBalloon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_SpeechBalloon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Emoticon_SpeechBalloon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Emoticon_SpeechBalloon::Bind_ShaderResources()
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

CUI_Emoticon_SpeechBalloon* CUI_Emoticon_SpeechBalloon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Emoticon_SpeechBalloon* pInstance = new CUI_Emoticon_SpeechBalloon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Emoticon_SpeechBalloon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Emoticon_SpeechBalloon::Clone(void* pArg)
{
	CUI_Emoticon_SpeechBalloon* pInstance = new CUI_Emoticon_SpeechBalloon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Emoticon_SpeechBalloon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Emoticon_SpeechBalloon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
