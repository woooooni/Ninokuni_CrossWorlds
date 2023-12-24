#include "PhysX_Manager.h"
#include "GameInstance.h"

#include "GameObject.h"
#include "Mesh.h"
#include "Collider.h"
#include "Collision_Manager.h"
#include "VIBuffer.h"
#define PX_SUPPORT_PVD



physx::PxFilterFlags FilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& retPairFlags, const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
{
	retPairFlags |= PxPairFlag::eTRIGGER_DEFAULT;
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	retPairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
	retPairFlags &= ~PxPairFlag::eSOLVE_CONTACT;
	

	return PxFilterFlag::eDEFAULT;
}


IMPLEMENT_SINGLETON(CPhysX_Manager)


CPhysX_Manager::CPhysX_Manager()
{
	
}

HRESULT CPhysX_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	PxCudaContextManagerDesc CudaDesc;
	m_pCudaContextManager = PxCreateCudaContextManager(*m_Foundation, CudaDesc);

	/*m_pPvd = PxCreatePvd(*m_Foundation);
	m_pTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", m_iPortNumber, m_iTimeOutSeconds);
	m_pPvd->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);
	_bool bConntected = m_pPvd->isConnected();*/




	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_pPvd);
	PxInitExtensions(*m_Physics, m_pPvd);
	

	PxSceneDesc SceneDesc(m_Physics->getTolerancesScale());

	SceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	
	m_Dispatcher = PxDefaultCpuDispatcherCreate(4);
	if (!m_Dispatcher)
		return E_FAIL;

	
	SceneDesc.cpuDispatcher = m_Dispatcher;
	SceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	SceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	SceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
	SceneDesc.solverType = PxSolverType::eTGS;
	SceneDesc.filterShader = FilterShader;
	SceneDesc.simulationEventCallback = this;
	SceneDesc.cudaContextManager = m_pCudaContextManager;



	m_pScene = m_Physics->createScene(SceneDesc);
	m_pController_Manager = PxCreateControllerManager(*m_pScene);
	// m_pController_Manager->setOverlapRecoveryModule(true);
	
	
	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (nullptr != pvdClient)
	{
		//I have a PVD client, so set some flags that it needs
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

#ifdef DEBUG
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCONTACT_NORMAL, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eSIMULATION_MESH, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_STATIC, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_DYNAMIC, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
#endif // DEBUG

	



	m_WorldMaterial = m_Physics->createMaterial(0.f, 0.f, 0.f);

	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	/*if (FAILED(Ready_ParticleSystem()))
		return E_FAIL;*/



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

	//if (FAILED(Init_Cloth(100, 100, PxVec3(-0.5f * 100 * 0.05f, 8.f, -0.5f * 100 * 0.05f), 0.05f, 10.f)))
	//	return E_FAIL;

	/*PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_WorldMaterial);
	m_pScene->addActor(*groundPlane);*/


	return S_OK;
}

void CPhysX_Manager::Tick(_float fTimeDelta)
{	



	
}

void CPhysX_Manager::LateTick(_float fTimeDelta)
{
	fTimeDelta = min(fTimeDelta, 1.f / 144.f);

	for (auto& CollisionDesc : m_GroundCollision)
	{
		switch (CollisionDesc.flag)
		{
		case PxPairFlag::eNOTIFY_TOUCH_FOUND:
			CollisionDesc.pCollideObject->Ground_Collision_Enter(CollisionDesc);
			break;
		case PxPairFlag::eNOTIFY_TOUCH_PERSISTS:
			CollisionDesc.pCollideObject->Ground_Collision_Continue(CollisionDesc);
			break;
		case PxPairFlag::eNOTIFY_TOUCH_LOST:
			CollisionDesc.pCollideObject->Ground_Collision_Exit(CollisionDesc);
			break;
		}
	}
	m_GroundCollision.clear();

	
	m_pScene->simulate(fTimeDelta);
	m_pScene->fetchResults(true);
	m_pScene->fetchResultsParticleSystem();
}

