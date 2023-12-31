#include "stdafx.h"
#include "UI_Costume_ItemSlot.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Costume_ItemSlot::CUI_Costume_ItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COSTUME_SECTION eSection, UI_COSTUME_SLOT eType)
	: CUI(pDevice, pContext, L"UI_Costume_ItemSlot")
	, m_eType(eType)
	, m_eSectionType(eSection)
{
}

CUI_Costume_ItemSlot::CUI_Costume_ItemSlot(const CUI_Costume_ItemSlot& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
	, m_eSectionType(rhs.m_eSectionType)
{
}

void CUI_Costume_ItemSlot::Set_Active(_bool bActive)
{
	if (COSTUMESLOT_END == m_eType)
		return;

	if (bActive)
	{
		m_iPass = 9;
		m_fTimeAcc = 0.f;
		m_bArrived = false;
		m_bClicked = false;
		m_tInfo.fX = m_vStartPosition.x;
		m_tInfo.fY = m_vStartPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
	}
	else
	{

	}

	m_bActive = bActive;
}

HRESULT CUI_Costume_ItemSlot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Costume_ItemSlot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	switch (m_eType)
	{
	case COSTUMESLOT_FIRST:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 100.f, m_vArrivedPosition.y);
		m_iTextureIndex = 0;
		break;

	case COSTUMESLOT_SECOND:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 150.f, m_vArrivedPosition.y);
		m_iTextureIndex = 1;
		break;

	case COSTUMESLOT_THIRD:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 200.f, m_vArrivedPosition.y);
		m_iTextureIndex = 2;
		break;
	}

	m_fSpeed = 500.f;
	m_fLimit = m_tInfo.fX + (m_tInfo.fCX * 0.5f);

	m_tInfo.fX = m_vStartPosition.x;
	m_tInfo.fY = m_vStartPosition.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	Set_CharacterType(CHARACTER_TYPE::SWORD_MAN);  // TestCode

	return S_OK;
}

void CUI_Costume_ItemSlot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (COSTUMESLOT_END == m_eType)
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
		else
		{
			if (m_bClicked)
				Update_Costume(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Costume_ItemSlot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (COSTUMESLOT_END == m_eType)
			return;

		if (SWORD_MAN == m_eCurPlayerType && COSTUMESECTION_HAIRACC == m_eSectionType && COSTUMESLOT_THIRD == m_eType)
			return;

		if (m_bClicked)
			m_iPass = 7;
		else
			m_iPass = 9;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Costume_ItemSlot::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		// 움직일때는 9
		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Costume_ItemSlot::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Costume_ItemSlot::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_Costume_ItemSlot::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Costume_ItemSlot::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Swordsman_Clothes
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Swordsman_Clothes"),
		TEXT("Com_Texture1"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	// Swordsman_HairAcc
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Swordsman_HairAcc"),
		TEXT("Com_Texture2"), (CComponent**)&m_pTexCom_SMAcc)))
		return E_FAIL;

	// Engineer_Clothes
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Engineer_Clothes"),
		TEXT("Com_Texture3"), (CComponent**)&m_pTexCom_EGCostume)))
		return E_FAIL;
	// Engineer_HairAcc
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Engineer_HairAcc"),
		TEXT("Com_Texture4"), (CComponent**)&m_pTexCom_EGAcc)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Slot_Glow"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Costume_ItemSlot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Costume_ItemSlot::Bind_ShaderResources()
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

	if (m_bArrived && m_bClicked && m_iPass == 7)
	{
		if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_FXTexture")))
			return E_FAIL;
	}

	// 직업과 Section에 따른 DiffuseTexture 구분.
	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (COSTUMESECTION_CLOTH == m_eSectionType)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
				return E_FAIL;
		}
		else if (COSTUMESECTION_HAIRACC == m_eSectionType)
		{
			if (FAILED(m_pTexCom_SMAcc->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
				return E_FAIL;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		if (COSTUMESECTION_CLOTH == m_eSectionType)
		{
			if (FAILED(m_pTexCom_EGCostume->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
				return E_FAIL;
		}
		else if (COSTUMESECTION_HAIRACC == m_eSectionType)
		{
			if (FAILED(m_pTexCom_EGAcc->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
				return E_FAIL;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		break;
	}

	return S_OK;
}

void CUI_Costume_ItemSlot::Update_Costume(_float fTimeDelta)
{
	if (COSTUMESECTION_END <= m_eSectionType ||
		COSTUMESLOT_END <= m_eType)
		return;

	switch (m_eSectionType)
	{
	case COSTUMESECTION_CLOTH:
		CUI_Manager::GetInstance()->Update_CostumeModel(m_eCurPlayerType, PART_TYPE::BODY, m_eType);
		break;

	case COSTUMESECTION_HAIRACC:
		CUI_Manager::GetInstance()->Update_CostumeModel(m_eCurPlayerType, PART_TYPE::HEAD, m_eType);
		break;
	}

}

void CUI_Costume_ItemSlot::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bClicked && m_bArrived)
			CUI_Manager::GetInstance()->Update_ClothSlotState(m_eSectionType, m_eType);
		// UIManager를 통해서 m_bClicked = true; 를 행한다.
	}
}

CUI_Costume_ItemSlot* CUI_Costume_ItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COSTUME_SECTION eSection, UI_COSTUME_SLOT eType)
{
	CUI_Costume_ItemSlot* pInstance = new CUI_Costume_ItemSlot(pDevice, pContext, eSection, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Costume_ItemSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Costume_ItemSlot::Clone(void* pArg)
{
	CUI_Costume_ItemSlot* pInstance = new CUI_Costume_ItemSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Costume_ItemSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Costume_ItemSlot::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTexCom_SMAcc);
	Safe_Release(m_pTexCom_EGCostume);
	Safe_Release(m_pTexCom_EGAcc);
	Safe_Release(m_pTextureCom);
}
