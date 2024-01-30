#include "stdafx.h"
#include "..\Public\CurlingGame_Arrow.h"

#include "GameInstance.h"

CCurlingGame_Arrow::CCurlingGame_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_Arrow::CCurlingGame_Arrow(const CCurlingGame_Arrow& rhs)
	: CCurlingGame_Prop(rhs)
{
}

HRESULT CCurlingGame_Arrow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Arrow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Type */
	m_eCGType = CG_TYPE::CG_ARROW;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
	{
		m_tLerpScale.vCurVec = m_vStartScale;

		m_pTransformCom->Set_Scale(m_tLerpScale.vCurVec);
	}

	return S_OK;
}

void CCurlingGame_Arrow::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (m_tLerpScale.bActive)
	{
		m_tLerpScale.Update_Lerp(fTimeDelta);
		m_pTransformCom->Set_Scale(m_tLerpScale.vCurVec);

		if (!m_tLerpScale.bActive)
		{
			if (m_bIncrease)
			{
				m_tLerpScale.Start(m_vStartScale, m_vTargetScale, m_fLerpTime, LERP_MODE::SMOOTHER_STEP);
			}
			else
			{
				m_tLerpScale.Start(m_vTargetScale, m_vStartScale, m_fLerpTime, LERP_MODE::SMOOTHER_STEP);
			}

			m_bIncrease = !m_bIncrease;
		}
	}
}

void CCurlingGame_Arrow::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

void CCurlingGame_Arrow::Start_Action()
{
	m_tLerpScale.Start(m_vStartScale, m_vTargetScale, m_fLerpTime, LERP_MODE::SMOOTHER_STEP);
	m_bIncrease = true;
}

void CCurlingGame_Arrow::Tick_Action()
{
}

HRESULT CCurlingGame_Arrow::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;
	}

	/* For. Com_Model */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Prop_Arrow"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CCurlingGame_Arrow::Ready_Colliders()
{
	return S_OK;
}

CCurlingGame_Arrow* CCurlingGame_Arrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCurlingGame_Arrow* pInstance = new CCurlingGame_Arrow(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCurlingGame_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCurlingGame_Arrow::Clone(void* pArg)
{
	CCurlingGame_Arrow* pInstance = new CCurlingGame_Arrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCurlingGame_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurlingGame_Arrow::Free()
{
	__super::Free();
}
