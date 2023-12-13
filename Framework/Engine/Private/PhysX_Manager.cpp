#include "PhysX_Manager.h"
#include "GameInstance.h"

#include "GameObject.h"
#include "Mesh.h"

//PxFilterFlags FilterShader(
//	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
//	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	// 1: 피직스
//	// 2: 플레이어 OBB
//	// 9: 머리카락
//	// 10: 머리카락 컷 박스 
//	
//	if (
//		// 플레이어 OBB와 피직스 물체 충돌 X
//		(1 == filterData0.word0 && 2 == filterData1.word0)
//		|| (2 == filterData0.word0 && 1 == filterData1.word0)
//		|| (2 == filterData0.word0 && 10 == filterData1.word0)
//		|| (10 == filterData0.word0 && 2 == filterData1.word0))
//	{
//		// 플레이어 박스 && 상자 박스
//		// 피직스 호출 X
//		// pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//
//	// 9 머리, 10 머리 컷 박스
//	if (9 == filterData0.word0 || 10 == filterData1.word0 || 10 == filterData0.word0 || 9 == filterData1.word0)
//	{
//		if ((9 == filterData0.word0 && 10 == filterData1.word0) || (10 == filterData0.word0 && 9 == filterData1.word0)
//			|| (9 == filterData0.word0 && 9 == filterData1.word0))
//			pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
//	}
//	else
//	{
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
//	}
//
//
//	return PxFilterFlag::eDEFAULT;
//}


IMPLEMENT_SINGLETON(CPhysX_Manager)


CPhysX_Manager::CPhysX_Manager()
{

}

HRESULT CPhysX_Manager::Reserve_Manager()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_pPvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(m_strIPAddress.c_str(), m_iPortNumber, m_iTimeOutSeconds);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_pPvd);
	PxInitExtensions(*m_Physics, m_pPvd);
	transport->release();



	PxSceneDesc SceneDesc(m_Physics->getTolerancesScale());
	SceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	m_Dispatcher = PxDefaultCpuDispatcherCreate(4);
	if (!m_Dispatcher)
		return E_FAIL;

	SceneDesc.wakeCounterResetValue = 0;
	SceneDesc.cpuDispatcher = m_Dispatcher;
	SceneDesc.filterShader = PxDefaultSimulationFilterShader;
	SceneDesc.simulationEventCallback = this;
	SceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.solverType = PxSolverType::eTGS;
	m_pScene = m_Physics->createScene(SceneDesc);



	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		//I have a PVD client, so set some flags that it needs
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_WorldMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);

	return S_OK;
}

void CPhysX_Manager::Tick(_float fTimeDelta)
{
	


}

void CPhysX_Manager::LateTick(_float fTimeDelta)
{
	auto iter = m_DynamicObjects.begin();
	while (iter != m_DynamicObjects.end())
	{
		if (iter->second.pObject->Is_Dead())
		{
			m_pScene->removeActor(*iter->second.pActor);

			Safe_Release(iter->second.pObject);
			iter->second.pActor->release();
			iter = m_DynamicObjects.erase(iter);
			continue;
		}

		PxTransform SceneTransform;
		if (FAILED(Convert_Transform(iter->second.pObject, SceneTransform)))
		{
			iter++;
			continue;
		}
		iter->second.pActor->setGlobalPose(SceneTransform);
		iter++;
	}

	m_pScene->simulate(fTimeDelta);
	m_pScene->fetchResults(true);
}