#ifdef _DEBUG
HRESULT CPhysX_Manager::Render()
{	
	//const PxRenderBuffer& rb = m_pScene->getRenderBuffer();

	//PxU32 iTrangleCount = rb.getNbTriangles();
	//for (PxU32 i = 0; i < rb.getNbTriangles(); i++)
	//{
	//	const PxDebugTriangle& Triangle = rb.getTriangles()[i];

	//	Vec3 vPos0 = { Triangle.pos0.x,Triangle.pos0.y, Triangle.pos0.z };
	//	Vec3 vPos1 = { Triangle.pos1.x,Triangle.pos1.y, Triangle.pos1.z };
	//	Vec3 vPos2 = { Triangle.pos2.x,Triangle.pos2.y, Triangle.pos2.z };
	//	
	//	m_pEffect->SetWorld(XMMatrixIdentity());
	//	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	//	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	//	m_pEffect->Apply(m_pContext);

	//	m_pContext->IASetInputLayout(m_pInputLayout);
	//	m_pBatch->Begin();

	//	DX::DrawTriangle(m_pBatch, vPos0, vPos1, vPos2);
	//	m_pBatch->End();
	//}

	return S_OK;
}
#endif

PxRigidStatic* CPhysX_Manager::Add_Static_Actor(const PHYSX_INIT_DESC& Desc)
{
	if (nullptr == Desc.pGameObject)
		return nullptr;

	if (Desc.eRigidType == PHYSX_COLLIDER_TYPE::MESH)
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

HRESULT CPhysX_Manager::Add_Static_Mesh_Actor(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidStatic*>& refOut)
{
	if (Desc.eRigidType != PHYSX_COLLIDER_TYPE::MESH)
		return E_FAIL;

	auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_StaticObjects.end())
		return E_FAIL;

	return Create_Static_Mesh(Desc, refOut);
}

HRESULT CPhysX_Manager::Add_Dynamic_Mesh_Actor(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidDynamic*>& refOut)
{
	if (Desc.eRigidType != PHYSX_COLLIDER_TYPE::MESH)
		return E_FAIL;

	auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter != m_DynamicObjects.end())
		return E_FAIL;

	return Create_Dynamic_Mesh(Desc, refOut);
}

HRESULT CPhysX_Manager::Remove_Actor(class CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto iterGround = m_GroundObjects.find(pGameObject->Get_ObjectID());
	for (auto& Desc : iterGround->second)
	{
		m_pScene->removeActor(*Desc.pActor);
		Safe_Release(Desc.pObject);
	}
	iterGround->second.clear();

	auto iterStatic = m_StaticObjects.find(pGameObject->Get_ObjectID());
	for (auto& Desc : iterStatic->second)
	{
		m_pScene->removeActor(*Desc.pActor);
		Safe_Release(Desc.pObject);
	}
	iterStatic->second.clear();

	auto iterDynamic = m_DynamicObjects.find(pGameObject->Get_ObjectID());
	for (auto& Desc : iterDynamic->second)
	{
		m_pScene->removeActor(*Desc.pActor);
		Safe_Release(Desc.pObject);
	}
	iterDynamic->second.clear();

	

	
	return S_OK;
}

PxController* CPhysX_Manager::Add_CapsuleController(CGameObject* pGameObject, Matrix WorldMatrix, _float fHeight, _float fRadius, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	// 컨트롤러는 하나만 가질 수 있다.
	if (nullptr == pGameObject)
		return nullptr;

	auto iter = m_Controllers.find(pGameObject->Get_ObjectID());
	if (iter != m_Controllers.end())
		return nullptr;

	PxCapsuleControllerDesc CapsuleDesc;
	PxTransform pxTransform = GI->To_PxTransform(WorldMatrix);

	CapsuleDesc.setToDefault();

	CapsuleDesc.material = m_WorldMaterial;
	CapsuleDesc.position = PxExtendedVec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
	CapsuleDesc.height = fHeight;
	CapsuleDesc.radius = fRadius;

	CapsuleDesc.maxJumpHeight = fMaxJumpHeight;
	CapsuleDesc.stepOffset = 0.1f;
	CapsuleDesc.contactOffset = 0.0001f;
	CapsuleDesc.userData = pGameObject;
	CapsuleDesc.reportCallback = pCallBack;
	

	PxController* pController = m_pController_Manager->createController(CapsuleDesc);

	pController->getActor()->userData = pGameObject;
	pController->getActor()->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pController->getActor()->setName("Controller");
	

	return pController;
}

