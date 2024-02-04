#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CPhysX_Controller;
class CStateMachine;
class CRigidBody;
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
END

BEGIN(Client)

class CVehicle abstract : public CGameObject
{
public:
	typedef struct tagVehicleDesc
	{
		_float fSpeed = 10.f;

	} VEHICLE_DESC;

public:
	enum VEHICLE_STATE {
		VEHICLE_ENTER, 
		VEHICLE_IDLE, 
		VEHICLE_WALK, 
		VEHICLE_RUN,
		VEHICLE_SPRINT,
		VEHICLE_JUMP,
		VEHICLE_JUMP_START, 
		VEHICLE_JUMP_UP, 
		VEHICLE_JUMP_DOWN, 
		VEHICLE_JUMP_FINISH, 

		// 미니게임용 상태(플레이어)
		VEHICLE_RUSH,
		VEHICLE_LEFT_EVASIVE, // 왼쪽으로 회피
		VEHICLE_RIGHT_EVASIVE, // 오른쪽으로 회피.
		VEHICLE_DAMAGED,

		// 미니게임용 상태(적)
		VEHICLE_TRACE,
		VEHICLE_ATTACK,
		VEHICLE_DEAD,
		

		VEHICLE_EXIT, 
		VEHICLE_STATE_END
	};
protected:
	CVehicle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, OBJ_TYPE eObjType);
	CVehicle(const CVehicle& rhs);
	virtual ~CVehicle() = default;

public:
	_bool Is_Aboard() { return m_bOnBoard; }
	void Set_Aboard(_bool bAboard);

	_float Get_Speed() { return m_VehicleDesc.fSpeed; }
	void Set_Speed(_float fSpeed) { m_VehicleDesc.fSpeed = fSpeed; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;

public:
	class CGameObject* Get_Rider() { return m_pRider; }
	class CTransform* Get_RiderTransform() { return m_pRiderTransform; }

public:
	virtual void Ride(class CGameObject* pRider);
	virtual void Update_Rider(_float fTimeDelta);


protected:
	class CPhysX_Controller* m_pControllerCom = nullptr;
	class CStateMachine* m_pStateCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;

protected:
	VEHICLE_DESC m_VehicleDesc = {};
	_bool m_bOnBoard = { false };
	
protected:
	class CGameObject* m_pRider = nullptr;
	class CTransform* m_pRiderTransform = nullptr;
	_bool m_bUseBone = { false };
	_float m_fOffsetY = { 0.f };

protected:
	virtual HRESULT Ready_Components() override; // [TW] : 자식객체에서는 모델만 따로 클론하여 사용하세요.
	virtual HRESULT	Ready_Colliders() PURE;		// ㄴ 네;;;;;
	virtual HRESULT Ready_States() PURE;


public:
	virtual void Free() override;
};

END

