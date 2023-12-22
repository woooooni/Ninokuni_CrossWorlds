#include "stdafx.h"
#include "UI_SkillSection_Frame.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_Frame::CUI_SkillSection_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SKILLFRAME_TYPE eType, UI_SPECIALSKILL_TYPE eNumType)
	: CUI(pDevice, pContext, L"UI_SkillSection_Frame")
	, m_eFrameType(eType)
{
	if (FRAME_SPECIAL == m_eFrameType)
		m_eNumType = eNumType;
}

CUI_SkillSection_Frame::CUI_SkillSection_Frame(const CUI_SkillSection_Frame& rhs)
	: CUI(rhs)
	, m_eFrameType(rhs.m_eFrameType)
	, m_eNumType(rhs.m_eNumType)
{
}

void CUI_SkillSection_Frame::Set_ClassicFrameColor(ELEMENTAL_TYPE eType)
{
	if (FRAME_CLASSIC != m_eFrameType)
		return;

	if (ELEMENTAL_END == eType)
		return;

	switch (eType)
	{
	case FIRE:
		m_vColor = _float4(0.914f, 0.2f, 0.192f, 1.f);
		break;

	case WATER:
		m_vColor = _float4(0.192f, 0.694f, 0.996f, 1.f);
		break;

	case WOOD:
		m_vColor = _float4(0.294f, 0.965f, 0.263f, 1.f);
		break;

	case LIGHT:
		m_vColor = _float4(0.965f, 0.718f, 0.071f, 1.f);
		break;

	case DARK:
		m_vColor = _float4(0.612f, 0.22f, 1.f, 1.f);
		break;
	}
}

void CUI_SkillSection_Frame::Set_SpecialFrameColor(CHARACTER_TYPE eType)
{
	if (FRAME_SPECIAL != m_eFrameType)
		return;

	switch (eType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (UI_SPECIALSKILL_TYPE::FRAME_FIRST == m_eNumType)
			m_vColor = _float4(0.612f, 0.22f, 1.f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_SECOND == m_eNumType)
			m_vColor = _float4(0.192f, 0.694f, 0.996f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_THIRD == m_eNumType)
			m_vColor = _float4(0.294f, 0.965f, 0.263f, 1.f);
		break;

	case CHARACTER_TYPE::DESTROYER:
		if (UI_SPECIALSKILL_TYPE::FRAME_FIRST == m_eNumType)
			m_vColor = _float4(0.612f, 0.22f, 1.f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_SECOND == m_eNumType)
			m_vColor = _float4(0.914f, 0.2f, 0.192f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_THIRD == m_eNumType)
			m_vColor = _float4(0.294f, 0.965f, 0.263f, 1.f);
		break;

	case CHARACTER_TYPE::ENGINEER:
		if (UI_SPECIALSKILL_TYPE::FRAME_FIRST == m_eNumType)
			m_vColor = _float4(0.965f, 0.718f, 0.071f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_SECOND == m_eNumType)
			m_vColor = _float4(0.965f, 0.718f, 0.071f, 1.f);
		else if (UI_SPECIALSKILL_TYPE::FRAME_THIRD == m_eNumType)
			m_vColor = _float4(0.294f, 0.965f, 0.263f, 1.f);
		break;
	}
}

HRESULT CUI_SkillSection_Frame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_Frame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true; 

	return S_OK;
}

void CUI_SkillSection_Frame::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_vColor.x == 1.f && m_vColor.y == 1.f && m_vColor.z == 1.f && m_vColor.w == 1.f)
			return;

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_Frame::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_vColor.x == 1.f && m_vColor.y == 1.f && m_vColor.z == 1.f && m_vColor.w == 1.f)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		//__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_SkillSection_Frame::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(10);

		m_pVIBufferCom->Render();

		//__super::Render();
	}

	return S_OK;
}

HRESULT CUI_SkillSection_Frame::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eFrameType)
	{
	case FRAME_CLASSIC:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSeection_Classic_ElementalFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case FRAME_SPECIAL:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSeection_Special_ElementalFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_SkillSection_Frame::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_Frame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Diffusecolor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_SkillSection_Frame* CUI_SkillSection_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SKILLFRAME_TYPE eType, UI_SPECIALSKILL_TYPE eNumType)
{
	CUI_SkillSection_Frame* pInstance = new CUI_SkillSection_Frame(pDevice, pContext, eType, eNumType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_Frame::Clone(void* pArg)
{
	CUI_SkillSection_Frame* pInstance = new CUI_SkillSection_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_Frame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
