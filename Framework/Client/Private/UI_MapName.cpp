#include "stdafx.h"
#include "UI_MapName.h"
#include "GameInstance.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "UIMinigame_Manager.h"

CUI_MapName::CUI_MapName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MapName")
{
}

CUI_MapName::CUI_MapName(const CUI_MapName& rhs)
	: CUI(rhs)
{
}

void CUI_MapName::Set_Active(_bool bActive)
{

	if (bActive)
	{
		if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
			return;
		
		m_bSetAlpha = false;
		m_iTextureIndex = 0;
		m_fTimeAcc = 0.f;
		m_fAlpha = 0.1f;

//		GI->Stop_Sound(CHANNELID::SOUND_UI);
//		GI->Play_Sound(TEXT("UI_Fx_HideAndSeek_HSClearMachine_Spawn_1_St.mp3"), CHANNELID::SOUND_UI,
//			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}

	m_bActive = bActive;

}

HRESULT CUI_MapName::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MapName::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.1f;
	m_bActive = false;

	return S_OK;
}

void CUI_MapName::Tick(_float fTimeDelta)
{
	if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
		return;

	if (m_bActive)
	{
		m_fTimeAcc += fTimeDelta; // 프레임을 돌리기 위해 시간값을 누적한다.

		if (m_bSetAlpha)
		{
			if (m_fTimeAcc > 1.f)
			{
				if (m_fAlpha < 0.1f)
				{
					m_bActive = false;
					m_fAlpha = 0.f;
				}
				else
					m_fAlpha -= fTimeDelta * 0.3f;
			}
		}
		else
		{
			// MapName이 활성화 되면
			if (m_fTimeAcc > 0.1f) // 프레임을 돌린다.
			{
				if (m_iTextureIndex < 6)
					m_iTextureIndex++;

				m_fTimeAcc = 0.f;
			}
			
			if (m_fAlpha > 1.f)
				m_fAlpha = 1.f;
			else
				m_fAlpha += fTimeDelta;
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_MapName::LateTick(_float fTimeDelta)
{
	if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
		return;

	if (m_bActive)
	{
		if (6 <= m_iTextureIndex) // MapName 프레임을 전부 돌렸으면
		{
			m_iTextureIndex = 6;
			m_bSetAlpha = true; // 알파값을 줄인다.
		}

		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MapName::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MapName::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GamePlay_MapName"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MapName::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_MapName::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_MapName* CUI_MapName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MapName* pInstance = new CUI_MapName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MapName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MapName::Clone(void* pArg)
{
	CUI_MapName* pInstance = new CUI_MapName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MapName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MapName::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
