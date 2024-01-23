#include "stdafx.h"
#include "UI_Minigame_EnemyInfo.h"
#include "GameInstance.h"

CUI_Minigame_EnemyInfo::CUI_Minigame_EnemyInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_EnemyInfo")
{
}

CUI_Minigame_EnemyInfo::CUI_Minigame_EnemyInfo(const CUI_Minigame_EnemyInfo& rhs)
	: CUI(rhs)
{
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

	return S_OK;
}

void CUI_Minigame_EnemyInfo::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		_int iOffsetX = 0;
//		if (3 < m_strName.length())
//		{
//			iOffsetX = (m_strName.length() - 3.f) * 6.f;
//		}
//
//		CRenderer::TEXT_DESC MaxHPDesc;
//		MaxHPDesc.strText = m_strName;
//		MaxHPDesc.strFontTag = L"Default_Bold";
//		MaxHPDesc.vScale = { 0.4f, 0.4f };
//		MaxHPDesc.vColor = m_vOutlineColor;
//		// Outline
//		MaxHPDesc.vPosition = _float2(m_vNamePosition.x - iOffsetX - 1.f, m_vNamePosition.y);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vNamePosition.x - iOffsetX + 1.f, m_vNamePosition.y);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vNamePosition.x - iOffsetX, m_vNamePosition.y - 1.f);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vNamePosition.x - iOffsetX, m_vNamePosition.y + 1.f);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		// Font
//		MaxHPDesc.vColor = m_vFontColor;
//		MaxHPDesc.vPosition = _float2((m_vNamePosition.x - iOffsetX), m_vNamePosition.y);
//		m_pRendererCom->Add_Text(MaxHPDesc);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_EnemyInfo::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (8 < m_iTextureIndex)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_EnemyInfo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

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

	Safe_Release(m_pTextureCom);
}