PxController* CPhysX_Manager::Add_BoxController(CGameObject* pGameObject, Matrix WorldMatrix, _float3 fExtents, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	// 컨트롤러는 하나만 가질 수 있다.
	if (nullptr == pGameObject)
		return nullptr;

	auto iter = m_Controllers.find(pGameObject->Get_ObjectID());
	if (iter != m_Controllers.end())
		return nullptr;

	PxBoxControllerDesc BoxDesc;
	PxTransform pxTransform = GI->To_PxTransform(WorldMatrix);

	BoxDesc.setToDefault();

	BoxDesc.material = m_WorldMaterial;
	BoxDesc.position = PxExtendedVec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
	BoxDesc.halfSideExtent = fExtents.x / 2.f;
	BoxDesc.halfHeight = fExtents.y / 2.f;
	BoxDesc.halfForwardExtent = fExtents.z / 2.f;

	BoxDesc.maxJumpHeight = fMaxJumpHeight;
	BoxDesc.userData = pGameObject;
	BoxDesc.reportCallback = pCallBack;


	PxController* pController = m_pController_Manager->createController(BoxDesc);
	pController->getActor()->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pController->getActor()->setName("Controller");

	return pController;
}



// For. Collider.
PxRigidDynamic* CPhysX_Manager::Create_Dynamic_Box(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(Desc.vExtents.x, Desc.vExtents.y, Desc.vExtents.z), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

	PxVec3 vPxStartPositon = PxVec3(Desc.vStartPosition.x, Desc.vStartPosition.y, Desc.vStartPosition.z);
	PxTransform physXTransform = PxTransform(vPxStartPositon);
	PxRigidDynamic* pActor = m_Physics->createRigidDynamic(physXTransform);

	
	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);

	pActor->setAngularDamping(Desc.fAngularDamping);
	pActor->setMaxLinearVelocity(Desc.fMaxVelocity);
	pActor->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pActor->setName("Collider");

	PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성

	pActor->attachShape(*shape);
	m_pScene->addActor(*pActor);

	PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter == m_DynamicObjects.end())
	{
		vector<PHYSX_DYNAMIC_OBJECT_DESC> DescTemp;
		m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
		iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
		iter->second.reserve(50);
	}

	iter->second.push_back(ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	pMateral->release();
	shape->release();

	return pActor;
}

PxRigidStatic* CPhysX_Manager::Create_Static_Box(const PHYSX_INIT_DESC& Desc)
{
	PxMaterial* pMateral = Create_Material(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(Desc.vExtents.x, Desc.vExtents.y, Desc.vExtents.z), *pMateral);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	// shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

	PxVec3 vPxStartPositon = PxVec3(Desc.vStartPosition.x, Desc.vStartPosition.y, Desc.vStartPosition.z);
	PxTransform physXTransform = PxTransform(vPxStartPositon);
	PxRigidStatic* pActor = m_Physics->createRigidStatic(physXTransform);

	pActor->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pActor->attachShape(*shape);
	pActor->setName("Collider");

	m_pScene->addActor(*pActor);

	PHYSX_STATIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter == m_StaticObjects.end())
	{
		vector<PHYSX_STATIC_OBJECT_DESC> DescTemp;
		m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
		iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
		iter->second.reserve(50);
	}

	iter->second.push_back(ObjectDesc);
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
	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

	PxVec3 vPxStartPositon = PxVec3(Desc.vStartPosition.x, Desc.vStartPosition.y, Desc.vStartPosition.z);
	PxTransform physXTransform = PxTransform(vPxStartPositon);
	PxRigidDynamic* pActor = m_Physics->createRigidDynamic(physXTransform);


	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);

	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
	pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);

	pActor->setAngularDamping(Desc.fAngularDamping);
	pActor->setMaxLinearVelocity(Desc.fMaxVelocity);
	pActor->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pActor->setName("Collider");

	PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성

	pActor->attachShape(*shape);
	m_pScene->addActor(*pActor);

	PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;

	auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter == m_DynamicObjects.end())
	{
		vector<PHYSX_DYNAMIC_OBJECT_DESC> DescTemp;
		m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
		iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
		iter->second.reserve(50);
	}

	iter->second.push_back(ObjectDesc);
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
	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

	PxVec3 vPxStartPositon = PxVec3(Desc.vStartPosition.x, Desc.vStartPosition.y, Desc.vStartPosition.z);
	PxTransform physXTransform = PxTransform(vPxStartPositon);
	PxRigidStatic* pActor = m_Physics->createRigidStatic(physXTransform);

	pActor->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	pActor->attachShape(*shape);
	pActor->setName("Collider");

	m_pScene->addActor(*pActor);
	

	PHYSX_STATIC_OBJECT_DESC ObjectDesc;
	ObjectDesc.pActor = pActor;
	ObjectDesc.pObject = Desc.pGameObject;
	

	auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
	if (iter == m_StaticObjects.end())
	{
		vector<PHYSX_STATIC_OBJECT_DESC> DescTemp;
		m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
		iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
		iter->second.reserve(50);
	}

	iter->second.push_back(ObjectDesc);
	Safe_AddRef(Desc.pGameObject);

	pMateral->release();
	shape->release();

	return pActor;
}


