#include "stdafx.h"
#include "UI_Minigame_EnemyInfo.h"
#include "GameInstance.h"
#include "UI_Minigame_EnemyHP.h"

CUI_Minigame_EnemyInfo::CUI_Minigame_EnemyInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_EnemyInfo")
{
}

CUI_Minigame_EnemyInfo::CUI_Minigame_EnemyInfo(const CUI_Minigame_EnemyInfo& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_EnemyInfo::Set_TextureIndex(_uint iIndex)
{
	if (8 < iIndex)
		return;

	m_vOriginPos = _float2(m_tInfo.fX, m_tInfo.fY);
	m_fSpeed = 800.f;

	_float fSizeX = m_tInfo.fCX * 0.5f;
	_float fOffsetX = fSizeX * 0.5f;

	m_vStartPos = _float2(m_tInfo.fX - fSizeX - (fOffsetX * iIndex), m_tInfo.fY);
	m_iTextureIndex = iIndex;
}

void CUI_Minigame_EnemyInfo::Set_Owner(CGameObject* pOwner)
{
	m_pOwner = pOwner;

	// m_pHP에도 Owner의 정보를 전달한다.
	if (nullptr != m_pHP)
		m_pHP->Set_Owner(pOwner);
}

void CUI_Minigame_EnemyInfo::Set_Active(_bool bActive)
{
	if (true == bActive)
	{
		m_bArrived = false;
		m_tInfo.fX = m_vStartPos.x;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
	else
	{

	}

	m_bActive = bActive;
}

HRESULT CUI_Minigame_EnemyInfo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_EnemyInfo::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	CUI::UI_INFO UIDesc = {};
	UIDesc.fCX = 102.f;
	UIDesc.fCY =16.f;
	UIDesc.fX = m_tInfo.fX + 30.f;
	UIDesc.fY = m_tInfo.fY + 19.f;
	CGameObject* pBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBar"), _uint(LAYER_TYPE::LAYER_UI), &UIDesc);
	if (nullptr == pBar)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_EnemyHP*>(pBar))
		return E_FAIL;
	m_pHP = dynamic_cast<CUI_Minigame_EnemyHP*>(pBar);

	return S_OK;
}

void CUI_Minigame_EnemyInfo::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (false == m_bArrived)
		{
			m_tInfo.fX += fTimeDelta * m_fSpeed;

			if (m_vOriginPos.x <= m_tInfo.fX)
			{
				m_tInfo.fX = m_vOriginPos.x;
				m_bArrived = true;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else
		{
			if (nullptr != m_pHP)
				m_pHP->Tick(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_EnemyInfo::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (8 < m_iTextureIndex)
			return;

		if (true == m_bArrived)
		{
			if (nullptr != m_pHP)
				m_pHP->LateTick(fTimeDelta);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_EnemyInfo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	if (true == m_bArrived)
	{
		if (nullptr != m_pHP)
			m_pHP->Render();
	}

	return S_OK;
}

HRESULT CUI_Minigame_EnemyInfo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar_Backs"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_EnemyInfo::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_EnemyInfo::Bind_ShaderResources()
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

CUI_Minigame_EnemyInfo* CUI_Minigame_EnemyInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_EnemyInfo* pInstance = new CUI_Minigame_EnemyInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_EnemyInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_EnemyInfo::Clone(void* pArg)
{
	CUI_Minigame_EnemyInfo* pInstance = new CUI_Minigame_EnemyInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_EnemyInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_EnemyInfo::Free()
{
	__super::Free();

	Safe_Release(m_pHP);
	Safe_Release(m_pTextureCom);
}
