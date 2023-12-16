#include "PhysX_Manager.h"
#include "GameInstance.h"

#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"
#include "Collision_Manager.h"


physx::PxFilterFlags FilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData /*filterData0*/,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData /*filterData1*/,
	physx::PxPairFlags& retPairFlags, const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
{
	
	
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
	if (PxFilterObjectIsKinematic(attributes0) && PxFilterObjectIsKinematic(attributes1))
	{
		retPairFlags |= PxPairFlag::eTRIGGER_DEFAULT;
	}
	return PxFilterFlag::eNOTIFY;
}


IMPLEMENT_SINGLETON(CPhysX_Manager)


CPhysX_Manager::CPhysX_Manager()
{

}

HRESULT CPhysX_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_pPvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(m_strIPAddress.c_str(), m_iPortNumber, m_iTimeOutSeconds);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_pPvd);
	PxInitExtensions(*m_Physics, m_pPvd);
	transport->release();

	PxSceneDesc SceneDesc(m_Physics->getTolerancesScale());
	SceneDesc.gravity = PxVec3(0.0f, 0.f, 0.0f);


	m_Dispatcher = PxDefaultCpuDispatcherCreate(4);
	if (!m_Dispatcher)
		return E_FAIL;

	
	SceneDesc.cpuDispatcher = m_Dispatcher;
	SceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.filterShader = FilterShader;
	SceneDesc.simulationEventCallback = this;

	m_pScene = m_Physics->createScene(SceneDesc);

	
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);


	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		//I have a PVD client, so set some flags that it needs
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_WorldMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);

	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(pContext);
	m_pEffect = new BasicEffect(pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;
#endif

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

		PxTransform Position = iter->second.pActor->getGlobalPose();
		PxTransform KinematicPos;
		iter->second.pActor->getKinematicTarget(KinematicPos);

		iter++;
	}


	m_pScene->simulate(min(fTimeDelta, 1.f / 144.f));
	m_pScene->fetchResults(true);
}

#ifdef _DEBUG
HRESULT CPhysX_Manager::Render()
{
	const PxRenderBuffer& rb = m_pScene->getRenderBuffer();

	PxU32 iTrangleCount = rb.getNbTriangles();
	for (PxU32 i = 0; i < rb.getNbTriangles(); i++)
	{
		const PxDebugTriangle& Triangle = rb.getTriangles()[i];

		Vec3 vPos0 = { Triangle.pos0.x,Triangle.pos0.y, Triangle.pos0.z };
		Vec3 vPos1 = { Triangle.pos1.x,Triangle.pos1.y, Triangle.pos1.z };
		Vec3 vPos2 = { Triangle.pos2.x,Triangle.pos2.y, Triangle.pos2.z };
		
		m_pEffect->SetWorld(XMMatrixIdentity());
		m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
		m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

		m_pEffect->Apply(m_pContext);

		m_pContext->IASetInputLayout(m_pInputLayout);
		m_pBatch->Begin();

		DX::DrawTriangle(m_pBatch, vPos0, vPos1, vPos2);
		m_pBatch->End();
	}

	return S_OK;
}
#endif

PxRigidStatic* CPhysX_Manager::Add_Static_Actor(const PHYSX_INIT_DESC& Desc)
{
	if (nullptr == Desc.pGameObject)
		return nullptr;

	if (Desc.eRigidType == PHYSX_COLLIDER_TYPE::MESH)
		return nullptr;

	if (Desc.eRigidType >= PHYSX_COLLIDER_TYPE::MESH)
		return nullptr;

	auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_StaticObjects.end())
		return nullptr;

	if (Desc.eColliderType == PHYSX_COLLIDER_TYPE::BOX)	
		return Create_Static_Box(Desc);	

	else if (Desc.eColliderType == PHYSX_COLLIDER_TYPE::SPHERE)
		return Create_Static_Sphere(Desc);

	return nullptr;
}

