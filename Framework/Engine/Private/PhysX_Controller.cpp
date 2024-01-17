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
		m_pPhysXController = GI->Add_BoxController(pControllerDesc->pOwner, WorldMatrix, pControllerDesc->vExtents, pControllerDesc->vOffset, pControllerDesc->fMaxJumpHeight, this);
		
		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else if (m_eType == CONTROLLER_TYPE::CAPSULE)
	{
		m_pPhysXController = GI->Add_CapsuleController(pControllerDesc->pOwner, WorldMatrix, pControllerDesc->fHeight, pControllerDesc->fRaidus, pControllerDesc->vOffset, pControllerDesc->fMaxJumpHeight, this);

		if (nullptr == m_pPhysXController)
			return E_FAIL;
	}
	else
		return E_FAIL;


	

	m_FilterData.setToDefault();
	m_Filters.mFilterData = &m_FilterData;
	m_Filters.mCCTFilterCallback = CPhysX_Manager::GetInstance();

	return S_OK;
}

void CPhysX_Controller::Tick_Controller(_float fTimeDelta)
{	
	if (m_pOwner->Is_Dead() || m_pOwner->Is_ReserveDead())
		return;

	m_bGroundChecked = false;


	Vec3 vPosition = m_pTransformCom->Get_Position(); // 피직스 기준으로는 발 끝이다.
	PxExtendedVec3 vPhysPosition = m_pPhysXController->getPosition();

	Vec3 vDirCenter =  Vec3(vPhysPosition.x, vPhysPosition.y, vPhysPosition.z) - m_vPrevPosition;
	Vec3 vNewCenterPosition = vPosition + vDirCenter;

	
	PxVec3 vDisp = PxVec3(vNewCenterPosition.x, vNewCenterPosition.y, vNewCenterPosition.z) - PxVec3(m_pPhysXController->getPosition().x, m_pPhysXController->getPosition().y, m_pPhysXController->getPosition().z);

	while (false == GI->Is_PhysX_Valid()) {}
	m_pPhysXController->move(vDisp, 0.00001f, fTimeDelta, m_Filters);
		
}

void CPhysX_Controller::LateTick_Controller(_float fTimeDelta)
{
	if (m_pOwner->Is_Dead())
	{
		if (false == m_bRemoved)
		{
			GI->Remove_Controller(m_pPhysXController);
			m_pPhysXController = nullptr;
			m_bRemoved = true;
		}
		return;
	}
		


	if (m_bGroundChecked == false) 
	{
		if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_FOUND || m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_LOST;
			PHYSX_GROUND_COLLISION_INFO Info;
			Info.pCollideObject = m_pOwner;
			Info.flag = m_eGroundFlag;

			m_pOwner->Ground_Collision_Exit(Info);
		}
		else if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			m_eGroundFlag = PxPairFlag::eCONTACT_DEFAULT;
		}
	}

	if (false == Is_Active())
		return;

	PxExtendedVec3 vPhysXPosition = m_pPhysXController->getFootPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPhysXPosition.x, vPhysXPosition.y, vPhysXPosition.z, 1.f));
	m_vPrevPosition = m_pTransformCom->Get_Position();

}

void CPhysX_Controller::Set_Active(_bool bActive)
{
	m_pPhysXController->getActor()->setActorFlag(PxActorFlag::eVISUALIZATION, bActive);
	m_pPhysXController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !bActive);
	m_pPhysXController->invalidateCache();
	
	m_bActive = bActive;
}

_bool CPhysX_Controller::Is_Active()
{
	return m_bActive;
}

void CPhysX_Controller::Set_EnterLevel_Position(Vec4 vPosition)
{
	m_pPhysXController->setFootPosition(PxExtendedVec3(vPosition.x, vPosition.y, vPosition.z));
	m_vPrevPosition = Vec3(vPosition.x, vPosition.y, vPosition.z);
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

	if (false == m_bRemoved && nullptr != m_pPhysXController)
	{
		Set_Active(false);
		m_pPhysXController = nullptr;
	}
		
	/*GI->Remove_Controller(m_pPhysXController);
	m_pPhysXController = nullptr;*/
		

	Safe_Release(m_pTransformCom);
}


void CPhysX_Controller::onShapeHit(const PxControllerShapeHit& hit)
{
	if (GI->Is_Compare(hit.actor->getName(), "Building"))
	{
		Vec3 vNormal = Vec3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z);
		if (vNormal.y > 0.f)
		{
			m_bGroundChecked = true;
			if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
				PHYSX_GROUND_COLLISION_INFO Info;
				Info.pCollideObject = m_pOwner;
				Info.flag = m_eGroundFlag;

				m_pOwner->Ground_Collision_Enter(Info);
			}

			else if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
				PHYSX_GROUND_COLLISION_INFO Info;
				Info.pCollideObject = m_pOwner;
				Info.flag = m_eGroundFlag;

				m_pOwner->Ground_Collision_Continue(Info);
			}

			else if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
				PHYSX_GROUND_COLLISION_INFO Info;
				Info.pCollideObject = m_pOwner;
				Info.flag = m_eGroundFlag;

				m_pOwner->Ground_Collision_Enter(Info);
			}

			else if (m_eGroundFlag == PxPairFlag::eCONTACT_DEFAULT)
			{
				m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
				PHYSX_GROUND_COLLISION_INFO Info;
				Info.pCollideObject = m_pOwner;
				Info.flag = m_eGroundFlag;

				m_pOwner->Ground_Collision_Enter(Info);
			}
		}

	}
	else if (GI->Is_Compare(hit.actor->getName(), "Ground"))
	{
		m_bGroundChecked = true;
		if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			PHYSX_GROUND_COLLISION_INFO Info;
			Info.pCollideObject = m_pOwner;
			Info.flag = m_eGroundFlag;

			m_pOwner->Ground_Collision_Enter(Info);
		}

		else if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			PHYSX_GROUND_COLLISION_INFO Info;
			Info.pCollideObject = m_pOwner;
			Info.flag = m_eGroundFlag;

			m_pOwner->Ground_Collision_Continue(Info);
		}

		else if (m_eGroundFlag == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
			PHYSX_GROUND_COLLISION_INFO Info;
			Info.pCollideObject = m_pOwner;
			Info.flag = m_eGroundFlag;

			m_pOwner->Ground_Collision_Enter(Info);
		}

		else if(m_eGroundFlag == PxPairFlag::eCONTACT_DEFAULT)
		{
			m_eGroundFlag = PxPairFlag::eNOTIFY_TOUCH_FOUND;
			PHYSX_GROUND_COLLISION_INFO Info;
			Info.pCollideObject = m_pOwner;
			Info.flag = m_eGroundFlag;

			m_pOwner->Ground_Collision_Enter(Info);
		}
	}
}

void CPhysX_Controller::onControllerHit(const PxControllersHit& hit)
{
	
}

void CPhysX_Controller::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