HRESULT CPhysX_Manager::Add_Static_Actor(const PHYSX_INIT_DESC& Desc, _bool isKinematic)
{
	if (nullptr == Desc.pGameObject)
		return E_FAIL;

	if (Desc.eRigidType >= PhysXRigidType::RIGID_TYPE_END)
		return E_FAIL;


	if (Desc.eRigidType == PhysXRigidType::DYNAMIC)
	{
		auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
		if (iter != m_StaticObjects.end())
			return E_FAIL;
	}
	else
	{
		auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
		if (iter != m_DynamicObjects.end())
			return E_FAIL;
	}


	if (Desc.eColliderType == PhysXColliderType::BOX)
	{
		if (FAILED(Create_Box(Desc, isKinematic)))
			return E_FAIL;
	}
	else if (Desc.eColliderType == PhysXColliderType::SPHERE)
	{
		if (FAILED(Create_Sphere(Desc, isKinematic)))
			return E_FAIL;
	}
	else if (Desc.eColliderType == PhysXColliderType::MESH)
	{
		if (FAILED(Create_Mesh(Desc, isKinematic)))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CPhysX_Manager::Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc, _bool isKinematic)
{
	if (nullptr == Desc.pGameObject)
		return E_FAIL;

	if (Desc.eRigidType >= PhysXRigidType::RIGID_TYPE_END)
		return E_FAIL;

	if (Desc.eColliderType == PhysXColliderType::BOX)
	{
		if (FAILED(Create_Box(Desc, isKinematic)))
			return E_FAIL;
	}
	else if (Desc.eColliderType == PhysXColliderType::SPHERE)
	{
		if (FAILED(Create_Sphere(Desc, isKinematic)))
			return E_FAIL;
	}
	else if (Desc.eColliderType == PhysXColliderType::MESH)
	{
		if (FAILED(Create_Mesh(Desc, isKinematic)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPhysX_Manager::Add_Ground(CGameObject* pGroundObj)
{

	auto iter = m_GroundObjects.find(pGroundObj->Get_ObjectID());
	if (iter != m_GroundObjects.end())
		return E_FAIL;

	CTransform* pTransform = pGroundObj->Get_Component<CTransform>(L"Com_Transform");
	CModel* pModel = pGroundObj->Get_Component<CModel>(L"Com_Model");

	if (nullptr == pModel || nullptr == pTransform)
		return E_FAIL;

	for (auto& pMesh : pModel->Get_Meshes())
	{
		const vector<VTXANIMMODEL>& AnimVB = pMesh->Get_AnimVertices();
		const vector<VTXMODEL>& NonAnimVB = pMesh->Get_NoneAnimVertice();

		const vector<FACEINDICES32>& FaceIndices = pMesh->Get_FaceIndices();
		_uint iNumVertices = pMesh->Get_VertexCount();
		_uint iNumPrimitives = pMesh->Get_NumPrimitives();
		_uint iNumIndices = iNumPrimitives * 3;

		vector<PxVec3> PhysXVertices;
		PhysXVertices.reserve(iNumVertices);

		Matrix WorldMatrix = pTransform->Get_WorldMatrix();

		if (AnimVB.size() > 0)
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = AnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}
		else
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = NonAnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}

		vector<PxU32> PhysXIndices;
		PhysXIndices.reserve(iNumIndices);

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			FACEINDICES32 Index = FaceIndices[i];

			PhysXIndices.push_back(Index._0);
			PhysXIndices.push_back(Index._1);
			PhysXIndices.push_back(Index._2);
		}

		PxTriangleMeshDesc tDesc;

		tDesc.points.count = iNumVertices;
		tDesc.points.stride = sizeof(PxVec3);
		tDesc.points.data = PhysXVertices.data();

		tDesc.triangles.count = iNumPrimitives;
		tDesc.triangles.stride = sizeof(PxU32) * 3;
		tDesc.triangles.data = PhysXIndices.data();


		PxTriangleMesh* pTriangleMesh = PxCreateTriangleMesh(PxCookingParams(PxTolerancesScale(0.0f, 0.0f)), tDesc);



		PxTriangleMeshGeometry* pGeometry = new PxTriangleMeshGeometry(pTriangleMesh);


		PxTransform pxTransform;
		if (FAILED(Convert_Transform(pGroundObj, pxTransform)))
			return E_FAIL;

		PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);


		PxMaterial* Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		pActor->attachShape(*pShape);
		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = pGroundObj;

		m_GroundObjects.emplace(pGroundObj->Get_ObjectID(), ObjectDesc);

		Safe_AddRef(pGroundObj);
	}
	return S_OK;
}



HRESULT CPhysX_Manager::Remove_Actor(_uint iObjectID, PhysXRigidType eRigidType)
{
	if (eRigidType >= PhysXRigidType::RIGID_TYPE_END)
		return E_FAIL;

	if (eRigidType == PhysXRigidType::DYNAMIC)
	{
		auto iter = m_DynamicObjects.find(iObjectID);
		if (iter == m_DynamicObjects.end())
			return E_FAIL;

		m_pScene->removeActor(*iter->second.pActor);
		Safe_Release(iter->second.pObject);
		iter->second.pActor->release();
		m_DynamicObjects.erase(iter);
	}
	else
	{
		auto iter = m_StaticObjects.find(iObjectID);
		if (iter == m_StaticObjects.end())
			return E_FAIL;

		m_pScene->removeActor(*iter->second.pActor);
		Safe_Release(iter->second.pObject);
		iter->second.pActor->release();
		m_StaticObjects.erase(iter);
	}

	return S_OK;
}


HRESULT CPhysX_Manager::Create_Box(const PHYSX_INIT_DESC& Desc, _bool isKinematic)
{
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(Desc.vExtents.x * 0.5f, Desc.vExtents.y * 0.5f, Desc.vExtents.z * 0.5f), *m_WorldMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);


	PxTransform pxTransform;
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return E_FAIL;



	if (Desc.eRigidType == PhysXRigidType::DYNAMIC)
	{
		PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);
		pActor->setAngularDamping(90.f);
		pActor->setMaxLinearVelocity(10.f);

		pActor->attachShape(*shape);
		pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

		PxRigidBodyExt::updateMassAndInertia(*pActor, 10.f); // 무게, 관성
		m_pScene->addActor(*pActor);

		PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;
		
		m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
		
		Safe_AddRef(Desc.pGameObject);

		CRigidBody* pRigidBody = Desc.pGameObject->Get_Component<CRigidBody>(L"Com_RigidBody");
		if (nullptr != pRigidBody)
			pRigidBody->Set_PhysXBody(pActor);
		
	}
	else
	{
		PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
		pActor->attachShape(*shape);
		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;

		m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
		Safe_AddRef(Desc.pGameObject);
	}


	shape->release();

	return S_OK;
}

