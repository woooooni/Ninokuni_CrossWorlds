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
	physx::PxPairFlags & retPairFlags, const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
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

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());
	PxInitExtensions(*m_Physics, nullptr);

	PxSceneDesc SceneDesc(m_Physics->getTolerancesScale());
	SceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);

	m_Dispatcher = PxDefaultCpuDispatcherCreate(0);
	if (!m_Dispatcher)
		return E_FAIL;

	SceneDesc.cpuDispatcher = m_Dispatcher;
	SceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
	SceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	SceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;


	SceneDesc.filterShader = FilterShader;
	SceneDesc.simulationEventCallback = this;
	m_pScene = m_Physics->createScene(SceneDesc);
	m_pController_Manager = PxCreateControllerManager(*m_pScene);
	m_pController_Manager->setOverlapRecoveryModule(true);

	m_WorldMaterial = m_Physics->createMaterial(0.f, 0.f, 0.f);
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	for (_uint i = 0; i < OBJ_TYPE::OBJ_END; ++i)
	{
		m_eObjectTypes.push_back(OBJ_TYPE(i));
	}
	
	return S_OK;
}
void CPhysX_Manager::Tick(_float fTimeDelta)
{
	
}
void CPhysX_Manager::LateTick(_float fTimeDelta)
{
	_float fStepSize = 1.f / 144.f;
	_float fDt = fTimeDelta;


	while (0.f < fDt)
	{
		fDt -= fStepSize;
		m_pScene->simulate(fStepSize);
		m_pScene->fetchResults(true);
	}

	
}

HRESULT CPhysX_Manager::Remove_Controller(PxController* pController)
{
	if (nullptr == pController)
		return S_OK;

	pController->release();
	return S_OK;
}

HRESULT CPhysX_Manager::Remove_Actor(class CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto iterGround = m_GroundObjects.find(pGameObject->Get_ObjectID());
	for (auto& Desc : iterGround->second)
	{
		m_pScene->checkResults(true);
		m_pScene->removeActor(*Desc.pActor);
		Safe_Release(Desc.pObject);
	}
	iterGround->second.clear();
	return S_OK;
}
void CPhysX_Manager::Is_Valid_Scene()
{
	m_pScene->checkResults(true);
}
PxController* CPhysX_Manager::Add_CapsuleController(CGameObject* pGameObject, Matrix WorldMatrix, _float fHeight, _float fRadius, _float3 vOffsetPos, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	// 컨트롤러는 하나만 가질 수 있다.
	if (nullptr == pGameObject)
		return nullptr;
	
	PxCapsuleControllerDesc CapsuleDesc;
	PxTransform pxTransform = GI->To_PxTransform(WorldMatrix);
	CapsuleDesc.setToDefault();
	CapsuleDesc.material = m_WorldMaterial;
	CapsuleDesc.position = PxExtendedVec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) + PxExtendedVec3(vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
	CapsuleDesc.height = fHeight;
	CapsuleDesc.radius = fRadius;
	CapsuleDesc.maxJumpHeight = fMaxJumpHeight;
	CapsuleDesc.stepOffset = 0.1f;
	CapsuleDesc.contactOffset = 0.0001f;
	CapsuleDesc.userData = &m_eObjectTypes[pGameObject->Get_ObjectType()];
	CapsuleDesc.reportCallback = pCallBack;

	

	PxController* pController = m_pController_Manager->createController(CapsuleDesc);
	pController->getActor()->setName("Controller");

	

	return pController;
}
PxController* CPhysX_Manager::Add_BoxController(CGameObject* pGameObject, Matrix WorldMatrix, _float3 fExtents, _float3 vOffsetPos, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack)
{
	// 컨트롤러는 하나만 가질 수 있다.
	if (nullptr == pGameObject)
		return nullptr;
	
	PxBoxControllerDesc BoxDesc;
	PxTransform pxTransform = GI->To_PxTransform(WorldMatrix);
	BoxDesc.setToDefault();
	BoxDesc.material = m_WorldMaterial;
	BoxDesc.position = PxExtendedVec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) + PxExtendedVec3(vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
	BoxDesc.halfSideExtent = fExtents.x / 2.f;
	BoxDesc.halfHeight = fExtents.y / 2.f;
	BoxDesc.halfForwardExtent = fExtents.z / 2.f;
	BoxDesc.maxJumpHeight = fMaxJumpHeight;
	BoxDesc.contactOffset = 0.0001f;
	BoxDesc.userData = &m_eObjectTypes[pGameObject->Get_ObjectType()];
	BoxDesc.reportCallback = pCallBack;

	
	PxController* pController = m_pController_Manager->createController(BoxDesc);
	pController->getActor()->setName("Controller");


	return pController;
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

		pActor->setName("Ground");
		pActor->attachShape(*pShape);
		pActor->userData = pGameObject;

		
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
HRESULT CPhysX_Manager::Add_Building(CGameObject* pGameObject, CModel* pModel, Matrix WorldMatrix, const wstring& strCollisionTag)
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
		pActor->setName("Building");
		// shape->setSimulationFilterData(PxFilterData(1, 0, 0, 0));
		pActor->attachShape(*pShape);
		pActor->userData = pGameObject;
		
		
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

HRESULT CPhysX_Manager::Reset_PhysX()
{
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


	m_pController_Manager->release();

	PxCloseExtensions();
	if (m_WorldMaterial)
		m_WorldMaterial->release();
	if (m_Dispatcher)
		m_Dispatcher->release();
	if (m_pScene)
		m_pScene->release();
	if (m_Physics)
		m_Physics->release();
	if (m_Foundation)
		m_Foundation->release();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

_bool CPhysX_Manager::Check_Push(_uint iLeftObjType, _uint iRightObjType)
{	

	if ((iLeftObjType == OBJ_MONSTER) && (iRightObjType == OBJ_MONSTER))
		return true;

	else if ((iLeftObjType == OBJ_MONSTER && iRightObjType == OBJ_CHARACTER) || (iLeftObjType == OBJ_CHARACTER && iRightObjType == OBJ_MONSTER))
		return true;

	else if ((iLeftObjType == OBJ_GRANDPRIX_ENEMY && iRightObjType == OBJ_GRANDPRIX_CHARACTER) || (iLeftObjType == OBJ_GRANDPRIX_ENEMY && iRightObjType == OBJ_GRANDPRIX_ENEMY))
		return true;



	return false;
}

// ControllerFilterCallBack
bool CPhysX_Manager::filter(const PxController& a, const PxController& b)
{
	OBJ_TYPE* pLeftObjType = static_cast<OBJ_TYPE*>(a.getUserData());
	OBJ_TYPE* pRightObjType = static_cast<OBJ_TYPE*>(b.getUserData());
	
	
	return Check_Push(*pLeftObjType, *pRightObjType);;
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
