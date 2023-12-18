#include "RigidBody.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "..\Public\PhysX_Controller.h"

CPhysX_Controller::CPhysX_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
	
}

CPhysX_Controller::CPhysX_Controller(CPhysX_Controller& rhs)
	: CComponent(rhs)
{

}

HRESULT CPhysX_Controller::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CPhysX_Controller::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CONTROLLER_DESC* pControllerDesc = (CONTROLLER_DESC*)pArg;
	m_eType = pControllerDesc->eType;
	m_pTransformCom = pControllerDesc->pTransform;
	Safe_AddRef(m_pTransformCom);

	if (m_eType == CONTROLLER_TYPE::BOX)
	{
		m_pPhysXController = GI->Add_BoxController(pControllerDesc->pOwner, m_pTransformCom->Get_WorldMatrix(), pControllerDesc->vExtents, pControllerDesc->fMaxJumpHeight);
		
		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else if (m_eType == CONTROLLER_TYPE::CAPSULE)
	{
		m_pPhysXController = GI->Add_CapsuleController(pControllerDesc->pOwner, m_pTransformCom->Get_WorldMatrix(), pControllerDesc->fHeight, pControllerDesc->fRaidus, pControllerDesc->fMaxJumpHeight);

		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else
		return E_FAIL;


	return S_OK;
}

void CPhysX_Controller::Tick_Controller(_float fTimeDelta)
{
	Vec3 vPosition = m_pTransformCom->Get_Position();
	m_pPhysXController->getActor()->setKinematicTarget({ vPosition.x, vPosition.y, vPosition.z });
}

void CPhysX_Controller::LateTick_Controller(_float fTimeDelta)
{

	PxTransform vPhysXPosition;
	m_pPhysXController->getActor()->getKinematicTarget(vPhysXPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPhysXPosition.p.x, vPhysXPosition.p.y, vPhysXPosition.p.z, 1.f));

}






CPhysX_Controller* CPhysX_Controller::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhysX_Controller* pInstance = new CPhysX_Controller(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Crate Failed. : CPhysX_Controller");
		return nullptr;
	}
	return pInstance;
}

CComponent* CPhysX_Controller::Clone(void* pArg)
{
	CPhysX_Controller* pInstance = new CPhysX_Controller(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CPhysX_Controller");
		return nullptr;
	}

	return pInstance;
}

void CPhysX_Controller::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
}

