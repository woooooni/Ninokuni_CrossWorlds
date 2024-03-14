#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
class CState_VehicleFlying_Stand : public CState_Vehicle
{
private:
	CState_VehicleFlying_Stand(class CStateMachine* pMachine);
	~CState_VehicleFlying_Stand() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Shoot();

private:
	class CVehicle_Flying_Biplane* m_pVehicle_Flying_Biplane = nullptr;
	class CCamera_Follow* m_pFollowCamera = nullptr;

private:
	_bool m_bShoot = { false };
	_float m_fTimeAcc = { 0.f };

private:
	_bool m_bInitVelocity = false;
	_bool m_bEngineer = false;

public:
	static CState_VehicleFlying_Stand* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END