PxRigidDynamic* CPhysX_Manager::Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc)
{
	if (nullptr == Desc.pGameObject)
		return nullptr;

	if (Desc.eRigidType >= PHYSX_RIGID_TYPE::RIGID_TYPE_END)
		return nullptr;


	if (Desc.eRigidType == PHYSX_COLLIDER_TYPE::MESH)
		return nullptr;

	auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_DynamicObjects.end())
		return nullptr;



	if (Desc.eColliderType == PHYSX_COLLIDER_TYPE::BOX)
	{
		return Create_Dynamic_Box(Desc);
	}
	else if (Desc.eColliderType == PHYSX_COLLIDER_TYPE::SPHERE)
	{
		return Create_Dynamic_Sphere(Desc);
	}

	return nullptr;
}

vector<PxRigidStatic*> CPhysX_Manager::Add_Static_Mesh_Actor(const PHYSX_INIT_DESC& Desc)
{
	if (Desc.eRigidType != PHYSX_COLLIDER_TYPE::MESH)
		return vector<PxRigidStatic*>();

	auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_StaticObjects.end())
		return vector<PxRigidStatic*>();



	return Create_Static_Mesh(Desc);
}

vector<PxRigidDynamic*> CPhysX_Manager::Add_Dynamic_Mesh_Actor(const PHYSX_INIT_DESC& Desc)
{
	if (Desc.eRigidType != PHYSX_COLLIDER_TYPE::MESH)
		return vector<PxRigidDynamic*>();

	auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_DynamicObjects.end())
		return vector<PxRigidDynamic*>();

	return Create_Dynamic_Mesh(Desc);
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

HRESULT CPhysX_Manager::Remove_Actor(_uint iObjectID, PHYSX_RIGID_TYPE eRigidType)
{
	if (eRigidType >= PHYSX_RIGID_TYPE::RIGID_TYPE_END)
		return E_FAIL;

	if (eRigidType == PHYSX_RIGID_TYPE::DYNAMIC)
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

HRESULT CPhysX_Manager::Remove_Actor(PxActor* pPhysXActor)
{
	if (nullptr == pPhysXActor)
		return E_FAIL;

	m_pScene->removeActor(*pPhysXActor);
	return S_OK;
}


PxRigidDynamic* CPhysX_Manager::Create_Dynamic_Box(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(Desc.vExtents.x * 0.5f, Desc.vExtents.y * 0.5f, Desc.vExtents.z * 0.5f), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	//shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	//shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));
	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

	PxTransform pxTransform;
	
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return nullptr;

	pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
	PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);

	
	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);
	
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);

	pActor->setAngularDamping(Desc.fAngularDamping);
	pActor->setMaxLinearVelocity(Desc.fMaxVelocity);
	pActor->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pActor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);

	PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성

	pActor->attachShape(*shape);
	m_pScene->addActor(*pActor);

	PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	return pActor;
}

PxRigidStatic* CPhysX_Manager::Create_Static_Box(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(Desc.vExtents.x * 0.5f, Desc.vExtents.y * 0.5f, Desc.vExtents.z * 0.5f), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform pxTransform;
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return nullptr;

	pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
	PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
	pActor->attachShape(*shape);
	m_pScene->addActor(*pActor);

	PHYSX_STATIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	pMateral->release();
	shape->release();

	return pActor;
}

PxRigidDynamic* CPhysX_Manager::Create_Dynamic_Sphere(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxSphereGeometry(Desc.fRadius), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform pxTransform;
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return nullptr;

	pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
	PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);

	pActor->attachShape(*shape);
	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);

	PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성
	m_pScene->addActor(*pActor);

	PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	pMateral->release();
	shape->release();


	return pActor;
}

PxRigidStatic* CPhysX_Manager::Create_Static_Sphere(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxSphereGeometry(Desc.fRadius), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform pxTransform;
	if (FAILED(Convert_Transform(Desc.pGameObject, pxTransform)))
		return nullptr;

	pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
	PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
	pActor->attachShape(*shape);
	m_pScene->addActor(*pActor);
	

	PHYSX_STATIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	pMateral->release();
	shape->release();

	return pActor;
}

