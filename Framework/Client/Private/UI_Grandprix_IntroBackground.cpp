#include "stdafx.h"
#include "UI_Grandprix_IntroBackground.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CUI_Grandprix_IntroBackground::CUI_Grandprix_IntroBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Grandprix_IntroBackground")
{
}

CUI_Grandprix_IntroBackground::CUI_Grandprix_IntroBackground(const CUI_Grandprix_IntroBackground& rhs)
	: CUI(rhs)
{
}

void CUI_Grandprix_IntroBackground::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
//		CUI_Manager::GetInstance()->OnOff_TextUI(false);

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
	else
	{
//		CUI_Manager::GetInstance()->OnOff_TextUI(true);

		CPlayer* pPlayer = nullptr;
		pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;

		CCharacter* pCharacter = nullptr;
		pCharacter = pPlayer->Get_Character();
		if (nullptr == pCharacter)
			return;

		pCharacter->Set_Skill_Input(true);
		pCharacter->Set_Attack_Input(true);
		pCharacter->Set_Move_Input(true);
	}

	m_bActive = bActive;
}

HRESULT CUI_Grandprix_IntroBackground::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_IntroBackground::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_IntroBackground::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		
		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_IntroBackground::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

//		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Grandprix_IntroBackground::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

//		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_Grandprix_IntroBackground::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_IntroBackground"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_IntroBackground::Ready_State()
{
	m_tInfo.fCX = g_iWinSizeX;
	m_tInfo.fCY = g_iWinSizeY;

	m_tInfo.fX = g_iWinSizeX * 0.5f;
	m_tInfo.fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_IntroBackground::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Grandprix_IntroBackground * CUI_Grandprix_IntroBackground::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Grandprix_IntroBackground*	pInstance = new CUI_Grandprix_IntroBackground(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Grandprix_IntroBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Grandprix_IntroBackground::Clone(void* pArg)
{
	CUI_Grandprix_IntroBackground*	pInstance = new CUI_Grandprix_IntroBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Grandprix_IntroBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_IntroBackground::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
