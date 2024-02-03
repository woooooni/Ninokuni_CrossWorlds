#include "stdafx.h"
#include "UI_Minigame_EnemyHP.h"
#include "GameInstance.h"
#include "Vehicle_Flying.h"

CUI_Minigame_EnemyHP::CUI_Minigame_EnemyHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_EnemyHP")
{
}

CUI_Minigame_EnemyHP::CUI_Minigame_EnemyHP(const CUI_Minigame_EnemyHP& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_EnemyHP::Set_Owner(CGameObject* pOwner)
{
	if (nullptr == dynamic_cast<CVehicle_Flying*>(pOwner))
		return;

	m_pOwner = dynamic_cast<CVehicle_Flying*>(pOwner);

	CVehicle_Flying::PLANE_STAT StatDesc = {};
	memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CVehicle_Flying::PLANE_STAT));

	if (false == StatDesc.bIsEnemy)
		return;

	m_fMaxHP = StatDesc.fMaxHP;
	m_fCurHP = StatDesc.fCurHP;
	m_fPreHP = m_fCurHP;
}

HRESULT CUI_Minigame_EnemyHP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_EnemyHP::Initialize(void* pArg)
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

void CUI_Minigame_EnemyHP::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * 50.f;

			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_EnemyHP::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOwner)
		{
			if (true == m_pOwner->Is_Dead() || true == m_pOwner->Is_ReserveDead())
				return;

			if (true == m_pOwner->Get_Stat().bIsEnemy)
			{
				CVehicle_Flying::PLANE_STAT StatDesc = m_pOwner->Get_Stat();
				m_fCurHP = StatDesc.fCurHP;

				CRenderer::TEXT_DESC  DefaultDesc;
				DefaultDesc.strText = L"/";
				DefaultDesc.strFontTag = L"Default_Bold";
				DefaultDesc.vScale = { 0.3f, 0.3f };
				// Outline
				DefaultDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
				DefaultDesc.vPosition = _float2(m_tInfo.fX - 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(DefaultDesc);
				DefaultDesc.vPosition = _float2(m_tInfo.fX + 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(DefaultDesc);
				DefaultDesc.vPosition = _float2(m_tInfo.fX, (m_tInfo.fY - 5.f) - 1.f);
				m_pRendererCom->Add_Text(DefaultDesc);
				DefaultDesc.vPosition = _float2(m_tInfo.fX, (m_tInfo.fY - 5.f) + 1.f);
				m_pRendererCom->Add_Text(DefaultDesc);
				// Origin
				DefaultDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
				DefaultDesc.vPosition = _float2(m_tInfo.fX, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(DefaultDesc);

				CRenderer::TEXT_DESC CurHPDesc;
				wstring strCurHP = to_wstring(_int(m_pOwner->Get_Stat().fCurHP));
				_float fOffsetX = (strCurHP.length() - 1) * 7.f;
				CurHPDesc.strText = strCurHP;
				CurHPDesc.strFontTag = L"Default_Bold";
				CurHPDesc.vScale = { 0.25f, 0.25f };
				// Outline
				CurHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
				CurHPDesc.vPosition = _float2((m_tInfo.fX - 5.f - fOffsetX) - 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(CurHPDesc);
				CurHPDesc.vPosition = _float2((m_tInfo.fX - 5.f - fOffsetX) + 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(CurHPDesc);
				CurHPDesc.vPosition = _float2((m_tInfo.fX - 5.f - fOffsetX), (m_tInfo.fY - 5.f) - 1.f);
				m_pRendererCom->Add_Text(CurHPDesc);
				CurHPDesc.vPosition = _float2((m_tInfo.fX - 5.f - fOffsetX), (m_tInfo.fY - 5.f) + 1.f);
				m_pRendererCom->Add_Text(CurHPDesc);
				// Origin
				CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
				CurHPDesc.vPosition = _float2((m_tInfo.fX - 5.f - fOffsetX), (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(CurHPDesc);
	
				CRenderer::TEXT_DESC MaxHPDesc;
				wstring strMaxHP = to_wstring(_int(m_pOwner->Get_Stat().fMaxHP));
				MaxHPDesc.strText = strMaxHP;
				MaxHPDesc.strFontTag = L"Default_Bold";
				MaxHPDesc.vScale = { 0.25f, 0.25f };
				// Outline
				MaxHPDesc.vColor = { 0.f, 0.f, 0.f, 1.f };
				MaxHPDesc.vPosition = _float2((m_tInfo.fX + 5.f) - 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(MaxHPDesc);
				MaxHPDesc.vPosition = _float2((m_tInfo.fX + 5.f) + 1.f, (m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(MaxHPDesc);
				MaxHPDesc.vPosition = _float2((m_tInfo.fX + 5.f), (m_tInfo.fY - 5.f) - 1.f);
				m_pRendererCom->Add_Text(MaxHPDesc);
				MaxHPDesc.vPosition = _float2((m_tInfo.fX + 5.f), (m_tInfo.fY - 5.f) + 1.f);
				m_pRendererCom->Add_Text(MaxHPDesc);
				// Origin
				MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
				MaxHPDesc.vPosition = _float2((m_tInfo.fX + 5.f),( m_tInfo.fY - 5.f));
				m_pRendererCom->Add_Text(MaxHPDesc);
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Minigame_EnemyHP::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Minigame_EnemyHP::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_HPBar_Lerp"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_EnemyHP::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_EnemyHP::Bind_ShaderResources()
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
	if (FAILED(m_pFXTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HPGaugeTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Minigame_EnemyHP* CUI_Minigame_EnemyHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_EnemyHP* pInstance = new CUI_Minigame_EnemyHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_EnemyHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_EnemyHP::Clone(void* pArg)
{
	CUI_Minigame_EnemyHP* pInstance = new CUI_Minigame_EnemyHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_EnemyHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_EnemyHP::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