vector<PxRigidDynamic*> CPhysX_Manager::Create_Dynamic_Mesh(const PHYSX_INIT_DESC& Desc)
{
	CTransform* pTransform = Desc.pGameObject->Get_Component<CTransform>(L"Com_Transform");
	CModel* pModel = Desc.pGameObject->Get_Component<CModel>(L"Com_Model");

	
	if (nullptr == pModel || nullptr == pTransform)
	{
		vector<PxRigidDynamic*> Temp;
		return Temp;
	}
	vector<PxRigidDynamic*> Actors;
	
	Actors.reserve(pModel->Get_NumMeshes());
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
		{
			vector<PxRigidDynamic*> Temp;
			return Temp;
		}


		PxMaterial* Material = m_Physics->createMaterial(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
		PxRigidDynamic* pActor = m_Physics->createRigidDynamic(pxTransform);
		pActor->attachShape(*pShape);
		pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);

		PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성
		m_pScene->addActor(*pActor);

		PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;

		m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
		Safe_AddRef(Desc.pGameObject);

		Actors.push_back(pActor);

		pShape->release();
		Material->release();
	}

	return Actors;
}

vector<PxRigidStatic*> CPhysX_Manager::Create_Static_Mesh(const PHYSX_INIT_DESC& Desc)
{
	CTransform* pTransform = Desc.pGameObject->Get_Component<CTransform>(L"Com_Transform");
	CModel* pModel = Desc.pGameObject->Get_Component<CModel>(L"Com_Model");


	if (nullptr == pModel || nullptr == pTransform)
	{
		vector<PxRigidStatic*> Temp;
		return Temp;
	}

	vector<PxRigidStatic*> Actors;
	Actors.reserve(pModel->Get_NumMeshes());

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
		{
			vector<PxRigidStatic*> Temp;
			return Temp;
		}


		PxMaterial* Material = m_Physics->createMaterial(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		pxTransform.p += PxVec3(Desc.vOffsetPosition.x, Desc.vOffsetPosition.y, Desc.vOffsetPosition.z);
		PxRigidStatic* pActor = m_Physics->createRigidStatic(pxTransform);
		pActor->attachShape(*pShape);
		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;

		m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), ObjectDesc);
		Safe_AddRef(Desc.pGameObject);

		Material->release();
		pShape->release();

		Actors.push_back(pActor);
	}
	return Actors;
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
	pxTransform = PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w));
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


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Release(m_pInputLayout);
#endif
}



void CPhysX_Manager::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CPhysX_Manager::onWake(PxActor** actors, PxU32 count)
{
 	_int i = 0;
}

void CPhysX_Manager::onSleep(PxActor** actors, PxU32 count)
{
	_int i = 0;
}

void CPhysX_Manager::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	if (nullptr == pairHeader.actors[0]->userData || nullptr == pairHeader.actors[1]->userData)
		return;


  	CCollider* pLeftCollider = (CCollider*)pairHeader.actors[0]->userData;
	CCollider* pRightCollider = (CCollider*)pairHeader.actors[1]->userData;

	if (pLeftCollider->Get_ColliderID() == pRightCollider->Get_ColliderID())
		return;

	if (true == pairs->events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND))
	{
		pLeftCollider->Collision_Enter(pRightCollider);
		pRightCollider->Collision_Enter(pLeftCollider);
	}
	else if (true == pairs->events.isSet(PxPairFlag::eNOTIFY_TOUCH_PERSISTS))
	{
		pLeftCollider->Collision_Continue(pRightCollider);
		pRightCollider->Collision_Continue(pLeftCollider);
	}
	else if (true == pairs->events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST))
	{
		pLeftCollider->Collision_Exit(pRightCollider);
		pRightCollider->Collision_Exit(pLeftCollider);
	}

}

void CPhysX_Manager::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	int i = 0;
}

void CPhysX_Manager::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	int i = 0;
}






