#include "stdafx.h"
#include "UI_Grandprix_Popup.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UIMInigame_Manager.h"

CUI_Grandprix_Popup::CUI_Grandprix_Popup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_POPUP eType)
	: CUI(pDevice, pContext, L"UI_Grandprix_Popup")
	, m_eType(eType)
{
}

CUI_Grandprix_Popup::CUI_Grandprix_Popup(const CUI_Grandprix_Popup& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Grandprix_Popup::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		m_fTimeAcc = 0.f;
		m_fAlpha = 1.f;
		m_bDisappear = false;

		// Sound
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Result_Item_RandomEffect_Finish_1_St.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI)); 
	}

	m_bActive = bActive;
}

void CUI_Grandprix_Popup::Set_TextureIndex(_uint iIndex)
{
	if (POPUP_MAIN == m_eType)
	{
		//인덱스 세팅이 필요한 타입이라면 세팅해주고
		m_iTextureIndex = iIndex;
	}

	// Active한다.
	Set_Active(true);
}

HRESULT CUI_Grandprix_Popup::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_Popup::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_Popup::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (POPUP_BACKGROUND == m_eType)
		{
			// 회전시킨다
			m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 0.f, 1.f, 0.f),fTimeDelta);
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_Popup::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (false == m_bDisappear)
		{
			m_fTimeAcc += fTimeDelta;

			if (1.f < m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				m_bDisappear = true;
			}
		}
		else
		{
			m_fAlpha -= fTimeDelta;
			if (0.f >= m_fAlpha)
			{
				m_fAlpha = 0.f;
				Set_Active(false);
			}
		}
		//if (POPUP_MAIN == m_eType && 3 == m_iTextureIndex)
		//{
		//	// 시간이 느려지는 아이템은 차별을 둔다.
		//	m_fAlpha -= fTimeDelta * 10.f;
		//}
		//else

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Grandprix_Popup::Render()
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

HRESULT CUI_Grandprix_Popup::Ready_Components()
{
	if (m_eType == UI_GRANDPRIX_POPUP::POPUP_END)
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_GRANDPRIX_POPUP::POPUP_BACKGROUND:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_ItemPopup_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_GRANDPRIX_POPUP::POPUP_MAIN:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_ItemPopup"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	default:
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Grandprix_Popup::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_Popup::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (POPUP_MAIN == m_eType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

CUI_Grandprix_Popup* CUI_Grandprix_Popup::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_GRANDPRIX_POPUP eType)
{
	CUI_Grandprix_Popup* pInstance = new CUI_Grandprix_Popup(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_Popup");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_Popup::Clone(void* pArg)
{
	CUI_Grandprix_Popup* pInstance = new CUI_Grandprix_Popup(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_Popup");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_Popup::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
