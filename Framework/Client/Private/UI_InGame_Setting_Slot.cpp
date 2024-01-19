#include "stdafx.h"
#include "UI_InGame_Setting_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Character.h"

CUI_InGame_Setting_Slot::CUI_InGame_Setting_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Slot")
	, m_eType(eType)
	, m_eSectionType(eSection)
{
}

CUI_InGame_Setting_Slot::CUI_InGame_Setting_Slot(const CUI_InGame_Setting_Slot& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
	, m_eSectionType(rhs.m_eSectionType)
{
}

void CUI_InGame_Setting_Slot::Set_Active(_bool bActive)
{
	if (SLOTORDER_END == m_eType)
		return;

	if (bActive)
	{
		//m_iPass = 9;
		m_fTimeAcc = 0.f;
		m_bArrived = false;
		m_bClicked = false;
		m_tInfo.fX = m_vStartPosition.x;
		m_tInfo.fY = m_vStartPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		Set_ChildActive(true);
	}
	else
	{
		if (m_bEvent)
			m_bEvent = false;

		Set_ChildActive(false);
	}

	m_bActive = bActive;
}

HRESULT CUI_InGame_Setting_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Ready_RadioGroup();

	m_bActive = false;

	switch (m_eType)
	{
	case SLOT_FIRST:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 100.f, m_vArrivedPosition.y);
		break;

	case SLOT_SECOND:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 150.f, m_vArrivedPosition.y);
		break;

	case SLOT_THIRD:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 200.f, m_vArrivedPosition.y);
		break;

	case SLOT_FOURTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 250.f, m_vArrivedPosition.y);
		break;

	case SLOT_FIFTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 300.f, m_vArrivedPosition.y);
		break;

	case SLOT_SIXTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 350.f, m_vArrivedPosition.y);
		break;
	}

	m_fSpeed = 500.f;
	m_fLimit = m_tInfo.fX + (m_tInfo.fCX * 0.5f);

	m_tInfo.fX = m_vStartPosition.x;
	m_tInfo.fY = m_vStartPosition.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

void CUI_InGame_Setting_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SLOTORDER_END == m_eType)
			return;

		if (!m_bArrived)
		{
			m_tInfo.fX -= fTimeDelta * m_fSpeed;

			if (m_tInfo.fX < m_vArrivedPosition.x)
			{
				m_bArrived = true;
				m_tInfo.fX = m_vArrivedPosition.x;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SLOTORDER_END == m_eType)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_Slot::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(9);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Ready_Components()
{

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Graphic_Slots"),
		TEXT("Com_Texture1"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Camera_Slots"),
		TEXT("Com_Texture2"), (CComponent**)&m_pTextureCamera)))
		return E_FAIL;

//	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Graphic_Slots"),
//		TEXT("Com_Texture3"), (CComponent**)&m_pTextureCom)))
//		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (!m_bArrived)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LimitX", &m_fLimit, sizeof(_float))))
			return E_FAIL;
	}

	switch(m_eSectionType)
	{
	case UI_SETTING_SECTION::SETTING_GRAPHIC:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
		break;

	case UI_SETTING_SECTION::SETTING_CAMERA:
		if (FAILED(m_pTextureCamera->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
		break;

	case UI_SETTING_SECTION::SETTING_AUDIO:
		break;
	}

	return S_OK;
}

void CUI_InGame_Setting_Slot::Ready_RadioGroup()
{
	if (m_eSectionType == SETTINGSECTION_END)
		return;

	if (m_eType == SLOTORDER_END)
		return;

	switch (m_eSectionType)
	{
	case UI_SETTING_SECTION::SETTING_GRAPHIC:
		if (m_eType != SLOT_THIRD)
			return;

//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Natural"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Shadow"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Outline"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Bloom"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Blur"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_SSAO"));
//		Make_Child(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_PBR"));
		break;

	case UI_SETTING_SECTION::SETTING_CAMERA:
		if (m_eType != SLOT_THIRD)
			return;

		break;

	case UI_SETTING_SECTION::SETTING_AUDIO:
		break;
	}
}


CUI_InGame_Setting_Slot* CUI_InGame_Setting_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType)
{
	CUI_InGame_Setting_Slot* pInstance = new CUI_InGame_Setting_Slot(pDevice, pContext, eSection, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Slot::Clone(void* pArg)
{
	CUI_InGame_Setting_Slot* pInstance = new CUI_InGame_Setting_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureAudio);
	Safe_Release(m_pTextureCamera);
	Safe_Release(m_pTextureCom);
}
