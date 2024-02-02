#include "stdafx.h"
#include "UI_Minigame_Curling_Info.h"
#include "GameInstance.h"

CUI_Minigame_Curling_Info::CUI_Minigame_Curling_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
	: CUI_Minigame_Curling_Base(pDevice, pContext, wstrTag)
	, m_bPlayerType(bPlayerType)
{
}

CUI_Minigame_Curling_Info::CUI_Minigame_Curling_Info(const CUI_Minigame_Curling_Info& rhs)
	: CUI_Minigame_Curling_Base(rhs)
	, m_bPlayerType(rhs.m_bPlayerType)
{
}

HRESULT CUI_Minigame_Curling_Info::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	//for (size_t i = 0; i < (_uint)CUI_Minigame_Curling_Info::TEX_TYPE::TYPEEND; i++)
	//	m_pTextureComs[i] = nullptr;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Info::Initialize(void* pArg)
{
	CUI::UI_INFO UIDesc = {};
	{
		UIDesc.fCX = UIDesc.fCY = UIDesc.fX = UIDesc.fY = 1.f;
	}

	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Default()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_Info::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Minigame_Curling_Info::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Minigame_Curling_Info::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)CUI_Minigame_Curling_Info::TEX_TYPE::TYPEEND; i++)
	{
		if (nullptr == m_pTextureComs[i])
			continue;

		/* Bind */
		{
			memcpy(&m_tInfo, &m_tUiDescs[i], sizeof(CUI::UI_INFO));

			Ready_Transform();
			
			if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
				return E_FAIL;

			if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}

		m_iPass = 1; // PS_MAIN_ALPHA

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}
	
	return S_OK;
}

HRESULT CUI_Minigame_Curling_Info::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_bPlayerType)
	{
		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Icon_Swordsman"),
			TEXT("Com_Texture_Face"), (CComponent**)&m_pTextureComs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Info_Frame_Yellow"),
			TEXT("Com_Texture_Frame"), (CComponent**)&m_pTextureComs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME])))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Icon_Destroyer"),
			TEXT("Com_Texture_Face"), (CComponent**)&m_pTextureComs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Info_Frame_Blue"),
			TEXT("Com_Texture_Frame"), (CComponent**)&m_pTextureComs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME])))
			return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Info::Ready_Default()
{
	if (FAILED(__super::Ready_Default()))
		return E_FAIL;

	const Vec2 vFramePosDelta	= { g_vWinCenter.x * 0.8f, g_vWinCenter.y * 0.7f };
	const Vec2 vFacePosDelta	= { g_vWinCenter.x * 0.915f, g_vWinCenter.y * 0.7f };

	const Vec2 vOriginFaceSize	= { 128.f, 128.f };
	const Vec2 vFaceSizeMag		= { 0.8f, 0.8f };

	const Vec2 vOriginFrameSize = { 256.f, 128.f };
	const Vec2 vFrameSizeMag	= { 1.4f, 1.1f };

	if (m_bPlayerType) // Right 
	{
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fCX = vOriginFaceSize.x * vFaceSizeMag.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fCY = vOriginFaceSize.y * vFaceSizeMag.y;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fX	= g_vWinCenter.x + vFacePosDelta.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fY	= g_vWinCenter.y + vFacePosDelta.y;

		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fCX = vOriginFrameSize.x * vFrameSizeMag.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fCY = vOriginFrameSize.y * vFrameSizeMag.y;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fX = g_vWinCenter.x + vFramePosDelta.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fY = g_vWinCenter.y + vFramePosDelta.y;
	}
	else // Left
	{
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fCX = vOriginFaceSize.x * vFaceSizeMag.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fCY = vOriginFaceSize.y * vFaceSizeMag.y;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fX	= g_vWinCenter.x - vFacePosDelta.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FACE].fY	= g_vWinCenter.y + vFacePosDelta.y;

		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fCX = vOriginFrameSize.x * vFrameSizeMag.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fCY = vOriginFrameSize.y * vFrameSizeMag.y;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fX = g_vWinCenter.x - vFramePosDelta.x;
		m_tUiDescs[(_uint)CUI_Minigame_Curling_Info::TEX_TYPE::FRAME].fY = g_vWinCenter.y + vFramePosDelta.y;
	}

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Info::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

CUI_Minigame_Curling_Info* CUI_Minigame_Curling_Info::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
{
	CUI_Minigame_Curling_Info* pInstance = new CUI_Minigame_Curling_Info(pDevice, pContext, wstrTag, bPlayerType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Curling_Info");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Curling_Info::Clone(void* pArg)
{
	CUI_Minigame_Curling_Info* pInstance = new CUI_Minigame_Curling_Info(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Curling_Info");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Curling_Info::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)CUI_Minigame_Curling_Info::TEX_TYPE::TYPEEND; i++)
	{
		Safe_Release(m_pTextureComs[i]);
	}
}
