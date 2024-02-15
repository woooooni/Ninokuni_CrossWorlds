#include "stdafx.h"
#include "UI_Btn_WorldMapIcon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Basic.h"
#include "UI_WindowWorldMap.h"

CUI_Btn_WorldMapIcon::CUI_Btn_WorldMapIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eIconType(eType)
{
}

CUI_Btn_WorldMapIcon::CUI_Btn_WorldMapIcon(const CUI_Btn_WorldMapIcon& rhs)
	: CUI(rhs)
	, m_eIconType(rhs.m_eIconType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}


void CUI_Btn_WorldMapIcon::Set_Active(_bool bActive)
{
	if (bActive)
	{
		/*
			enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_TEST, LEVEL_LOADING,
		LEVEL_LOBBY, LEVEL_EVERMORE, LEVEL_KINGDOMHALL, LEVEL_ICELAND, LEVEL_WITCHFOREST,
		LEVEL_DUMMY, LEVEL_PARTS = 98, LEVEL_TOOL = 99, LEVEL_END };
		*/

		switch (m_eIconType) // 내 아이콘 타입
		{
		case UI_WORLDMAP_ICONTYPE::ICON_EVERMORE:
			if (_uint(LEVELID::LEVEL_EVERMORE) == GI->Get_CurrentLevel())
			{
				m_pIcon->Set_WorldmapIcon();
			}
			break;

		case UI_WORLDMAP_ICONTYPE::ICON_ICELAND:
			if (_uint(LEVELID::LEVEL_ICELAND) == GI->Get_CurrentLevel())
			{
				m_pIcon->Set_WorldmapIcon();
			}
			break;

		case UI_WORLDMAP_ICONTYPE::ICON_WITCHFOREST:
			if (_uint(LEVELID::LEVEL_WITCHFOREST) == GI->Get_CurrentLevel())
			{
				m_pIcon->Set_WorldmapIcon();
			}
			break;
		}
	}
	else
	{
		if (m_bEvent)
			m_bEvent = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Btn_WorldMapIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 108.f * 0.5f;
	UIDesc.fCY = 128.f * 0.5f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	CGameObject* pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_WorldMap_Icon_PlayerPortrait"), LAYER_TYPE::LAYER_UI, &UIDesc);
	if (nullptr == pTemp)
		return E_FAIL;
	m_pIcon = dynamic_cast<CUI_Basic*>(pTemp);

	m_bUseMouse = true;

	return S_OK;
}

void CUI_Btn_WorldMapIcon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pIcon)
		{
			if (m_pIcon->Get_Active())
			{
				/*
				* 	world Map의 중점
					UIDesc.fX = 1120;
					UIDesc.fY = 550.f;
					Make_Child(-28.f, -329.5f, 170.f, 141.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_Evermore")); // 1148.f, 361.5f
					Make_Child(-446.f, -57.5f, 170.f, 152.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_IceLand")); // 674.f, 492.5f
					Make_Child(-429.f, 148.f, 170.f, 159.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_WitchForest")); // 691.f, 777.5f
				*/
				if (nullptr == m_pWorldMap)
				{
					CGameObject* pObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_UI,
						TEXT("UI_WindowWorldMap"));
					if (nullptr == pObject)
						return;
					m_pWorldMap = dynamic_cast<CUI_WindowWorldMap*>(pObject);
				}

				CUI::UI_INFO MapDesc = {};
				MapDesc = m_pWorldMap->Get_UI_Info();

				_float2 vTotalInfo = _float2(MapDesc.fX + m_tInfo.fX, MapDesc.fY + m_tInfo.fY);
				m_pIcon->Update_Position(vTotalInfo);
				m_pIcon->Tick(fTimeDelta);
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Btn_WorldMapIcon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 만약 지금 레벨이 나랑 일치하지 않다면 Icon을 Set_Active(false)해야한다. 그렇지 않으면 true
		switch (m_eIconType) // 내 아이콘 타입
		{
		case UI_WORLDMAP_ICONTYPE::ICON_EVERMORE:
			if (_uint(LEVELID::LEVEL_EVERMORE) == GI->Get_CurrentLevel() ||
				_uint(LEVELID::LEVEL_KINGDOMHALL) == GI->Get_CurrentLevel())
				m_pIcon->Set_Active(true);
			else
				m_pIcon->Set_Active(false);
			break;

		case UI_WORLDMAP_ICONTYPE::ICON_ICELAND:
			if (_uint(LEVELID::LEVEL_ICELAND) == GI->Get_CurrentLevel())
				m_pIcon->Set_Active(true);
			else
				m_pIcon->Set_Active(false);
			break;

		case UI_WORLDMAP_ICONTYPE::ICON_WITCHFOREST:
			if (_uint(LEVELID::LEVEL_WITCHFOREST) == GI->Get_CurrentLevel())
				m_pIcon->Set_Active(true);
			else
				m_pIcon->Set_Active(false);
			break;
		}

		if (nullptr != m_pIcon)
		{
			if(m_pIcon->Get_Active())
				m_pIcon->LateTick(fTimeDelta);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Btn_WorldMapIcon::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();

		if (nullptr != m_pIcon)
		{
			if (m_pIcon->Get_Active())
				m_pIcon->Render();
		}
	}

	return S_OK;
}

void CUI_Btn_WorldMapIcon::On_MouseEnter(_float fTimeDelta)
{

}

void CUI_Btn_WorldMapIcon::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_Btn_WorldMapIcon::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_Btn_WorldMapIcon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapIcon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_eIconType)
	{
	case ICON_EVERMORE:
		m_iTextureIndex = 0;
		break;

	case ICON_ICELAND:
		m_iTextureIndex = 1;
		break;

	case ICON_WITCHFOREST:
		m_iTextureIndex = 2;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Btn_WorldMapIcon::Bind_ShaderResources()
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

void CUI_Btn_WorldMapIcon::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{

	}
}

CUI_Btn_WorldMapIcon* CUI_Btn_WorldMapIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType)
{
	CUI_Btn_WorldMapIcon* pInstance = new CUI_Btn_WorldMapIcon(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Btn_WorldMapIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Btn_WorldMapIcon::Clone(void* pArg)
{
	CUI_Btn_WorldMapIcon* pInstance = new CUI_Btn_WorldMapIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Btn_WorldMapIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Btn_WorldMapIcon::Free()
{
	__super::Free();

	Safe_Release(m_pIcon);
	Safe_Release(m_pTextureCom);
}
