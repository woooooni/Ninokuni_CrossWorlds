#include "stdafx.h"
#include "UI_Damage_Skill.h"
#include "GameInstance.h"

CUI_Damage_Skill::CUI_Damage_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
	: CUI(pDevice, pContext, L"UI_Damage_Skill")
	, m_eFontType(eType)
{
}

CUI_Damage_Skill::CUI_Damage_Skill(const CUI_Damage_Skill& rhs)
	: CUI(rhs)
	, m_eFontType(rhs.m_eFontType)
{
}

HRESULT CUI_Damage_Skill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Damage_Skill::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

//	CUI::UI_INFO UI
// ;
//	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
//	UIDesc.fX = g_iWinSizeX * 0.5f;
//	UIDesc.fY = g_iWinSizeY * 0.5f;
//	UIDesc.fCX = 112.f;
//	UIDesc.fCY = 112.f;
//	UIDesc.pParent = nullptr;
//	UIDesc.pDesc = nullptr;
//	Set_UI_Info(UIDesc);

	memcpy(&m_FontDesc, pArg, sizeof(DAMAGE_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = m_FontDesc.pTargetTransform;
	m_iDamage = m_FontDesc.iDamage;
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	m_vTargetPosition = Get_ProjectionPosition(m_pTargetTransform);

	m_tInfo.fX = m_vTargetPosition.x;
	m_tInfo.fY = m_vTargetPosition.y;
	_float fNumSize = 112.f * 0.3f;
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
	m_bSetPosition = false;

	return S_OK;
}

void CUI_Damage_Skill::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (!m_bSetPosition)
			{
				m_bSetPosition = true;
				m_fRandomOffset = _float2(GI->RandomFloat(-100.f, 100.f), GI->RandomFloat(-200.f, 0.f));

				m_tInfo.fX += m_fRandomOffset.x;
				m_tInfo.fY += m_fRandomOffset.y;

				m_vTargetPosition.x = m_tInfo.fX;
				m_vTargetPosition.y = m_tInfo.fY;

				m_fArrivedPosY = m_vTargetPosition.y - 100.f;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}

			if (!m_bFadeOut)
			{
				// FadeOut상태가 아니라면 시간을 누적한다.
				m_fFadeTimeAcc += fTimeDelta;

				if (m_fFadeTimeAcc > 0.3f)
				{
					m_bFadeOut = true;
					m_fFadeTimeAcc = 0.f;
				}
			}
			else // FadeOut 상태라면.
			{ // Y값 조정 안됨
				m_tInfo.fY -= fTimeDelta * 200.f;
				m_fAlpha -= fTimeDelta;

				if (m_fAlpha <= 0.f)
				{
					m_fAlpha = 0.f;
					Set_Dead(true);
				}

				if (m_tInfo.fY <= m_fArrivedPosY)
				{
					m_tInfo.fY = m_fArrivedPosY;
				}

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}

			__super::Tick(fTimeDelta);
		}
	}

}

void CUI_Damage_Skill::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;


		if (0 == m_iDamage || 999999 < m_iDamage)
			return;

		if (m_tInfo.fY == m_fArrivedPosY)
			Set_Dead(true);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_Skill::Render()
{
	if (0 == m_iDamage || 999999 < m_iDamage) // Damage변수를 가지고 숫자가 총 몇자리인지 파악하여 Render할 것이다.
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	// 큰 수부터 if문으로 걸러낸다.
	if (100000 <= m_iDamage) // 6 자리
	{
		m_iTextNum = (_uint)m_iDamage / 100000; // 제일 첫번째 숫자가 나온다.

		m_tInfo.fX = m_vTargetPosition.x;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
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
		//(m_iDamage % 100000 계산을 통해서 6자리 이상인 경우에는 첫번째 숫자를 버린다.
		//m_tInfo.fX = m_tInfo.fX + m_fOffsetX; // Offset을 줘서 다음 숫자를 그릴 위치를 지정한다. // 1210.f - m_fOffsetX;
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		//	XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY + g_iWinSizeY) * 0.5f, 0.f, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
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
		//m_tInfo.fX = m_tInfo.fX + m_fOffsetX; // 1230.f - m_fOffsetX;
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 2.f; // Offset을 줘서 다음 숫자를 그릴 위치를 지정한다. // 1210.f - m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (100 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 1000) / 100); // 3번째 자리를 구한다.
		//m_tInfo.fX = m_tInfo.fX + m_fOffsetX; // 1250.f - m_fOffsetX;
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 3.f; // Offset을 줘서 다음 숫자를 그릴 위치를 지정한다. // 1210.f - m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 100) / 10); ;// 2번째 자리를 구한다.
		//m_tInfo.fX = m_tInfo.fX + m_fOffsetX; //1270.f - m_fOffsetX;
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 4.f; // Offset을 줘서 다음 숫자를 그릴 위치를 지정한다. // 1210.f - m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
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
		//m_tInfo.fX = m_tInfo.fX + m_fOffsetX; //1290.f - m_fOffsetX;
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 5.f; // Offset을 줘서 다음 숫자를 그릴 위치를 지정한다. // 1210.f - m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// 셰이더에 바인딩한다.
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CUI_Damage_Skill::Resize_Scale()
{
	// Resize를 시작한다.

	m_bResize = true;

	m_tInfo.fCX = m_fMaxScale;
	m_tInfo.fCY = m_fMaxScale;
}

HRESULT CUI_Damage_Skill::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// 	enum UI_DAMAGEFONT { BLUE, GOLD_WITHRED, RED, WHITE, WHITEYELLOW, GOLD, DAMAGEFOND_END };
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

	case UI_DAMAGEFONT::WHITE: //////////////////////////////////////////////////// General
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

HRESULT CUI_Damage_Skill::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Damage_Skill::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

//	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
//		return E_FAIL;

	return S_OK;
}

CUI_Damage_Skill* CUI_Damage_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
{
	CUI_Damage_Skill* pInstance = new CUI_Damage_Skill(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Damage_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Damage_Skill::Clone(void* pArg)
{
	CUI_Damage_Skill* pInstance = new CUI_Damage_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Damage_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage_Skill::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
