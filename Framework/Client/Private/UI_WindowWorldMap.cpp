#include "stdafx.h"
#include "UI_WindowWorldMap.h"
#include "GameInstance.h"

CUI_WindowWorldMap::CUI_WindowWorldMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_WindowWorldMap")
{
}

CUI_WindowWorldMap::CUI_WindowWorldMap(const CUI_WindowWorldMap& rhs)
	: CUI(rhs)
{
}

void CUI_WindowWorldMap::Set_Active(_bool bActive)
{
	if (bActive)
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}
	}
	else
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_WindowWorldMap::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WindowWorldMap::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	/*
	world Map의 중점
	UIDesc.fX = 1120;
	UIDesc.fY = 550.f;
	*/
	// ChildUI를 추가한다
	Make_Child(-28.f, -329.5f, 170.f, 141.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_Evermore")); // 1148.f, 361.5f
	Make_Child(-446.f, -57.5f, 170.f, 152.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_IceLand")); // 674.f, 492.5f
	Make_Child(-429.f, 148.f, 170.f, 159.f, TEXT("Prototype_GameObject_UI_WorldMap_Btn_WitchForest")); // 691.f, 777.5f

	// m_vOffsetMin
	// m_vOffsetMax

	m_bActive = false;

	return S_OK;
}

void CUI_WindowWorldMap::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		if (m_bMove)
			m_fTimeAcc += fTimeDelta;

		if (0.1f < m_fTimeAcc)
		{
			m_bMove = false;
			m_fTimeAcc = 0.f;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_WindowWorldMap::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_WindowWorldMap::Render()
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

void CUI_WindowWorldMap::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_WindowWorldMap::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_Mouse(fTimeDelta);

		if (KEY_HOLD(KEY::LBTN))
		{

		}
	}
}

void CUI_WindowWorldMap::On_MouseExit(_float fTimeDelta)
{
}

void CUI_WindowWorldMap::On_MouseDragEnter(_float fTimeDelta)
{
}

void CUI_WindowWorldMap::On_MouseDrag(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_tInfo.fX += GI->Get_DIMMoveState(DIMM::DIMM_X);
			m_tInfo.fY += GI->Get_DIMMoveState(DIMM::DIMM_Y);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		if (!m_bMove)
		{
			if (m_tInfo.fX > (m_tInfo.fCX * 0.5f + 100.f))
			{
				m_tInfo.fX = m_tInfo.fCX * 0.5f;
				m_bMove = true;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}

			if (m_tInfo.fX < 490.f)
			{
				m_tInfo.fX = 490.f;
				m_bMove = true;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}
		}
	}
}

void CUI_WindowWorldMap::On_MouseDragExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 일정 범위를 넘어가면 정해진 위치로 이동한다.
//		_float fMinX = 0.f;
//		_float fMaxX = 0.f;
	}
}

HRESULT CUI_WindowWorldMap::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapBackground"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_WindowWorldMap::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WindowWorldMap::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_WindowWorldMap* CUI_WindowWorldMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WindowWorldMap* pInstance = new CUI_WindowWorldMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WindowWorldMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WindowWorldMap::Clone(void* pArg)
{
	CUI_WindowWorldMap* pInstance = new CUI_WindowWorldMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WindowWorldMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WindowWorldMap::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
