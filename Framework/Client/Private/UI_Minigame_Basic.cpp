#include "stdafx.h"
#include "UI_Minigame_Basic.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"
#include "TowerDefence_Manager.h"

CUI_Minigame_Basic::CUI_Minigame_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType)
	: CUI(pDevice, pContext, L"UI_Minigame_Basic")
	, m_eType(eType)
{
}

CUI_Minigame_Basic::CUI_Minigame_Basic(const CUI_Minigame_Basic& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Minigame_Basic::Set_Active(_bool bActive)
{
	if (true == bActive)
	{

	}
	else
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIW_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
		{
			m_bStart = false;
			m_bEnd = false;
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_Minigame_Basic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_TextInformation()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Basic::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIW_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
			Tick_Count(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_Basic::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIW_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END)
			LateTick_Count(fTimeDelta);

		if (TOWERDEFENCE_GOLD == m_eType)
		{
			// °ñµå ÅØ½ºÆ®
			CRenderer::TEXT_DESC TextDesc = {};
			TextDesc.strFontTag = L"Default_Bold";
			TextDesc.strText = to_wstring(CGame_Manager::GetInstance()->Get_Player()->Get_Gold());
			TextDesc.vColor = Vec4(1.f, 0.7f, 0.f, 1.f);
			TextDesc.vPosition = Vec2(m_tInfo.fX + 40.f - (TextDesc.strText.size() * 8.5f), m_tInfo.fY - 10.f);
			TextDesc.vScale = Vec2(0.45f, 0.45f);
			m_pRendererCom->Add_Text(TextDesc);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_Basic::Render()
{
	if (true == m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Minigame_Basic::On_MouseEnter(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (TOWERDEFENCE_START == m_eType)
			m_iTextureIndex = 1;
	}
}

void CUI_Minigame_Basic::On_Mouse(_float fTimeDelta)
{
	if (true == m_bActive)
	{

		__super::On_Mouse(fTimeDelta);
		if (KEY_TAP(KEY::LBTN) && m_eType == UI_MINIGAMEBASIC::TOWERDEFENCE_START)
		{
			CTowerDefence_Manager::GetInstance()->Start_Defence();
			return;
		}
	}
}

void CUI_Minigame_Basic::On_MouseExit(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		if (TOWERDEFENCE_START == m_eType)
			m_iTextureIndex = 0;

		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Minigame_Basic::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_bActive = false;

	switch (m_eType)
	{
	case UI_MINIGAMEBASIC::TOWERDEFENCE_MENU:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::TOWERDEFENCE_START:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_StartBtn"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_bUseMouse = true;
		break;

	case UI_MINIGAMEBASIC::TOWERDEFENCE_GOLD:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_TowerDefence_Gold"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_FRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_CLASSFRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_SkillFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bActive = true;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_SPECIALFRAME:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_SkillFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bActive = true;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_READY:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_THREE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 2;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_TWO:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIW_ONE:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text_Number"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 0;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_START:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 1;
		m_bStart = false;
		m_bEnd = false;
		break;

	case UI_MINIGAMEBASIC::GRANDPRIX_END:
		if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Text"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_bUseIndex = true;
		m_iTextureIndex = 2;
		m_bStart = false;
		m_bEnd = false;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Minigame_Basic::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_Basic::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (true == m_bUseIndex)
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

HRESULT CUI_Minigame_Basic::Ready_TextInformation()
{
	if (!(m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
		m_eType == GRANDPRIW_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END))
		return S_OK;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMaxSize = _float2(m_vOriginSize.x * 10.f, m_vOriginSize.y * 10.f);

	m_tInfo.fCX = m_vMaxSize.x;
	m_tInfo.fCY = m_vMaxSize.y;
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

	m_fAlpha = 0.f;

	return S_OK;
}

void CUI_Minigame_Basic::Tick_Count(_float fTimeDelta)
{
	if (true == m_bStart && false == m_bEnd)
	{
		if (m_tInfo.fCX <= m_vOriginSize.x)
			m_tInfo.fCX = m_vOriginSize.x;
		
		if (m_tInfo.fCY <= m_vOriginSize.y)
		{
			m_tInfo.fCY = m_vOriginSize.y;
			m_bEnd = true;
		}

		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;

		m_fAlpha += fTimeDelta;

		m_tInfo.fCX -= fTimeDelta * (m_vOriginSize.x * 10.f);
		m_tInfo.fCY -= fTimeDelta * (m_vOriginSize.y * 10.f);

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}
}

void CUI_Minigame_Basic::LateTick_Count(_float fTimeDelta)
{

}

CUI_Minigame_Basic* CUI_Minigame_Basic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType)
{
	CUI_Minigame_Basic* pInstance = new CUI_Minigame_Basic(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Basic::Clone(void* pArg)
{
	CUI_Minigame_Basic* pInstance = new CUI_Minigame_Basic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Basic::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