HRESULT CPhysX_Manager::Create_Sphere(const PHYSX_INIT_DESC& Desc, _bool isKinematic)
{
	PxShape* shape = m_Physics->createShape(PxSphereGeometry(Desc.fRadius), *m_WorldMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);


	PxTransform pxTransform;
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return E_FAIL;



	if (Desc.eRigidType == PhysXRigidType::DYNAMIC)
	{
		PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);
		pActor->attachShape(*shape);
		pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
		PxRigidBodyExt::updateMassAndInertia(*pActor, 10.f); // 무게, 관성
		m_pScene->addActor(*pActor);

		PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;


		CRigidBody* pRigidBody = Desc.pGameObject->Get_Component<CRigidBody>(L"Com_RigidBody");
		if (nullptr != pRigidBody)
			pRigidBody->Set_PhysXBody(pActor);
		else
			return E_FAIL;

		m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);

		Safe_AddRef(Desc.pGameObject);

		

	}
	else
	{
		PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
		pActor->attachShape(*shape);
		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;

		m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
		Safe_AddRef(Desc.pGameObject);
	}

	shape->release();

	return S_OK;
}

HRESULT CPhysX_Manager::Create_Mesh(const PHYSX_INIT_DESC& Desc, _bool isKinematic)
{
	CTransform* pTransform = Desc.pGameObject->Get_Component<CTransform>(L"Com_Transform");
	CModel* pModel = Desc.pGameObject->Get_Component<CModel>(L"Com_Model");

	if (nullptr == pModel || nullptr == pTransform)
		return E_FAIL;

	for (auto& pMesh : pModel->Get_Meshes())
	{
		const vector<VTXANIMMODEL>& AnimVB = pMesh->Get_AnimVertices();
		const vector<VTXMODEL>& NonAnimVB = pMesh->Get_NoneAnimVertice();

		const vector<FACEINDICES32>& FaceIndices = pMesh->Get_FaceIndices();
		_uint iNumVertices = pMesh->Get_VertexCount();
		_uint iNumPrimitives = pMesh->Get_NumPrimitives();
		_uint iNumIndices = iNumPrimitives * 3;

		vector<PxVec3> PhysXVertices;
		PhysXVertices.reserve(iNumVertices);

		Matrix WorldMatrix = pTransform->Get_WorldMatrix();

		if (AnimVB.size() > 0)
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = AnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}
		else
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = NonAnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}




		vector<PxU32> PhysXIndices;
		PhysXIndices.reserve(iNumIndices);

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			FACEINDICES32 Index = FaceIndices[i];

			PhysXIndices.push_back(Index._0);
			PhysXIndices.push_back(Index._1);
			PhysXIndices.push_back(Index._2);
		}

		PxTriangleMeshDesc tDesc;

		tDesc.points.count = iNumVertices;
		tDesc.points.stride = sizeof(PxVec3);
		tDesc.points.data = PhysXVertices.data();

		tDesc.triangles.count = iNumPrimitives;
		tDesc.triangles.stride = sizeof(PxU32) * 3;
		tDesc.triangles.data = PhysXIndices.data();


		PxTriangleMesh* pTriangleMesh = PxCreateTriangleMesh(PxCookingParams(PxTolerancesScale(0.0f, 0.0f)), tDesc);
		PxTriangleMeshGeometry* pGeometry = new PxTriangleMeshGeometry(pTriangleMesh);


		PxTransform pxTransform;
		if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
			return E_FAIL;

		
		PxMaterial* Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);
		
		if (Desc.eRigidType == PhysXRigidType::DYNAMIC)
		{
			 
			PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);
			CRigidBody* pRigidBody = Desc.pGameObject->Get_Component<CRigidBody>(L"Com_RigidBody");
			if (nullptr != pRigidBody)
				pRigidBody->Set_PhysXBody(pActor);

			pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
			pActor->attachShape(*pShape);
			m_pScene->addActor(*pActor);

			PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
			ObjectDesc.pActor = pActor;
			ObjectDesc.pObject = Desc.pGameObject;

			m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
			Safe_AddRef(Desc.pGameObject);
		}
			
		else
		{
			PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
			pActor->attachShape(*pShape);
			m_pScene->addActor(*pActor);

			PHYSX_STATIC_OBJECT_DESC ObjectDesc;
			ObjectDesc.pActor = pActor;
			ObjectDesc.pObject = Desc.pGameObject;

			m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
			Safe_AddRef(Desc.pGameObject);

		}
		pShape->release();
		Material->release();
	}

	return S_OK;
}


