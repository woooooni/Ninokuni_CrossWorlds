#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
class CState_Enemy_VehicleFlying_Enter : public CState_Vehicle
{
private:
	CState_Enemy_VehicleFlying_Enter(class CStateMachine* pMachine);
	~CState_Enemy_VehicleFlying_Enter() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Move(_float fTimeDelta);

private:
	_float m_fMovingSpeed = { 12.f }; // Temp
	_bool m_bUpdate = { false };
	_bool m_bSet = { false };

	_float m_fTimeAcc = { 0.f };

private:
	CVehicle_Flying* m_pVehicle_Flying = nullptr;

public:
	static CState_Enemy_VehicleFlying_Enter* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END