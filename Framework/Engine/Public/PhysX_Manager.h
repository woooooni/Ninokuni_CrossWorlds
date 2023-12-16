#pragma once

#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CPhysX_Manager final : public CBase, public PxSimulationEventCallback
{

	DECLARE_SINGLETON(CPhysX_Manager)

public:
	CPhysX_Manager();
	virtual ~CPhysX_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif


public:
	PxRigidStatic* Add_Static_Actor(const PHYSX_INIT_DESC& Desc);
	PxRigidDynamic* Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidStatic*> Add_Static_Mesh_Actor(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidDynamic*> Add_Dynamic_Mesh_Actor(const PHYSX_INIT_DESC& Desc);

	HRESULT Add_Ground(class CGameObject* pGroundObj);

public:
	HRESULT Reset_PhysX();

public:
	HRESULT Remove_Actor(class CGameObject* pGameObject, PxActor* pPhysXActor);
	

	

public:
	PxMaterial* Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution) {
		return m_Physics->createMaterial(fStaticFriction, fDynamicFriction, fRestitution);
	}

public:
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;


public:
	PxParticleSystem* Get_ParticleSystem() { return m_pParticleSystem; }
	PxParticleClothBuffer* Get_ClothBuffer() { return m_pClothBuffer; }
	PxCudaContext* Get_CudaContext() { return m_pCudaContextManager->getCudaContext(); }
	PxCudaContextManager* Get_CudaContext_Manager() { return m_pCudaContextManager; }


private:
	PxRigidDynamic* Create_Dynamic_Box(const PHYSX_INIT_DESC& Desc);
	PxRigidDynamic* Create_Dynamic_Sphere(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidDynamic*> Create_Dynamic_Mesh(const PHYSX_INIT_DESC& Desc);

	PxRigidStatic* Create_Static_Box(const PHYSX_INIT_DESC& Desc);
	PxRigidStatic* Create_Static_Sphere(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidStatic*> Create_Static_Mesh(const PHYSX_INIT_DESC& Desc);


private:
	HRESULT Ready_ParticleSystem();
	HRESULT Create_Cloth(class CVIBuffer* pBufferCom);
	HRESULT Create_Cloth(class CMesh* pMesh);

private:
	HRESULT Init_Cloth(const PxU32 numX, const PxU32 numZ, const PxVec3& position = PxVec3(0, 0, 0), const PxReal particleSpacing = 0.2f, const PxReal totalClothMass = 10.f);
	


#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
#endif

private:
	PxDefaultAllocator			m_Allocator;			// 메모리 관리용..?
	PxDefaultErrorCallback		m_ErrorCallback;		// 만들때 필요함.
	

	PxFoundation*				m_Foundation = nullptr;		// 기초 설정
	PxPhysics*					m_Physics = nullptr;		// 코어가 되는거 같음.

	PxDefaultCpuDispatcher*		m_Dispatcher = nullptr;		// 멀티쓰레드 관련?
	PxScene*					m_pScene = nullptr;			// 시뮬레이션 돌릴 Scene.

	PxMaterial* m_WorldMaterial = nullptr;		// 객체의 재질


	// PxParticle.
	PxCudaContextManager*       m_pCudaContextManager = nullptr;
	PxPBDParticleSystem*		m_pParticleSystem = nullptr;
	PxParticleClothBuffer*		m_pClothBuffer = nullptr;


	_bool m_bCudaGraphics = false;

	PxPvd*						m_pPvd = nullptr; // 서버
	string						m_strIPAddress = "127.0.0.1";
	int							m_iPortNumber = 5555;
	_uint						m_iTimeOutSeconds = 10;


private:
	map<_uint, vector<PHYSX_STATIC_OBJECT_DESC>> m_StaticObjects;
	map<_uint, vector<PHYSX_DYNAMIC_OBJECT_DESC>> m_DynamicObjects;
	map<_uint, vector<PHYSX_STATIC_OBJECT_DESC>> m_GroundObjects;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

		
public:
	virtual void Free() override;

};

END