HRESULT CPhysX_Manager::Convert_Transform(CGameObject* pObj, PxTransform& pxTransform)
{
	CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return E_FAIL;

	Matrix WorldMatrix = pTransform->Get_WorldMatrix();
	Vec3 vPos, vScale;
	Quaternion vQuat;

	WorldMatrix.Decompose(vScale, vQuat, vPos);

	pxTransform = PxTransform(PxVec3(vPos.x, vPos.y, vPos.z), PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w));
	return S_OK;
}

//
//PxRigidDynamic* CPhysX_Manager::Create_Box(_float3 vPos, _float3 vExtents, _uint iFlag)
//{
//	// 박스를 만든다.
//	PxShape* shape = m_Physics->createShape(PxBoxGeometry(vExtents.x * 0.5f, vExtents.y * 0.5f, vExtents.z * 0.5f), *m_Material);
//	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//
//	shape->setSimulationFilterData(PxFilterData(iFlag, 0, 0, 0));
//
//	PxTransform localTm(PxVec3(vPos.x, vPos.y, vPos.z)); // 포지션 설정
//
//	PxRigidDynamic* body = m_Physics->createRigidDynamic(localTm);
//	body->attachShape(*shape);
//	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
//
//
//	PxRigidBodyExt::updateMassAndInertia(*body, 10.f); // 무게, 관성
//	m_pScene->addActor(*body);
//
//	shape->release();
//
//	return body;
//}
//
//PxRigidDynamic* CPhysX_Manager::Create_Sphere(_float3 vPos, _float fRad, _uint iFlag)
//{
//	// 박스를 만든다.
//	PxShape* Shape = m_Physics->createShape(PxSphereGeometry(PxReal(fRad)), *m_Material);
//	Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//
//	Shape->setSimulationFilterData(PxFilterData(iFlag, 0, 0, 0));
//
//
//	PxTransform localTm(PxVec3(vPos.x, vPos.y, vPos.z));
//	PxRigidDynamic* body = m_Physics->createRigidDynamic(localTm);
//
//	body->attachShape(*Shape);
//	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
//
//	PxRigidBodyExt::updateMassAndInertia(*body, 0.1f);
//
//	m_pScene->addActor(*body);
//	Shape->release();
//
//	return body;
//}
//
//PxRigidDynamic * CPhysX_Manager::Create_PxBox(_float3 vExtents, _float fWeight, _float fAngleDamp, PxMaterial* pMaterial, _float fMaxVel)
//{
//
//	// 박스를 만든다.
//	PxShape* shape = m_Physics->createShape(PxBoxGeometry(vExtents.x * 0.5f, vExtents.y * 0.5f, vExtents.z * 0.5f), *pMaterial);
//	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//
//	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));
//
//	// 이때의 vPos는 상대적인 포스
//	// PxTransform localTm(PxVec3(vPos.x, vPos.y, vPos.z)); // 포지션 설정
//	PxRigidDynamic* body = m_Physics->createRigidDynamic(PxTransform(0.f, 0.f, 0.f));
//	body->attachShape(*shape);
//
//	body->setAngularDamping(fAngleDamp); // 회전을 방해하는 힘
//
//	if(fMaxVel)
//		body->setMaxLinearVelocity(fMaxVel); // 최대 속도
//
//	PxRigidBodyExt::updateMassAndInertia(*body, fWeight); // 무게, 관성
//
//	m_pScene->addActor(*body);
//	shape->release();
//
//	return body;
//}
//
//PxRigidDynamic * CPhysX_Manager::Create_PxSphere(_float3 vExtents, _float fWeight, _float fAngleDamp, PxMaterial* pMaterial, _float fMaxVel)
//{
//
//	PxShape* pPxShape = m_Physics->createShape(PxSphereGeometry(PxReal(vExtents.x)), *m_Material);
//	pPxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//	pPxShape->setDensityForFluid(10.f); // 밀도
//
//	pPxShape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));
//
//	PxRigidDynamic* body = m_Physics->createRigidDynamic(PxTransform(0.f, 0.f, 0.f));
//	body->attachShape(*pPxShape);
//
//
//	body->setAngularDamping(fAngleDamp);
//
//	if (fMaxVel)
//		body->setMaxLinearVelocity(fMaxVel);
//
//	PxRigidBodyExt::updateMassAndInertia(*body, fWeight);
//
//	m_pScene->addActor(*body);
//
//	pPxShape->release();
//
//	return body;
//}



