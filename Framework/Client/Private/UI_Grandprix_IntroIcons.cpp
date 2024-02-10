#include "stdafx.h"
#include "UI_Grandprix_IntroIcons.h"
#include "GameInstance.h"


CUI_Grandprix_IntroIcons::CUI_Grandprix_IntroIcons(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, GRANDPRIX_INTRO eType)
	: CUI(pDevice, pContext, L"UI_Grandprix_IntroIcons")
	, m_eType(eType)
{
}

CUI_Grandprix_IntroIcons::CUI_Grandprix_IntroIcons(const CUI_Grandprix_IntroIcons& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Grandprix_IntroIcons::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		if (m_eType == GRANDPRIX_INTRO::VS_ICON)
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_VS_1_St.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
		}
	}
	else
	{
		if (m_eType == GRANDPRIX_INTRO::SWORDMAN)
		{
			m_tInfo.fX = m_vStart.x;
			m_tInfo.fY = m_vStart.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else if (m_eType == GRANDPRIX_INTRO::ENGINEER)
		{
			m_tInfo.fX = m_vStart.x;
			m_tInfo.fY = m_vStart.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else if (m_eType == GRANDPRIX_INTRO::SPLITTER)
			m_vStart.y = 0.f;

		m_bFinished = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Grandprix_IntroIcons::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_IntroIcons::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	switch (m_eType)
	{
	case GRANDPRIX_INTRO::SWORDMAN:
		m_vStart = { -0.5f * g_iWinSizeX, m_tInfo.fY };
		m_vGoal = { m_tInfo.fX, m_tInfo.fY };

		m_tInfo.fX = m_vStart.x;
		m_tInfo.fY = m_vStart.y;

		m_fSpeed = 900.f;
		break;

	case GRANDPRIX_INTRO::ENGINEER:
		m_vStart = { g_iWinSizeX * 1.5f, m_tInfo.fY };
		m_vGoal = { m_tInfo.fX, m_tInfo.fY };

		m_tInfo.fX = m_vStart.x;
		m_tInfo.fY = m_vStart.y;

		m_fSpeed = 1000.f;
		break;

	case GRANDPRIX_INTRO::SPLITTER:
		m_vStart = { m_tInfo.fX, 0.f };
		m_vGoal = { m_tInfo.fX, g_iWinSizeY };
		// 셰이더로 던질 y값을 산정하기 위한 목적. 움직임이나 스케일 변화 없음.

		m_fSpeed = 500.f;
		m_iPass = 12;
		break;

	case GRANDPRIX_INTRO::VS_ICON:
		m_vStart = { m_tInfo.fCX, m_tInfo.fCY };
		m_vGoal = { m_tInfo.fCX * 1.5f, m_tInfo.fCY * 1.5f };

		m_fSpeed = 300.f;
		break;

	default:
		break;
	}

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_IntroIcons::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == INTRO_END)
			return;
		
		if (GRANDPRIX_INTRO::SWORDMAN == m_eType || GRANDPRIX_INTRO::ENGINEER == m_eType || GRANDPRIX_INTRO::SPLITTER == m_eType)
			Update_Position(fTimeDelta);
		else if (GRANDPRIX_INTRO::VS_ICON)
			Update_Scale(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_IntroIcons::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == INTRO_END)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Grandprix_IntroIcons::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Grandprix_IntroIcons::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_IntroIcons"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_IntroIcons::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_IntroIcons::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_eType)))
		return E_FAIL;

	if (12 == m_iPass && m_eType == SPLITTER)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LimitY", &m_vStart.y, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_Grandprix_IntroIcons::Update_Position(_float fTimeDelta)
{
	if (GRANDPRIX_INTRO::SWORDMAN == m_eType)
	{
		if (false == m_bFinished)
		{
			m_tInfo.fX += fTimeDelta * m_fSpeed;

			if (m_vGoal.x <= m_tInfo.fX)
			{
				m_bFinished = true;
				m_tInfo.fX = m_vGoal.x;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
	}
	else if (GRANDPRIX_INTRO::ENGINEER == m_eType)
	{
		if (false == m_bFinished)
		{
			m_tInfo.fX -= fTimeDelta * m_fSpeed;

			if (m_vGoal.x >= m_tInfo.fX)
			{
				m_bFinished = true;
				m_tInfo.fX = m_vGoal.x;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
	}
	else
	{
		if (false == m_bFinished)
		{
			m_vStart.y += fTimeDelta * m_fSpeed;

			if (m_vStart.y >= m_vGoal.y)
			{
				m_bFinished = true;
				m_vStart.y = m_vGoal.y;
			}
		}
	}
}

void CUI_Grandprix_IntroIcons::Update_Scale(_float fTimeDelta)
{
	if (false == m_bFinished)
	{
		m_tInfo.fCX += fTimeDelta * m_fSpeed;
		m_tInfo.fCY += fTimeDelta * m_fSpeed;

		if (m_tInfo.fCX >= m_vGoal.x)
		{
			m_bFinished = true;
			m_tInfo.fCX = m_vGoal.x;
			m_tInfo.fCY = m_vGoal.y;
		}

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}
	else
	{
		m_tInfo.fCX -= fTimeDelta * m_fSpeed;
		m_tInfo.fCY -= fTimeDelta * m_fSpeed;

		if (m_tInfo.fCX <= m_vStart.x)
		{
			m_bFinished = false;
			m_tInfo.fCX = m_vStart.x;
			m_tInfo.fCY = m_vStart.y;
		}

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}
}

CUI_Grandprix_IntroIcons * CUI_Grandprix_IntroIcons::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, GRANDPRIX_INTRO eType)
{
	CUI_Grandprix_IntroIcons*	pInstance = new CUI_Grandprix_IntroIcons(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Grandprix_IntroIcons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Grandprix_IntroIcons::Clone(void* pArg)
{
	CUI_Grandprix_IntroIcons*	pInstance = new CUI_Grandprix_IntroIcons(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Grandprix_IntroIcons");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_IntroIcons::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
