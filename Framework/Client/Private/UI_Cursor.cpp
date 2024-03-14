#include "stdafx.h"
#include "UI_Cursor.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"

CUI_Cursor::CUI_Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Cursor")
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Cursor::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Cursor::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_ptOffset = { 14, 13 };
	m_bUseMouse = true;

	return S_OK;
}

void CUI_Cursor::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// 마우스 포지션을 실시간으로 받아서 m_tInfo.fX, m_tInfo.fY에 저장한다.
		m_ptMouse = GI->GetMousePos();

		m_tInfo.fX = m_ptMouse.x + m_ptOffset.x;
		m_tInfo.fY = m_ptMouse.y + m_ptOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (LEVELID::LEVEL_LOBBY == GI->Get_CurrentLevel())
		{
			if (CUI_Manager::GetInstance()->Is_NicknameSettingComplete())
			{
				if (m_iTextureIndex == 1)
					m_iTextureIndex = 0;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Cursor::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_CURSOR, this);
	}
}

HRESULT CUI_Cursor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Cursor::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Cursor::On_Mouse(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		_matrix WorldMatrix = XMMatrixIdentity();
		_float4x4 Worldfloat4x4;
		XMStoreFloat4x4(&Worldfloat4x4, WorldMatrix);
		Worldfloat4x4.m[3][0] = m_ptMouse.x;
		Worldfloat4x4.m[3][1] = m_ptMouse.y;
		Worldfloat4x4.m[3][2] = 0.f;
		WorldMatrix = XMLoadFloat4x4(&Worldfloat4x4);
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_MouseClick"), WorldMatrix);
	}
}

void CUI_Cursor::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Cursor::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Cursor"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Cursor::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Cursor::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_Cursor* CUI_Cursor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Cursor* pInstance = new CUI_Cursor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Cursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Cursor::Clone(void* pArg)
{
	CUI_Cursor* pInstance = new CUI_Cursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Cursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Cursor::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
