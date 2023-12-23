#include "stdafx.h"
#include "Camera_Free.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Move(fTimeDelta);
	Rotate(fTimeDelta);
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_Components()
{
	return S_OK;
}

void CCamera_Free::Move(_float fTimeDelta)
{
	if (GI->Mouse_Pressing(DIMK_WHEEL))
	{
		if (KEY_HOLD(KEY::W))
		{
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fMoveSpeed, fTimeDelta);
		}

		if (KEY_HOLD(KEY::S))
		{
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), -m_fMoveSpeed, fTimeDelta);
		}

		if (KEY_HOLD(KEY::A))
		{
			m_pTransformCom->Move(m_pTransformCom->Get_Right(), -m_fMoveSpeed, fTimeDelta);
		}

		if (KEY_HOLD(KEY::D))
		{
			m_pTransformCom->Move(m_pTransformCom->Get_Right(), m_fMoveSpeed, fTimeDelta);
		}

		if (KEY_HOLD(KEY::Q))
		{
			m_pTransformCom->Move(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMoveSpeed, fTimeDelta);
		}

		if (KEY_HOLD(KEY::E))
		{
			m_pTransformCom->Move(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fMoveSpeed, fTimeDelta);
		}
	}
}

void CCamera_Free::Rotate(_float fTimeDelta)
{
	if (GI->Mouse_Pressing(DIMK_WHEEL))
	{
		_long	MouseMove = 0;

		if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * m_fRotateSpeed, fTimeDelta);
		}

		if (MouseMove = GI->Get_DIMMoveState(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * m_fRotateSpeed, fTimeDelta);
		}
	}
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