HRESULT CPhysX_Manager::Reset_PhysX()
{

	return S_OK;
}

void CPhysX_Manager::Free()
{
	for (auto& iter : m_StaticObjects)
	{
		m_pScene->removeActor(*iter.second.pActor);
		Safe_Release(iter.second.pObject);
		iter.second.pActor->release();
	}
	m_StaticObjects.clear();

	for (auto& iter : m_DynamicObjects)
	{
		m_pScene->removeActor(*iter.second.pActor);
		Safe_Release(iter.second.pObject);
		iter.second.pActor->release();
	}
	m_DynamicObjects.clear();


	for (auto& iter : m_GroundObjects)
	{
		m_pScene->removeActor(*iter.second.pActor);
		Safe_Release(iter.second.pObject);
		iter.second.pActor->release();
	}
	m_GroundObjects.clear();
	
	


	PxCloseExtensions();
	if (m_WorldMaterial)
		m_WorldMaterial->release();

	if (m_Dispatcher)
		m_Dispatcher->release();

	if(m_pCudaContextManager)
		m_pCudaContextManager->release();

	if (m_pPvd)
		m_pPvd->release();

	if (m_pScene)
		m_pScene->release();

	if (m_Physics)
		m_Physics->release();

	if(m_Foundation)
		m_Foundation->release();
}

void CPhysX_Manager::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CPhysX_Manager::onWake(PxActor** actors, PxU32 count)
{
}

void CPhysX_Manager::onSleep(PxActor** actors, PxU32 count)
{
}

void CPhysX_Manager::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
}

void CPhysX_Manager::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void CPhysX_Manager::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}




