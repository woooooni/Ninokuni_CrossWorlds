#include "stdafx.h"
#include "UI_Quest_Reward.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

CUI_Quest_Reward::CUI_Quest_Reward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTREWARD eType)
	: CUI(pDevice, pContext, L"UI_Quest_Reward")
	, m_eType(eType)
{
}

CUI_Quest_Reward::CUI_Quest_Reward(const CUI_Quest_Reward& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Quest_Reward::Set_Active(_bool bActive)
{
	if (bActive)
	{
//		m_fAlpha = 1.f;
//
//		m_bAlpha = false;
//		m_fIncrement = 0.f;
//
//		if (REWARD_BOTTOM == m_eType)
//		{
//			m_bFinish = false;
//
//			m_tInfo.fX = m_vOriginPosition.x;
//			m_tInfo.fY = m_vOriginPosition.y;
//
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
//		}
//		if (REWARD_WINDOW == m_eType)
//		{
//			m_bFinish = false;
//
//			m_tInfo.fCX = m_vOriginSize.x;
//			m_tInfo.fCY = m_vOriginSize.y;
//			m_tInfo.fX = m_vOriginPosition.x;
//			m_tInfo.fY = m_vOriginPosition.y;
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
//		}

		if (REWARD_TOP == m_eType)
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Quest_Complete_Popup_1_St.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			// Effect Create
			_matrix WorldMatrix = XMMatrixIdentity();
			_float4x4 Worldfloat4x4;
			XMStoreFloat4x4(&Worldfloat4x4, WorldMatrix);
			Worldfloat4x4.m[3][0] = m_tInfo.fX;
			Worldfloat4x4.m[3][1] = m_tInfo.fY + 90.f;
			Worldfloat4x4.m[3][2] = 0.f;
			WorldMatrix = XMLoadFloat4x4(&Worldfloat4x4);
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_UI_Quest"), WorldMatrix);
		}

		m_bActive = bActive;
	}
	else
	{
		m_bActive = bActive;

		m_fAlpha = 1.f;

		m_bAlpha = false;
		m_fIncrement = 0.f;

		if (REWARD_BOTTOM == m_eType)
		{
			m_bFinish = false;

			m_tInfo.fX = m_vOriginPosition.x;
			m_tInfo.fY = m_vOriginPosition.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
		if (REWARD_WINDOW == m_eType)
		{
			m_bFinish = false;

			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;
			m_tInfo.fX = m_vOriginPosition.x;
			m_tInfo.fY = m_vOriginPosition.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
	}
}

HRESULT CUI_Quest_Reward::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Quest_Reward::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (REWARD_WINDOW == m_eType)
	{
		m_vOriginSize = _float2(m_tInfo.fCX, 0.001f);
		m_vOriginPosition = _float2(m_tInfo.fX, 218.5f + (296.f * 0.65f * 0.25f));
		m_vMaxSize  = _float2(m_tInfo.fCX, m_tInfo.fCY);
		m_vTargetPosition = _float2(m_tInfo.fX, m_tInfo.fY);

		m_tInfo.fCX = m_vOriginSize.x;
		m_tInfo.fCY = m_vOriginSize.y;
		m_tInfo.fX = m_vOriginPosition.x;
		m_tInfo.fY = m_vOriginPosition.y;
	}
	else if (REWARD_BOTTOM == m_eType)
	{
		m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vTargetPosition = _float2(m_tInfo.fX, m_tInfo.fY + (82.f * 0.65f));
	}
	else if (REWARD_TOP == m_eType)
	{
		m_vTextPosition = _float2(m_tInfo.fX - 10.f, m_tInfo.fY + 40.f);
	}

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Quest_Reward::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (REWARD_END == m_eType)
			return;

		m_fActiveTimeAcc += fTimeDelta;

		if (!m_bAlpha)
		{
			if (REWARD_WINDOW == m_eType)
			{
				m_fIncrement = fTimeDelta * m_fSpeed;

				if (false == m_bFinish)
				{
					m_fIncrement = fTimeDelta * m_fSpeed;

					if (m_tInfo.fCY >= m_vMaxSize.y)
					{
						m_bFinish = true;
						m_tInfo.fCY = m_vMaxSize.y;

						CUI_Manager::GetInstance()->Show_RewardItems();
					}
					else
					{
						m_tInfo.fCY += m_fIncrement;
						m_tInfo.fY += m_fIncrement * 0.5f;
					}
				}
			}
			else if (REWARD_BOTTOM == m_eType)
			{
				if (false == m_bFinish)
				{
					m_fIncrement = fTimeDelta * m_fSpeed;

					if (m_tInfo.fY >= m_vTargetPosition.y)
					{
						m_bFinish = true;
						m_tInfo.fY = m_vTargetPosition.y;
					}
					else
					{
						m_tInfo.fY += m_fIncrement;
					}
				}
			}

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
		else
		{
//			m_fIncrement = 0.f;
//			m_bFinish = false;
//
//			if (REWARD_WINDOW == m_eType)
//			{
//				m_tInfo.fCX = m_vOriginSize.x;
//				m_tInfo.fCY = m_vOriginSize.y;
//				m_tInfo.fX = m_vOriginPosition.x;
//				m_tInfo.fY = m_vOriginPosition.y;
//			}
//			else if (REWARD_BOTTOM == m_eType)
//			{
//				m_tInfo.fX = m_vOriginPosition.x;
//				m_tInfo.fY = m_vOriginPosition.y;
//			}
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Quest_Reward::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (REWARD_END == m_eType)
			return;

		if (REWARD_TOP == m_eType)
		{
			CRenderer::TEXT_DESC TextDesc;

			_int iLength = m_strText.length();

			TextDesc.strText = m_strText;
			TextDesc.strFontTag = L"Default_Bold";
			TextDesc.vScale = { 0.5f, 0.5f };
			TextDesc.vColor = _float4(0.478f, 0.541f, 0.549f, 1.f);
			TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 7.f, m_vTextPosition.y);

			if (0.4f <= m_fAlpha)
				m_pRendererCom->Add_Text(TextDesc);
		}

		if (3.5f < m_fActiveTimeAcc)
		{
			m_bAlpha = true;
			m_fActiveTimeAcc = 0.f;
		}

		if (m_bAlpha)
		{
			m_fAlpha -= fTimeDelta;

			if (REWARD_TOP == m_eType)
			{
				CUI_Manager::GetInstance()->Set_AlphaToItems();
			}

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				//Set_Active(false);
				CUI_Manager::GetInstance()->OnOff_QuestRewards(false);
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Quest_Reward::Render()
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

HRESULT CUI_Quest_Reward::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Quest_Finish_Popup"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Quest_Reward::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Quest_Reward::Bind_ShaderResources()
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

CUI_Quest_Reward* CUI_Quest_Reward::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTREWARD eType)
{
	CUI_Quest_Reward* pInstance = new CUI_Quest_Reward(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Quest_Reward");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Quest_Reward::Clone(void* pArg)
{
	CUI_Quest_Reward* pInstance = new CUI_Quest_Reward(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Quest_Reward");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Quest_Reward::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
