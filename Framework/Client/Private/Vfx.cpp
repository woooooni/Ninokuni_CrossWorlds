#include "stdafx.h"
#include "Vfx.h"
#include "GameInstance.h"

CVfx::CVfx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
{
}

CVfx::CVfx(const CVfx& rhs)
	: CGameObject(rhs)
	, m_bOwnerStateIndex(rhs.m_bOwnerStateIndex)
{
}

HRESULT CVfx::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx::Tick(_float fTimeDelta)
{
	if (m_bFinish || m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");
	CModel* pModel          = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");

	if (pMachine == nullptr || pModel == nullptr || pMachine->Get_CurrState() != m_bOwnerStateIndex)
	{
		Set_Dead(true);
		return;
	}

	if (m_bOwnerTween) // false == TweenFinish
		m_bOwnerTween = pModel->Is_Tween();
	else
		m_iOwnerFrame = pModel->Get_CurrAnimationFrame();
}

void CVfx::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx::Render()
{
	return S_OK;
}

HRESULT CVfx::Ready_Components()
{
	return S_OK;
}

void CVfx::Free()
{
	__super::Free();
}