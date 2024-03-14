#include "stdafx.h"
#include "UI_Setting_Section.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Section::CUI_Setting_Section(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_SETTING_SECTION eType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eType(eType)
{
}

CUI_Setting_Section::CUI_Setting_Section(const CUI_Setting_Section& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Setting_Section::Set_Active(_bool bActive)
{
	if (bActive)
	{
		Set_ChildActive(true);
	}

	m_bActive = bActive;
}

HRESULT CUI_Setting_Section::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Section::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_fAlpha = 1.f;

	if (SECTION_AUDIO == m_eType)
	{
		// AudioSection에서 슬리이더를 자식으로 가진다.
		// DefaultPos.xdp 50 -> Max Position
		_float2 vDefaultPos = _float2(-110.f, -5.f);
		_float2 vSize = _float2(30.f, 30.f);
		_float fOffset = 55.f;

		Make_Child(vDefaultPos.x, vDefaultPos.y, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_First"), TEXT("UI_Setting_Audio_FirstSlider"));
		Make_Child(vDefaultPos.x, vDefaultPos.y + fOffset, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_Second"), TEXT("UI_Setting_Audio_SecondSlider"));
		Make_Child(vDefaultPos.x, vDefaultPos.y + (fOffset * 1.8f), vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_Third"), TEXT("UI_Setting_Audio_ThirdSlider"));

		// Minus Btn
		_float2 vMinusPos = _float2(-142.f, -3.f);
		Make_Child(vMinusPos.x, vMinusPos.y, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Minus"), TEXT("UI_Setting_Audio_FirstMinusBtn"));
		_float2 vPlusPos = _float2(72.f, -3.f);
		Make_Child(vPlusPos.x, vPlusPos.y, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Plus"), TEXT("UI_Setting_Audio_FirstPlusBtn"));

		Make_Child(vMinusPos.x, vMinusPos.y + fOffset, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Minus"), TEXT("UI_Setting_Audio_SecondMinusBtn"));
		Make_Child(vPlusPos.x, vPlusPos.y + fOffset, vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Plus"), TEXT("UI_Setting_Audio_SecondPlusBtn"));

		Make_Child(vMinusPos.x, vMinusPos.y + (fOffset * 1.8f), vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Minus"), TEXT("UI_Setting_Audio_ThirdMinusBtn"));
		Make_Child(vPlusPos.x, vPlusPos.y + (fOffset * 1.8f), vSize.x, vSize.y,
			TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Plus"), TEXT("UI_Setting_Audio_ThirdPlusBtn"));
	}

	m_bUseMouse = true;

	return S_OK;
}

void CUI_Setting_Section::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Section::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Setting_Section::Render()
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

HRESULT CUI_Setting_Section::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Section"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Section::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Section::Bind_ShaderResources()
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

CUI_Setting_Section* CUI_Setting_Section::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_SETTING_SECTION eType)
{
	CUI_Setting_Section* pInstance = new CUI_Setting_Section(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Section");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Section::Clone(void* pArg)
{
	CUI_Setting_Section* pInstance = new CUI_Setting_Section(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Section");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Section::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
