#include "stdafx.h"
#include "UI_MonsterHP_Bar.h"
#include "GameInstance.h"
#include "Monster.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_MonsterHP_Bar::CUI_MonsterHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_MonsterHPBar")
{
}

CUI_MonsterHP_Bar::CUI_MonsterHP_Bar(const CUI_MonsterHP_Bar& rhs)
	: CUI(rhs)
{
}

void CUI_MonsterHP_Bar::Set_MonsterInfo(CMonster* pOwner)
{
	if (nullptr == pOwner)
		return;

	m_pTarget = pOwner;

	CMonster::MONSTER_STAT StatDesc = {};
	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));

	memcpy(&StatDesc, &(m_pTarget->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

	m_eElementalType = StatDesc.eElementType;
	m_fMaxHP = StatDesc.fMaxHp;
	m_fCurHP = StatDesc.fHp;
	m_fPreHP = m_fCurHP;
}

HRESULT CUI_MonsterHP_Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	m_bLerp = false;

	return S_OK;
}

void CUI_MonsterHP_Bar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (ELEMENTAL_TYPE::BASIC <= m_eElementalType)
			return;
		
		if (m_pTarget->Is_Dead() || m_pTarget->Is_ReserveDead() || m_fCurHP <= 0.f)
		{
			m_pTarget = nullptr;
			CUI_Manager::GetInstance()->OnOff_MonsterHP(false);
			return;
		}

		// 타겟과 일정거리 이상 멀어지면 끈다
		if (nullptr != m_pTarget)
		{
			CPlayer* pPlayer = nullptr;
			pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;

			CCharacter* pCharacter = nullptr;
			pCharacter = pPlayer->Get_Character();
			if (nullptr == pCharacter)
				return;

			CTransform* pPlayerTransform = pCharacter->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pPlayerTransform)
				return;
			CTransform* pTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");

			_vector vTemp = (pTransform->Get_Position()) - (pPlayerTransform->Get_Position());
			_float fTotarget = XMVectorGetX(XMVector3Length(vTemp));

			if (fTotarget > 10.f)
			{
				m_pTarget = nullptr;
				CUI_Manager::GetInstance()->OnOff_MonsterHP(false);
				return;
			}
		}

		m_fCurHP = m_pTarget->Get_Stat().fHp;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{

			m_fPreHP -= fTimeDelta * ((m_pTarget->Get_Stat().fMaxHp) / 2.f);
		
			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_MonsterHP_Bar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (ELEMENTAL_TYPE::BASIC <= m_eElementalType)
			return;

		if (m_pTarget->Is_Dead() || m_pTarget->Is_ReserveDead())
		{
			m_pTarget = nullptr;
			CUI_Manager::GetInstance()->OnOff_MonsterHP(false);
			return;
		}

		_float fX = g_iWinSizeX * 0.5f + 5.f;
		// 기준점
		Engine::CRenderer::TEXT_DESC  MAXHPDesc;
		MAXHPDesc.strText = L"/";
		MAXHPDesc.strFontTag = L"Default_Medium";
		MAXHPDesc.vScale = { 0.4f, 0.4f };
		MAXHPDesc.vColor = m_vOutlineColor;
		// Outline
		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x - 1.f, m_vDefaultPosition.y);
		m_pRendererCom->Add_Text(MAXHPDesc);
		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x + 1.f, m_vDefaultPosition.y);
		m_pRendererCom->Add_Text(MAXHPDesc);
		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x, m_vDefaultPosition.y - 1.f);
		m_pRendererCom->Add_Text(MAXHPDesc);
		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x, m_vDefaultPosition.y + 1.f);
		m_pRendererCom->Add_Text(MAXHPDesc);
		// Font
		MAXHPDesc.vColor = m_vFontColor;
		MAXHPDesc.vPosition = m_vDefaultPosition;
		m_pRendererCom->Add_Text(MAXHPDesc);

		Engine::CRenderer::TEXT_DESC CurHPDesc;
		wstring strCurTemp = to_wstring(_int(m_fCurHP));
		_int iLength = (strCurTemp.length() - 1.f) * 10.f;
		CurHPDesc.strText = strCurTemp;
		CurHPDesc.strFontTag = L"Default_Medium";
		CurHPDesc.vScale = { 0.35f, 0.35f };
		CurHPDesc.vColor = m_vOutlineColor;
		// Outline
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - 1.f - iLength, m_vCurHPPosition.y);
		m_pRendererCom->Add_Text(CurHPDesc);
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x + 1.f - iLength, m_vCurHPPosition.y);
		m_pRendererCom->Add_Text(CurHPDesc);
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - iLength, m_vCurHPPosition.y - 1.f);
		m_pRendererCom->Add_Text(CurHPDesc);
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - iLength, m_vCurHPPosition.y + 1.f);
		m_pRendererCom->Add_Text(CurHPDesc);
		// Font
		CurHPDesc.vColor = m_vFontColor;
		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - iLength, m_vCurHPPosition.y);
		m_pRendererCom->Add_Text(CurHPDesc);

		Engine::CRenderer::TEXT_DESC MaxHPDesc;
		wstring strMaxTemp = to_wstring(_int(m_fMaxHP));
		iLength = strMaxTemp.length() - 1.f;
		MaxHPDesc.strText = strMaxTemp;
		MaxHPDesc.strFontTag = L"Default_Medium";
		MaxHPDesc.vScale = { 0.35f, 0.35f };
		MaxHPDesc.vColor = m_vOutlineColor;
		// Outline
		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x - 1.f, m_vMaxHPPosition.y);
		m_pRendererCom->Add_Text(MaxHPDesc);
		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x + 1.f, m_vMaxHPPosition.y);
		m_pRendererCom->Add_Text(MaxHPDesc);
		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x, m_vMaxHPPosition.y - 1.f);
		m_pRendererCom->Add_Text(MaxHPDesc);
		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x, m_vMaxHPPosition.y + 1.f);
		m_pRendererCom->Add_Text(MaxHPDesc);
		// Font
		MaxHPDesc.vColor = m_vFontColor;
		MaxHPDesc.vPosition = m_vMaxHPPosition;
		m_pRendererCom->Add_Text(MaxHPDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_MonsterHP_Bar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_WhiteBar"),
		TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Bar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_MonsterHP_Bar::Bind_ShaderResources()
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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eElementalType))))
		return E_FAIL;

	return S_OK;
}

CUI_MonsterHP_Bar* CUI_MonsterHP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MonsterHP_Bar* pInstance = new CUI_MonsterHP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_MonsterHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MonsterHP_Bar::Clone(void* pArg)
{
	CUI_MonsterHP_Bar* pInstance = new CUI_MonsterHP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_MonsterHP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MonsterHP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pFXTextureCom);
	Safe_Release(m_pTextureCom);
}