// For. Model.
HRESULT CPhysX_Manager::Create_Dynamic_Mesh(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidDynamic*>& refOut)
{
	CModel* pModel = Desc.pGameObject->Get_Component<CModel>(L"Com_Model");

	
	if (nullptr == pModel)
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


		if (AnimVB.size() > 0)
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = AnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, Desc.WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}
		else
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = NonAnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, Desc.WorldMatrix);
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

		PxMaterial* Material = m_Physics->createMaterial(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		PxTransform physXTransform = PxTransform(PxVec3(0.f, 0.f, 0.f));

		PxRigidDynamic* pActor = m_Physics->createRigidDynamic(physXTransform);
		pActor->attachShape(*pShape);
		pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Desc.bKinematic);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Desc.bLockAngle_X);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Desc.bLockAngle_Y);
		pActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Desc.bLockAngle_Z);
		pActor->setName("Mesh");

		PxRigidBodyExt::updateMassAndInertia(*pActor, Desc.fDensity); // 무게, 관성
		pActor->userData = Desc.pGameObject;
		m_pScene->addActor(*pActor);

		PHYSX_DYNAMIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;

		auto iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
		if (iter == m_DynamicObjects.end())
		{
			vector<PHYSX_DYNAMIC_OBJECT_DESC> DescTemp;
			m_DynamicObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
			iter = m_DynamicObjects.find(Desc.pGameObject->Get_ObjectID());
			iter->second.reserve(50);
		}
		iter->second.push_back(ObjectDesc);
		Safe_AddRef(ObjectDesc.pObject);

		refOut.push_back(pActor);

		pShape->release();
		Material->release();
	}

	return S_OK;
}

HRESULT CPhysX_Manager::Create_Static_Mesh(const PHYSX_INIT_DESC& Desc, __out vector<PxRigidStatic*>& refOut)
{
	CTransform* pTransform = Desc.pGameObject->Get_Component<CTransform>(L"Com_Transform");
	CModel* pModel = Desc.pGameObject->Get_Component<CModel>(L"Com_Model");


	if (nullptr == pModel)
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

		if (AnimVB.size() > 0)
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = AnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, Desc.WorldMatrix);
				PhysXVertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}
		else
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = NonAnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, Desc.WorldMatrix);
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

		PxMaterial* Material = m_Physics->createMaterial(Desc.fStaticFriction, Desc.fDynamicFriction, Desc.fRestitution);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		PxVec3 vPxStartPositon = PxVec3(0.f, 0.f, 0.f);
		PxTransform physXTransform = PxTransform(vPxStartPositon);

		PxRigidStatic* pActor = m_Physics->createRigidStatic(physXTransform);
		pActor->userData = Desc.pGameObject;
		pActor->attachShape(*pShape);
		pActor->setName("Mesh");
		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = Desc.pGameObject;


		auto iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
		if (iter == m_StaticObjects.end())
		{
			vector<PHYSX_STATIC_OBJECT_DESC> DescTemp;
			m_StaticObjects.emplace(Desc.pGameObject->Get_ObjectID(), DescTemp);
			iter = m_StaticObjects.find(Desc.pGameObject->Get_ObjectID());
			iter->second.reserve(50);
		}
		iter->second.push_back(ObjectDesc);
		Safe_AddRef(ObjectDesc.pObject);

		Material->release();
		pShape->release();

		refOut.push_back(pActor);
	}
	return S_OK;
}

