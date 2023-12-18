#include "stdafx.h"
#include "UI_MonsterHP_Elemental.h"
#include "GameInstance.h"

CUI_MonsterHP_Elemental::CUI_MonsterHP_Elemental(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHP_Elemental")
{
}

CUI_MonsterHP_Elemental::CUI_MonsterHP_Elemental(const CUI_MonsterHP_Elemental& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MonsterHP_Elemental::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterHP_Elemental::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_iTextureIndex = 0; // For Test

	return S_OK;
}

void CUI_MonsterHP_Elemental::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_MonsterHP_Elemental::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// Temp
		CRenderer::TEXT_DESC LevelDesc;
		LevelDesc.strText = L"Lv.20";
		LevelDesc.strFontTag = L"Default_Medium";
		LevelDesc.vScale = { 0.4f, 0.4f };
		LevelDesc.vPosition = m_vTextPosition;
		LevelDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(LevelDesc);

		// 보스는 Level이 없음.


		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MonsterHP_Elemental::Render()
{
	if (0 > m_iTextureIndex || 5 < m_iTextureIndex)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	// AddText ( Monster Level )

	return S_OK;
}

HRESULT CUI_MonsterHP_Elemental::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Elemental"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MonsterHP_Elemental::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MonsterHP_Elemental::Bind_ShaderResources()
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

CUI_MonsterHP_Elemental* CUI_MonsterHP_Elemental::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_Elemental* pInstance = new CUI_MonsterHP_Elemental(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MonsterHP_Elemental");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_Elemental::Clone(void* pArg)
{
	CUI_MonsterHP_Elemental* pInstance = new CUI_MonsterHP_Elemental(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MonsterHP_Elemental");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_Elemental::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
