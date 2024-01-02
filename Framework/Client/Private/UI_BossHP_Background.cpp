#include "stdafx.h"
#include "UI_BossHP_Background.h"
#include "GameInstance.h"
#include "Boss.h"

CUI_BossHP_Background::CUI_BossHP_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_BOSSHPBACK eType)
	: CUI(pDevice, pContext, L"UI_BossHP_Background")
	, m_eUIType(eType)
{
}

CUI_BossHP_Background::CUI_BossHP_Background(const CUI_BossHP_Background& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
{
}

void CUI_BossHP_Background::Set_Owner(CBoss* pBoss)
{
	if (BOSS_INFO != m_eUIType)
		return;
	
	if (nullptr == pBoss)
		return;

	m_pOwner = pBoss;

	CMonster::MONSTER_STAT StatDesc = {};
	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));

	memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));
	m_strName = m_pOwner->Get_KorName();

	if (TEXT("Glanix") == m_pOwner->Get_ObjectTag())
		m_iTextureIndex = 0;
	else if (TEXT("Stellia") == m_pOwner->Get_ObjectTag())
		m_iTextureIndex = 1;
	else if (TEXT("DreamerMazeWitch") == m_pOwner->Get_ObjectTag())
		m_iTextureIndex = 2;
}

HRESULT CUI_BossHP_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BossHP_Background::Initialize(void* pArg)
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

void CUI_BossHP_Background::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_BossHP_Background::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BossHP_Background::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_BossHP_Background::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eUIType)
	{
	case UI_BOSSHPBACK::BOSS_INFO:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_BossInfo"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_BOSSHPBACK::BOSS_BARFRAME:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_BossHP_Background::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BossHP_Background::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (BOSS_INFO == m_eUIType)
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

CUI_BossHP_Background* CUI_BossHP_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_BOSSHPBACK eType)
{
	CUI_BossHP_Background* pInstance = new CUI_BossHP_Background(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BossHP_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossHP_Background::Clone(void* pArg)
{
	CUI_BossHP_Background* pInstance = new CUI_BossHP_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BossHP_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossHP_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
