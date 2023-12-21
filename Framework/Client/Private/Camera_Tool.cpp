#include "stdafx.h"
#include "Camera_Tool.h"
#include "GameInstance.h"
#include "Key_Manager.h"

CCamera_Tool::CCamera_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Tool::CCamera_Tool(const CCamera_Tool& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Tool::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Tool::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	return S_OK;
}

void CCamera_Tool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

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

		if (KEY_TAP(KEY::R))
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));
			m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));
		}


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

	__super::LateTick(fTimeDelta);

}

void CCamera_Tool::LateTick(_float fTimeDelta)
{
	// __super::LateTick(fTimeDelta);
}

HRESULT CCamera_Tool::Render()
{
	return S_OK;
}

HRESULT CCamera_Tool::Ready_Components()
{
	__super::Ready_Components();


	if (FAILED(__super::Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CCamera_Tool* CCamera_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Tool* pInstance = new CCamera_Tool(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Tool::Clone(void* pArg)
{
	CCamera::CAMERADESC* pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Tool* pInstance = new CCamera_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Tool::Free()
{
	__super::Free();


}
