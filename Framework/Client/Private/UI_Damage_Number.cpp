#include "stdafx.h"
#include "UI_Damage_Number.h"
#include "GameInstance.h"
#include "UIDamage_Manager.h"
#include "UI_Manager.h"
#include "Character.h"

CUI_Damage_Number::CUI_Damage_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
	: CUI(pDevice, pContext, L"UI_Damage_Skill")
	, m_eFontType(eType)
{
}

CUI_Damage_Number::CUI_Damage_Number(const CUI_Damage_Number& rhs)
	: CUI(rhs)
	, m_eFontType(rhs.m_eFontType)
{
}

HRESULT CUI_Damage_Number::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Damage_Number::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_FontDesc, pArg, sizeof(DAMAGE_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// 데미지 넘버를 띄울 타겟의 Transform
	m_pTargetTransform = m_FontDesc.pTargetTransform;
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	// 데미지
	m_iDamage = m_FontDesc.iDamage;
	// 데미지 넘버 오프셋 포지션
	m_vOffset = m_FontDesc.vOffset; // 플레이어의 경우 0.f, 0.f가 들어옴

	// 카메라와의 거리(폰트의 사이즈를 조절하기 위함)
	_float4 vCamPos = GI->Get_CamPosition();
	_vector vTempForDistance = m_pTargetTransform->Get_Position() - XMLoadFloat4(&vCamPos);
	_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

	_float fNumSize;
	if (true == m_FontDesc.bIsPlayer)
	{
		CCharacter* pCharacter = CUI_Manager::GetInstance()->Get_Character();
		if (nullptr == pCharacter)
			return E_FAIL;
		switch (pCharacter->Get_CharacterType())
		{
		case CHARACTER_TYPE::SWORD_MAN:
			m_tInfo.fY = m_FontDesc.vTargetPosition.y;
			break;

		case CHARACTER_TYPE::ENGINEER:
			m_tInfo.fY = m_FontDesc.vTargetPosition.y - 110.f;
			break;

		case CHARACTER_TYPE::DESTROYER:
			m_tInfo.fY = m_FontDesc.vTargetPosition.y;
			break;

		default:
			m_tInfo.fY = m_FontDesc.vTargetPosition.y;
			break;
		}

		fNumSize = 112.f * 0.23f;
		m_tInfo.fX = m_FontDesc.vTargetPosition.x;
		m_vTargetPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_fArrivedPosY = m_tInfo.fY - 200.f;
	}
	else
	{
		// 타겟의 직교 좌표 -> 갱신 필요함
		m_vTargetPosition = CUI_Manager::GetInstance()->Get_ProjectionPosition(m_pTargetTransform);

		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x;
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;

		if (true == m_FontDesc.bIsBoss)
		{
			if (18.f <= fDistance)
			{
				if (12.f <= fDistance)
					m_fAlpha = 0.1f;

				_float fRandom = GI->RandomFloat(0.13f, 0.17f);
				fNumSize = 112.f * fRandom;
			}
			else
			{
				_float fRandom = GI->RandomFloat(0.19f, 0.3f);
				fNumSize = 112.f * fRandom;
			}
		}
		else
		{
			if (18.f <= fDistance)
			{
				if (22.f <= fDistance)
					m_fAlpha = 0.1f;

				_float fRandom = GI->RandomFloat(0.11f, 0.15f);
				fNumSize = 112.f * fRandom;
			}
			else
			{
				_float fRandom = GI->RandomFloat(0.15f, 0.19f);
				fNumSize = 112.f * fRandom;
			}
		}
	}

	m_pTransformCom->Set_Scale(XMVectorSet(fNumSize, fNumSize, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	m_fOffsetX = fNumSize * 0.6f;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fX = m_tInfo.fX;
	UIDesc.fY = m_tInfo.fY;
	UIDesc.fCX = fNumSize;
	UIDesc.fCY = fNumSize;
	UIDesc.pParent = nullptr;
	UIDesc.pDesc = nullptr;
	Set_UI_Info(UIDesc);

	m_bFadeOut = false;
	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_Damage_Number::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (true == m_FontDesc.bIsPlayer)
				Tick_Player(fTimeDelta);
			else
				Tick_Monster(fTimeDelta);

			__super::Tick(fTimeDelta);
		}
	}

}