HRESULT CPhysX_Manager::Add_Ground(CGameObject* pGameObject, CModel* pModel, Matrix WorldMatrix, const wstring& strCollisionTag)
{
	if (nullptr == pModel)
		return E_FAIL;


	for (auto& pMesh : pModel->Get_Meshes())
	{
		const vector<VTXANIMMODEL>& AnimVB = pMesh->Get_AnimVertices();
		const vector<VTXMODEL>& NonAnimVB = pMesh->Get_NoneAnimVertice();

		const vector<FACEINDICES32>& FaceIndices = pMesh->Get_FaceIndices();
		_uint iNumVertices = pMesh->Get_VertexCount();
		_uint iNumPrimitives = pMesh->Get_NumPrimitives();
		_uint iNumIndices = iNumPrimitives * 3;
		_uint iStride = pMesh->Get_Stride();

		vector<PxVec3> Vertices;
		Vertices.reserve(iNumVertices);

		if (AnimVB.size() > 0)
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = AnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				Vertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}
		else
		{
			for (_uint i = 0; i < iNumVertices; ++i)
			{
				Vec3 vVertex = NonAnimVB[i].vPosition;
				vVertex = XMVector3TransformCoord(vVertex, WorldMatrix);
				Vertices.push_back(PxVec3(vVertex.x, vVertex.y, vVertex.z));
			}
		}

		vector<PxU32> Indices;
		Indices.reserve(iNumIndices);

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			FACEINDICES32 Index = FaceIndices[i];

			Indices.push_back(Index._0);
			Indices.push_back(Index._1);
			Indices.push_back(Index._2);
		}

		PxTriangleMeshDesc tDesc;

		tDesc.points.count = iNumVertices;
		tDesc.points.stride = sizeof(PxVec3);
		tDesc.points.data = Vertices.data();

		tDesc.triangles.count = iNumPrimitives;
		tDesc.triangles.stride = sizeof(PxU32) * 3;
		tDesc.triangles.data = Indices.data();


		PxTriangleMesh* pTriangleMesh = PxCreateTriangleMesh(PxCookingParams(PxTolerancesScale(1.f, 1.f)), tDesc);
		PxTriangleMeshGeometry* pGeometry = new PxTriangleMeshGeometry(pTriangleMesh);


		Vec3 vScale = {};
		Quaternion vQuat = {};
		Vec3 vPosition = {};
		WorldMatrix.Decompose(vScale, vQuat, vPosition);

		PxTransform physXTransform = PxTransform({ 0.f, 0.f, 0.f });



		PxRigidStatic* pActor = m_Physics->createRigidStatic(physXTransform);
		PxMaterial* Material = m_Physics->createMaterial(0.f, 0.f, 0.f);
		PxShape* pShape = m_Physics->createShape(*pGeometry, *Material);

		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
		pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		pShape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));
		

		pShape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

		pActor->setName("Ground");

		// shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));

		pActor->attachShape(*pShape);
		pActor->userData = nullptr;

		m_pScene->addActor(*pActor);

		PHYSX_STATIC_OBJECT_DESC ObjectDesc;
		ObjectDesc.pActor = pActor;
		ObjectDesc.pObject = pGameObject;

		auto iter = m_GroundObjects.find(pGameObject->Get_ObjectID());
		if (iter == m_GroundObjects.end())
		{
			vector<PHYSX_STATIC_OBJECT_DESC> Temp;
			m_GroundObjects.emplace(pGameObject->Get_ObjectID(), Temp);
			iter = m_GroundObjects.find(pGameObject->Get_ObjectID());
			iter->second.reserve(10);
		}

		iter->second.push_back(ObjectDesc);

		Safe_AddRef(pGameObject);
		Safe_Delete(pGeometry);

		pTriangleMesh->release();
		Material->release();
		pShape->release();
	}

	return S_OK;
}

HRESULT CPhysX_Manager::Clear_PhysX_Ground()
{
	for (auto& iter : m_GroundObjects)
	{
		for (auto& Desc : iter.second)
		{
			m_pScene->removeActor(*Desc.pActor);
			Desc.pActor->release();
			Safe_Release(Desc.pObject);
		}
		iter.second.clear();
	}
	m_GroundObjects.clear();
	return S_OK;
}

