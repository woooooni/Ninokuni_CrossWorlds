#include "stdafx.h"
#include "UI_Quest_Reward_Item.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Quest_Reward_Item::CUI_Quest_Reward_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Quest_Reward_Item")
{

}

CUI_Quest_Reward_Item::CUI_Quest_Reward_Item(const CUI_Quest_Reward_Item& rhs)
	: CUI(rhs)
	,m_eType(rhs.m_eType)
{
}

void CUI_Quest_Reward_Item::Set_Active(_bool bActive)
{
	if (false == bActive)
	{
		m_fAlpha = 1.f;
		//m_bSetInt = false;
		//m_iRandomInt = 0;
		//m_fRandomTimeAcc = 0.f;

		m_bActive = bActive;
	}
	else
	{
		if (false == m_bUsable)
			return;

		m_bActive = bActive;
	}
}

void CUI_Quest_Reward_Item::Set_Position(_int iTotal, _int iOrder)
{
	if (1 > iTotal || 4 < iTotal)
		return;

	// iTotal = 아이템의 총 개수(1부터 시작), iOrder = 아이템의 순서(0부터 시작)
	_float fSize = 64.f * 0.5f;
	_float fOffset = 5.f;
	m_tInfo.fY = 285.f;

	switch (iTotal)
	{
	case 1:
		if (0 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f;
		}
		break;

	case 2:
		if (0 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f - (fSize * 0.5f + fOffset);
		}
		else if (1 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f + (fSize * 0.5f + fOffset);
		}
		break;

	case 3:
		if (0 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f - (fSize + fOffset);
		}
		else if (1 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f;
		}
		else if (2 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f + (fSize + fOffset);
		}
		break;

	case 4:
		if (0 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f - ((fSize * 0.5f + fOffset) * 2.f);
		}
		else if (1 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f - (fSize * 0.5f + fOffset);
		}
		else if (2 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f + (fSize * 0.5f + fOffset);
		}
		else if (3 == iOrder)
		{
			m_tInfo.fX = g_iWinSizeX * 0.5f + ((fSize * 0.5f + fOffset) * 2.f);
		}
		break;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	m_vTextPosition = _float2(m_tInfo.fX, m_tInfo.fY + 16.f);
}

HRESULT CUI_Quest_Reward_Item::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Quest_Reward_Item::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	m_vTextPosition = _float2(m_tInfo.fX, m_tInfo.fY + 16.f);

	return S_OK;
}

void CUI_Quest_Reward_Item::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		//m_fRandomTimeAcc += fTimeDelta;

		__super::Tick(fTimeDelta);
	}
}

void CUI_Quest_Reward_Item::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0.4f <= m_fAlpha)
		{
			if (0 >= m_iAmount)
				return;

			if (false == m_bIsEnding)
			{
				m_strText = to_wstring(m_iAmount);
				_int iLength = m_strText.length();

				CRenderer::TEXT_DESC TextDesc;
				TextDesc.strText = m_strText;
				TextDesc.strFontTag = L"Default_Bold";
				TextDesc.vScale = { 0.25f, 0.25f };
				TextDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
				TextDesc.vPosition = _float2((m_vTextPosition.x - iLength * 3.5f) + 1.f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2((m_vTextPosition.x - iLength * 3.5f) - 1.f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y - 1.f);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y + 1.f);
				m_pRendererCom->Add_Text(TextDesc);

				TextDesc.vColor = _float4(0.478f, 0.541f, 0.549f, 1.f);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
			}
			else
			{
				m_strText = TEXT("1,000,000,000,000");
				_int iLength = m_strText.length() - 3; // 쉼표 4개 -> 3을 빼서 오프셋 조정함

				CRenderer::TEXT_DESC TextDesc;
				TextDesc.strText = m_strText;
				TextDesc.strFontTag = L"Default_Bold";
				TextDesc.vScale = { 0.25f, 0.25f };
				TextDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
				TextDesc.vPosition = _float2((m_vTextPosition.x - iLength * 3.5f) + 1.f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2((m_vTextPosition.x - iLength * 3.5f) - 1.f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y - 1.f);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y + 1.f);
				m_pRendererCom->Add_Text(TextDesc);

				TextDesc.vColor = _float4(0.792f, 0.843f, 0.851f, 1.f);
				TextDesc.vPosition = _float2(m_vTextPosition.x - iLength * 3.5f, m_vTextPosition.y);
				m_pRendererCom->Add_Text(TextDesc);
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Quest_Reward_Item::Render()
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

void CUI_Quest_Reward_Item::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_Quest_Reward_Item::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Quest_Reward_Item::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Quest_Reward_Item::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Quest_Rewards_Icons"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Quest_Reward_Item::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Quest_Reward_Item::Bind_ShaderResources()
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

CUI_Quest_Reward_Item* CUI_Quest_Reward_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Quest_Reward_Item* pInstance = new CUI_Quest_Reward_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Quest_Reward_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Quest_Reward_Item::Clone(void* pArg)
{
	CUI_Quest_Reward_Item* pInstance = new CUI_Quest_Reward_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Quest_Reward_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Quest_Reward_Item::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