void CUI_Damage_Number::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;

		if (0 == m_iDamage || 999999 < m_iDamage)
			return;

		if (true == m_FontDesc.bIsPlayer)
		{
			if (m_tInfo.fY == m_fArrivedPosY)
				Set_Dead(true);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_Number::Render()
{
	if (0 == m_iDamage || 999999 < m_iDamage)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (true == m_FontDesc.bIsPlayer)
		Render_Player();
	else
		Render_Monster();

	return S_OK;
}

void CUI_Damage_Number::Tick_Player(_float fTimeDelta)
{
	if (!m_bFadeOut)
	{
		if (m_fAlpha <= 0.f)
		{
			m_fAlpha = 0.f;
			Set_Dead(true);
		}
		else
		{
			m_fAlpha -= fTimeDelta;
		}

		if (m_tInfo.fY <= m_fArrivedPosY)
		{
			m_tInfo.fY = m_fArrivedPosY;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

			m_bFadeOut = true;
		}
		else
		{
			m_tInfo.fY -= fTimeDelta * 250.f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
	}
}

void CUI_Damage_Number::Tick_Monster(_float fTimeDelta)
{
	if (false == m_bFadeOut)
	{
		m_fFadeTimeAcc += fTimeDelta;

		if (m_fFadeTimeAcc > 0.2f)
		{
			m_bFadeOut = true;
			m_fFadeTimeAcc = 0.f;
		}
	}
	else
	{
		_float fRandom = GI->RandomFloat(2.f, 2.5f);
		m_fAlpha -= fTimeDelta * fRandom;

		if (m_fAlpha <= 0.f)
		{
			m_fAlpha = 0.f;
			Set_Dead(true);
		}
		else
		{
			m_tInfo.fY -= fTimeDelta * 1000.f;
			if (m_tInfo.fY <= m_fArrivedPosY)
			{
				m_tInfo.fY = m_fArrivedPosY;
			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
	}
}

HRESULT CUI_Damage_Number::Render_Player()
{
	if (100000 <= m_iDamage) // 6자리
	{
		m_iTextNum = (_uint)m_iDamage / 100000;
		m_tInfo.fX = m_vTargetPosition.x;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10000 <= m_iDamage) // 5자리
	{
		m_iTextNum = ((m_iDamage % 100000) / 10000);
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (1000 <= m_iDamage) // 4자리
	{
		m_iTextNum = ((m_iDamage % 10000) / 1000);
		m_tInfo.fX = m_vTargetPosition.x + (m_fOffsetX * 2.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (100 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 1000) / 100);
		m_tInfo.fX = m_vTargetPosition.x + (m_fOffsetX * 3.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 100) / 10);
		m_tInfo.fX = m_vTargetPosition.x + (m_fOffsetX * 4.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (0 < m_iDamage)
	{
		m_iTextNum = (m_iDamage % 10);
		m_tInfo.fX = m_vTargetPosition.x + (m_fOffsetX * 5.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Damage_Number::Render_Monster()
{
	// TargetPosition 갱신 -> Font가 따라간다
	m_vTargetPosition = CUI_Manager::GetInstance()->Get_ProjectionPosition(m_pTargetTransform);

	if (100000 <= m_iDamage) // 6자리
	{
		m_iTextNum = (_uint)m_iDamage / 100000;
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x;
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10000 <= m_iDamage) // 5자리
	{
		m_iTextNum = ((m_iDamage % 100000) / 10000);
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x + m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (1000 <= m_iDamage) // 4자리
	{
		m_iTextNum = ((m_iDamage % 10000) / 1000);
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x + (m_fOffsetX * 2.f);
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (100 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 1000) / 100);
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x + (m_fOffsetX * 3.f);
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 100) / 10);
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x + (m_fOffsetX * 4.f);
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (0 < m_iDamage)
	{
		m_iTextNum = (m_iDamage % 10);
		m_tInfo.fX = m_vTargetPosition.x + m_vOffset.x + (m_fOffsetX * 5.f);
		m_tInfo.fY = m_vTargetPosition.y + m_vOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Damage_Number::Resize_Scale()
{
	// Resize를 시작한다.

	m_bResize = true;

	m_tInfo.fCX = m_fMaxScale;
	m_tInfo.fCY = m_fMaxScale;
}

HRESULT CUI_Damage_Number::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eFontType)
	{
	case UI_DAMAGEFONT::BLUE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Blue"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL; // Water
		break;

	case UI_DAMAGEFONT::GOLD_WITHRED: ////////////////////////////////////////////////////
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Gold"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONT::GREEN:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Green"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL; // Wood
		break;

	case UI_DAMAGEFONT::PURPLE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Purple"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL; // Dark
		break;

	case UI_DAMAGEFONT::RED:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Red"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL; // Fire
		break;

	case UI_DAMAGEFONT::WHITE: ////////////////////////////////////////////////////
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_White"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONT::WHITEGOLD: ////////////////////////////////////////////////////
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_WhiteGold"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL; // Light
		break;

	case UI_DAMAGEFONT::GOLD: ////////////////////////////////////////////////////
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Yellow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Damage_Number::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Damage_Number::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Damage_Number* CUI_Damage_Number::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
{
	CUI_Damage_Number* pInstance = new CUI_Damage_Number(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Damage_Number");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Damage_Number::Clone(void* pArg)
{
	CUI_Damage_Number* pInstance = new CUI_Damage_Number(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Damage_Number");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage_Number::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