#pragma region Deprecated.
HRESULT CPhysX_Manager::Create_Cloth(CVIBuffer* pBufferCom)
{
	//PxParticleClothBuffer* clothBuffer = m_Physics->createParticleClothBuffer(pBufferCom->Get_VertexCount(), pBufferCom->Get_VertexCount(), 1, pBufferCom->Get_FaceIndices().size(), pBufferCom->Get_FaceIndices().size(), m_pCudaContextManager);
	//PxVec4* bufferPos;
	//m_pCudaContextManager->getCudaContext()->memcpyHtoDAsync(&clothBuffer->getPositionInvMasses(), bufferPos, 1000 * sizeof(PxVec4), 0);
	//particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);



	//PxPBDMaterial* defaultMat = m_Physics->createPBDMaterial(0.8f, 0.05f, 1e+6f, 0.001f, 0.5f, 0.005f, 0.05f, 0.f, 0.f);
	//PxParticleClothBuffer* clothBuffer = m_Physics->createParticleClothBuffer(pBufferCom->Get_VertexCount(), pBufferCom->Get_VertexCount(), 1, pBufferCom->Get_NumPrimitives(), pBufferCom->Get_FaceIndices().size(), m_pCudaContextManager);
	//const PxU32 particlePhase = m_pParticleSystem->createPhase(defaultMat, PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide));

	//vector<PxVec4> vertices;
	//vertices.reserve(pBufferCom->Get_VertexCount());

	//for (_uint i = 0; i < pBufferCom->Get_VertexCount(); ++i)	
	//	vertices.push_back(PxVec4(pBufferCom->Get_VertexLocalPositions()[i].x , pBufferCom->Get_VertexLocalPositions()[i].y, pBufferCom->Get_VertexLocalPositions()[i].z, 1.f));

	//vector<PxU32> indices;
	//for (auto& Index : pBufferCom->Get_FaceIndices())
	//{
	//	indices.push_back(Index._0);
	//	indices.push_back(Index._1);
	//	indices.push_back(Index._2);
	//}

	//PxParticleClothCooker* cooker = PxCreateParticleClothCooker(vertices.size(), vertices.data(), indices.size(), indices.data(),
	//	PxParticleClothConstraint::eTYPE_HORIZONTAL_CONSTRAINT | PxParticleClothConstraint::eTYPE_VERTICAL_CONSTRAINT | PxParticleClothConstraint::eTYPE_DIAGONAL_CONSTRAINT);
	//cooker->cookConstraints();
	//cooker->calculateMeshVolume();


	//PxArray<PxU32> triangles;
	//triangles.reserve(pBufferCom->Get_NumPrimitives() * 3);

	//PxU32 cookedTriangleIndicesCount = cooker->getTriangleIndicesCount();
	//PxU32* cookedTriangleIndices = cooker->getTriangleIndices();
	//for (PxU32 t = 0; t < cookedTriangleIndicesCount; t += 3)
	//{
	//	triangles.pushBack(cookedTriangleIndices[t + 0]);
	//	triangles.pushBack(cookedTriangleIndices[t + 1]);
	//	triangles.pushBack(cookedTriangleIndices[t + 2]);
	//}

	//PxArray<PxParticleSpring> springs;
	//springs.reserve(pBufferCom->Get_FaceIndices().size() * 3);

	//PxU32 constraintCount = cooker->getConstraintCount();
	//PxParticleClothConstraint* constraintBuffer = cooker->getConstraints();
	//for (PxU32 i = 0; i < constraintCount; i++)
	//{
	//	PxParticleSpring spring;
	//	spring.ind0 = triangles[i];
	//	spring.ind1 = triangles[i + 1];
	//	spring.stiffness = 10000.f;
	//	spring.length = pBufferCom->Get_FaceIndices().size();
	//	spring.damping = 0.001f;
	//	springs.pushBack(spring);
	//}

	//
	//PxParticleClothDesc clothDesc;
	//clothDesc.cloths = clothBuffer;
	//clothDesc.triangles = triangles.begin();
	//clothDesc.springs = springs.begin();
	//clothDesc.restPositions = restPositions.begin();
	//clothDesc.nbCloths = nbCloth;
	//clothDesc.nbSprings = nbSprings;
	//clothDesc.nbTriangles = nbTriangles;
	//clothDesc.nbParticles = nbParticles;

	return S_OK;
}
HRESULT CPhysX_Manager::Create_Cloth(CMesh* pMesh)
{
	return S_OK;
}
#pragma endregion


static PX_FORCE_INLINE PxU32 id(PxU32 x, PxU32 y, PxU32 numY)
{
	return x * numY + y;
}

