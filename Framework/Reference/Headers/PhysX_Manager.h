#pragma once

#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CPhysX_Manager final : public CBase, PxSimulationEventCallback
{

public:
	DECLARE_SINGLETON(CPhysX_Manager)

public:
	CPhysX_Manager();
	virtual ~CPhysX_Manager() = default;

public:
	HRESULT Reserve_Manager();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	HRESULT Add_Static_Actor(const PHYSX_INIT_DESC& Desc);
	HRESULT Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc);
	HRESULT Add_Ground(class CGameObject* pGroundObj);

public:
	HRESULT Remove_Actor(_uint iObjectID, PhysXRigidType eRigidType);

public:
	HRESULT Convert_Transform(class CGameObject* pObj, PxTransform& PxTransform);

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


private:
	HRESULT Create_Box(const PHYSX_INIT_DESC& Desc);
	HRESULT Create_Sphere(const PHYSX_INIT_DESC& Desc);
	HRESULT Create_Mesh(const PHYSX_INIT_DESC& Desc);

private:
	

private:
	PxDefaultAllocator			m_Allocator;			// 메모리 관리용..?
	PxDefaultErrorCallback		m_ErrorCallback;		// 만들때 필요함.
	

	PxFoundation*				m_Foundation = nullptr;		// 기초 설정
	PxPhysics*					m_Physics = nullptr;		// 코어가 되는거 같음.

	PxDefaultCpuDispatcher*		m_Dispatcher = nullptr;		// 멀티쓰레드 관련?
	PxScene*					m_pScene = nullptr;			// 시뮬레이션 돌릴 Scene.

	PxMaterial* m_WorldMaterial = nullptr;		// 객체의 재질
	PxCudaContextManager*       m_pCudaContextManager = nullptr;


	PxPvd*						m_pPvd = nullptr; // 서버
	string						m_strIPAddress = "127.0.0.1";
	int							m_iPortNumber = 5425;
	_uint						m_iTimeOutSeconds = 10;


private:
	map<_uint, PHYSX_STATIC_OBJECT_DESC> m_StaticObjects;
	map<_uint, PHYSX_DYNAMIC_OBJECT_DESC> m_DynamicObjects;


		
public:
	virtual void Free() override;





};

END