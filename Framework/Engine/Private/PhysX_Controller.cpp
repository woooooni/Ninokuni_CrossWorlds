#include "RigidBody.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "PhysX_Manager.h"

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

	m_vPrevPosition = m_pTransformCom->Get_Position();

	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix.r[CTransform::STATE_POSITION] += pControllerDesc->vOffset;

	if (m_eType == CONTROLLER_TYPE::BOX)
	{
		m_pPhysXController = GI->Add_BoxController(pControllerDesc->pOwner, WorldMatrix, pControllerDesc->vExtents, pControllerDesc->fMaxJumpHeight);
		
		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else if (m_eType == CONTROLLER_TYPE::CAPSULE)
	{
		m_pPhysXController = GI->Add_CapsuleController(pControllerDesc->pOwner, WorldMatrix, pControllerDesc->fHeight, pControllerDesc->fRaidus, pControllerDesc->fMaxJumpHeight);

		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else
		return E_FAIL;


	m_Filters.mFilterData = &m_FilterData;
	m_Filters.mCCTFilterCallback = this;

	

	return S_OK;
}

void CPhysX_Controller::Tick_Controller(_float fTimeDelta)
{		
	Vec3 vPosition = m_pTransformCom->Get_Position(); // 피직스 기준으로는 발 끝이다.
	PxExtendedVec3 vPhysPosition = m_pPhysXController->getPosition();

	Vec3 vDirCenter =  Vec3(vPhysPosition.x, vPhysPosition.y, vPhysPosition.z) - m_vPrevPosition;
	Vec3 vNewCenterPosition = vPosition + vDirCenter;

	
	PxVec3 vDisp = PxVec3(vNewCenterPosition.x, vNewCenterPosition.y, vNewCenterPosition.z) - PxVec3(m_pPhysXController->getPosition().x, m_pPhysXController->getPosition().y, m_pPhysXController->getPosition().z);
	m_pPhysXController->move(vDisp, 0.00001f, fTimeDelta, m_Filters);
}

void CPhysX_Controller::LateTick_Controller(_float fTimeDelta)
{		
	PxExtendedVec3 vPhysXPosition = m_pPhysXController->getFootPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPhysXPosition.x, vPhysXPosition.y, vPhysXPosition.z, 1.f));
	m_vPrevPosition = m_pTransformCom->Get_Position();
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

PxQueryHitType::Enum CPhysX_Controller::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	return PxQueryHitType::Enum();
}

PxQueryHitType::Enum CPhysX_Controller::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	return PxQueryHitType::Enum();
}

bool CPhysX_Controller::filter(const PxController& a, const PxController& b)
{
	return false;
}