//HRESULT CPhysX_Manager::Init_Cloth(const PxU32 numX, const PxU32 numZ, const PxVec3& position, const PxReal particleSpacing, const PxReal totalClothMass)
//{
//	//if (nullptr == m_pCudaContextManager)
//	//	return E_FAIL;
//
//	//const PxU32 numParticles = numX * numZ;
//	//const PxU32 numSprings = (numX - 1) * (numZ - 1) * 4 + (numX - 1) + (numZ - 1);
//	//const PxU32 numTriangles = (numX - 1) * (numZ - 1) * 2;
//
//	//const PxReal restOffset = particleSpacing;
//
//	//const PxReal stretchStiffness = 10000.f;
//	//const PxReal shearStiffness = 100.f;
//	//const PxReal springDamping = 0.001f;
//
//	//// Material setup
//	//PxPBDMaterial* defaultMat = m_Physics->createPBDMaterial(0.8f, 0.05f, 1e+6f, 0.001f, 0.5f, 0.005f, 0.05f, 0.f, 0.f);
//
//	//PxPBDParticleSystem* particleSystem = m_Physics->createPBDParticleSystem(*m_pCudaContextManager);
//	//m_pParticleSystem = particleSystem;
//
//	//// General particle system setting
//
//	//const PxReal particleMass = totalClothMass / numParticles;
//	//particleSystem->setRestOffset(restOffset);
//	//particleSystem->setContactOffset(restOffset + 0.02f);
//	//particleSystem->setParticleContactOffset(restOffset + 0.02f);
//	//particleSystem->setSolidRestOffset(restOffset);
//	//particleSystem->setFluidRestOffset(0.0f);
//
//
//
//	//m_pScene->addActor(*particleSystem);
//
//	//// Create particles and add them to the particle system
//	//const PxU32 particlePhase = particleSystem->createPhase(defaultMat, PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | PxParticlePhaseFlag::eParticlePhaseSelfCollide));
//
//	//PxParticleClothBufferHelper* clothBuffers = PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, m_pCudaContextManager);
//
//	//PxU32* phase = m_pCudaContextManager->allocPinnedHostBuffer<PxU32>(numParticles);
//	//PxVec4* positionInvMass = m_pCudaContextManager->allocPinnedHostBuffer<PxVec4>(numParticles);
//	//PxVec4* velocity = m_pCudaContextManager->allocPinnedHostBuffer<PxVec4>(numParticles);
//
//	//PxReal x = position.x;
//	//PxReal y = position.y;
//	//PxReal z = position.z;
//
//	//// Define springs and triangles
//	//PxArray<PxParticleSpring> springs;
//	//springs.reserve(numSprings);
//	//PxArray<PxU32> triangles;
//	//triangles.reserve(numTriangles * 3);
//
//	//for (PxU32 i = 0; i < numX; ++i)
//	//{
//	//	for (PxU32 j = 0; j < numZ; ++j)
//	//	{
//	//		const PxU32 index = i * numZ + j;
//
//	//		PxVec4 pos(x, y, z, 1.0f / particleMass);
//	//		phase[index] = particlePhase;
//	//		positionInvMass[index] = pos;
//	//		velocity[index] = PxVec4(GI->RandomFloat(-10.f, 10.f));
//
//	//		if (i > 0)
//	//		{
//	//			PxParticleSpring spring = { id(i - 1, j, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
//	//			springs.pushBack(spring);
//	//		}
//	//		if (j > 0)
//	//		{
//	//			PxParticleSpring spring = { id(i, j - 1, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
//	//			springs.pushBack(spring);
//	//		}
//
//	//		if (i > 0 && j > 0)
//	//		{
//	//			PxParticleSpring spring0 = { id(i - 1, j - 1, numZ), id(i, j, numZ), PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
//	//			springs.pushBack(spring0);
//	//			PxParticleSpring spring1 = { id(i - 1, j, numZ), id(i, j - 1, numZ), PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
//	//			springs.pushBack(spring1);
//
//	//			//Triangles are used to compute approximated aerodynamic forces for cloth falling down
//	//			triangles.pushBack(id(i - 1, j - 1, numZ));
//	//			triangles.pushBack(id(i - 1, j, numZ));
//	//			triangles.pushBack(id(i, j - 1, numZ));
//
//	//			triangles.pushBack(id(i - 1, j, numZ));
//	//			triangles.pushBack(id(i, j - 1, numZ));
//	//			triangles.pushBack(id(i, j, numZ));
//	//		}
//
//	//		z += particleSpacing;
//	//	}
//	//	z = position.z;
//	//	x += particleSpacing;
//	//}
//
//	//PX_ASSERT(numSprings == springs.size());
//	//PX_ASSERT(numTriangles == triangles.size() / 3);
//
//	//clothBuffers->addCloth(0.0f, 0.0f, 0.0f, triangles.begin(), numTriangles, springs.begin(), numSprings, positionInvMass, numParticles);
//
//	//ExtGpu::PxParticleBufferDesc bufferDesc;
//	//bufferDesc.maxParticles = numParticles;
//	//bufferDesc.numActiveParticles = numParticles;
//	//bufferDesc.positions = positionInvMass;
//	//bufferDesc.velocities = velocity;
//	//bufferDesc.phases = phase;
//
//	//const PxParticleClothDesc& clothDesc = clothBuffers->getParticleClothDesc();
//	//PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(m_pCudaContextManager);
//
//
//	//clothPreProcessor->partitionSprings(clothDesc, m_Cloth);
//	//clothPreProcessor->release();
//
//	//m_pClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, m_Cloth, m_pCudaContextManager);
//	//
//	//m_pParticleSystem->addParticleBuffer(m_pClothBuffer);
//
//	//clothBuffers->release();
//	//defaultMat->release();
//
//
//	//m_pCudaContextManager->freePinnedHostBuffer(positionInvMass);
//	//m_pCudaContextManager->freePinnedHostBuffer(velocity);
//	//m_pCudaContextManager->freePinnedHostBuffer(phase);
//
//	return S_OK;
//}





