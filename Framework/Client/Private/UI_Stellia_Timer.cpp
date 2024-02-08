#include "stdafx.h"
#include "UI_Stellia_Timer.h"
#include "GameInstance.h"
#include "Stellia.h"

CUI_Stellia_Timer::CUI_Stellia_Timer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Stellia_Timer")
{
}

CUI_Stellia_Timer::CUI_Stellia_Timer(const CUI_Stellia_Timer& rhs)
	: CUI(rhs)
{
}

void CUI_Stellia_Timer::Set_Timer(_float fMaxSecond)
{
	m_fMaxSecond = fMaxSecond;
	m_fCurSecond = m_fMaxSecond;

	m_bActive = true;
}

HRESULT CUI_Stellia_Timer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Stellia_Timer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

//	if (FAILED(Set_Owner()))
//		return E_FAIL;

	m_bActive = true;

	// TestCode
	m_fMaxSecond = 20.f;
	m_fCurSecond = m_fMaxSecond;
	//

	return S_OK;
}

void CUI_Stellia_Timer::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0.f <= m_fCurSecond)
		{
			if (0.f == m_fCurSecond)
				Set_Active(false);

			m_fCurSecond -= fTimeDelta;
			// 보스의 시간을 받아오는 방식이 더 좋음.
		}
		else
			Set_Active(false);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Stellia_Timer::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Stellia_Timer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Stellia_Timer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Timer"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Timer_Background"),
		TEXT("Com_Texture_Background"), (CComponent**)&m_pBackTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Stellia_Timer::Set_Owner()
{
	//	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_WITCHFOREST)
	//		return;

	CGameObject* pOwner = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, TEXT("Stellia"));
	if (nullptr == pOwner)
		return E_FAIL;
	if (nullptr == dynamic_cast<CStellia*>(pOwner))
		return E_FAIL;

	m_pStellia = dynamic_cast<CStellia*>(pOwner);
}

HRESULT CUI_Stellia_Timer::Ready_State()
{
	m_tInfo.fCX = 355.f;
	m_tInfo.fCY = 15.f;
	m_tInfo.fX = 708.5f;
	m_tInfo.fY = 759.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Stellia_Timer::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurSecond, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxSecond, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HPGaugeTexture")))
		return E_FAIL;
	if (FAILED(m_pBackTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Stellia_Timer* CUI_Stellia_Timer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Stellia_Timer* pInstance = new CUI_Stellia_Timer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Stellia_Timer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Stellia_Timer::Clone(void* pArg)
{
	CUI_Stellia_Timer* pInstance = new CUI_Stellia_Timer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Stellia_Timer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Stellia_Timer::Free()
{
	__super::Free();

	Safe_Release(m_pBackTextureCom);
	Safe_Release(m_pTextureCom);
}
