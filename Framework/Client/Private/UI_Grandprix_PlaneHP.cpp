#include "stdafx.h"
#include "UI_Grandprix_PlaneHP.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Vehicle_Flying.h"

CUI_Grandprix_PlaneHP::CUI_Grandprix_PlaneHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Grandprix_PlaneHP")
{
}

CUI_Grandprix_PlaneHP::CUI_Grandprix_PlaneHP(const CUI_Grandprix_PlaneHP& rhs)
	: CUI(rhs)
{
}

void CUI_Grandprix_PlaneHP::Set_Owner(CVehicle_Flying* pOwner)
{
	m_pOwner = pOwner;

	CVehicle_Flying::PLANE_STAT StatDesc = {};
	StatDesc = m_pOwner->Get_Stat();

	m_fMaxHP = StatDesc.fMaxHP;
	m_fPreHP = StatDesc.fCurHP;
	m_fCurHP = m_fPreHP;
}

HRESULT CUI_Grandprix_PlaneHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_PlaneHP::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bLerp = false;
	m_bActive = true;

	return S_OK;
}

void CUI_Grandprix_PlaneHP::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner)
			return;
		
		m_fTimeAcc += fTimeDelta * 0.1f;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 500.f;

			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_PlaneHP::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner)
			return;

		CVehicle_Flying::PLANE_STAT StatDesc = m_pOwner->Get_Stat();
		m_fCurHP = StatDesc.fCurHP;

		CRenderer::TEXT_DESC  DefaultDesc;
		DefaultDesc.strText = L"/";
		DefaultDesc.strFontTag = L"Default_Bold";
		DefaultDesc.vScale = { 0.35f, 0.35f };
		DefaultDesc.vPosition = m_vDefaultPosition;
		DefaultDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(DefaultDesc);

		CRenderer::TEXT_DESC CurHPDesc;
		wstring strCurHP = to_wstring(_int(StatDesc.fCurHP));
		_float fOffsetX = (strCurHP.length() - 1) * 8.f;

		CurHPDesc.strText = strCurHP;
		CurHPDesc.strFontTag = L"Default_Bold";
		CurHPDesc.vScale = { 0.35f, 0.35f };
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - fOffsetX, m_vCurHPPosition.y);
		CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(CurHPDesc);

		CRenderer::TEXT_DESC MaxHPDesc;
		wstring strMaxHP = to_wstring(_int(StatDesc.fMaxHP));

		MaxHPDesc.strText = strMaxHP;
		MaxHPDesc.strFontTag = L"Default_Bold";
		MaxHPDesc.vScale = { 0.35f, 0.35f };
		MaxHPDesc.vPosition = m_vMaxHPPosition;
		MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MaxHPDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Grandprix_PlaneHP::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Grandprix_PlaneHP::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar_Back"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Grandprix_PlaneHP::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_PlaneHP::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_LerpHP", &m_fPreHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_LerpTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Grandprix_PlaneHP* CUI_Grandprix_PlaneHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Grandprix_PlaneHP* pInstance = new CUI_Grandprix_PlaneHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_PlaneHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_PlaneHP::Clone(void* pArg)
{
	CUI_Grandprix_PlaneHP* pInstance = new CUI_Grandprix_PlaneHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_PlaneHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_PlaneHP::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
