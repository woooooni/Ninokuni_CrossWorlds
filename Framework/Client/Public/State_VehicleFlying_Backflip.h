#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
class CState_VehicleFlying_Backflip : public CState_Vehicle
{
private:
	CState_VehicleFlying_Backflip(class CStateMachine* pMachine);
	~CState_VehicleFlying_Backflip() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fTimeAcc = { 0.f };
	_float m_fAccumulatedRotation = { 0.f };
	_bool m_bFinished = { false };

public:
	static CState_VehicleFlying_Backflip* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END