HRESULT CPhysX_Manager::Reset_PhysX()
{
	for (auto iter : m_StaticObjects)
	{
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_StaticObjects.clear();

	for (auto iter : m_DynamicObjects)
	{
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_DynamicObjects.clear();


	for (auto iter : m_GroundObjects)
	{
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_GroundObjects.clear();

	return S_OK;
}

void CPhysX_Manager::Free()
{
	for (auto iter : m_StaticObjects)
	{		
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_StaticObjects.clear();

	for (auto iter : m_DynamicObjects)
	{
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_DynamicObjects.clear();


	for (auto iter : m_GroundObjects)
	{
		for (_uint i = 0; i < iter.second.size(); ++i)
		{
			m_pScene->removeActor(*iter.second[i].pActor);
			Safe_Release(iter.second[i].pObject);
			iter.second[i].pActor->release();
		}
		iter.second.clear();
	}
	m_GroundObjects.clear();

	for (auto iter : m_Controllers)
		iter.second->release();

	m_pController_Manager->release();
	

	// m_pParticleSystem->removeParticleBuffer(m_pClothBuffer);
	// m_pScene->removeActor(*m_pParticleSystem);
	// m_pParticleSystem->release();


	PxCloseExtensions();
	if (m_WorldMaterial)
		m_WorldMaterial->release();

	if (m_Dispatcher)
		m_Dispatcher->release();


	

	if (m_pPvd)
		m_pPvd->release();

	if (m_pTransport)
		m_pTransport->release();

	if (m_pScene)
		m_pScene->release();

	if (m_Physics)
		m_Physics->release();

	if (m_pCudaContextManager)
		m_pCudaContextManager->release();

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

bool CPhysX_Manager::filter(const PxController& a, const PxController& b)
{
	return false;
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
	
	vector<PxContactPairPoint> PairPoints;
	PairPoints.resize(pairs->contactCount);
	pairs->extractContacts(PairPoints.data(), pairs->contactCount);


	if (nullptr == pairHeader.actors[0] || nullptr == pairHeader.actors[1])
		return;

	PxActor* pLeftActor = pairHeader.actors[0];
	PxActor* pRightActor = pairHeader.actors[1];

	if (true == GI->Is_Compare(pLeftActor->getName(), "Collider"))
	{
		if (true == GI->Is_Compare(pRightActor->getName(), "Collider"))
		{
			CCollider* pLeftCollider = static_cast<CCollider*>(pLeftActor->userData);
			CCollider* pRightCollider = static_cast<CCollider*>(pRightActor->userData);

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
	}
}

void CPhysX_Manager::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	PxActor* pLeftActor = pairs->triggerActor;
	PxActor* pRightActor = pairs->otherActor;
	if (nullptr == pLeftActor || nullptr == pRightActor)
		return;


}

void CPhysX_Manager::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	int i = 0;
}








