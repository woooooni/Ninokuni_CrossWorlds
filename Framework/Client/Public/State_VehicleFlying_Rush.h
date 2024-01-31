#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_VehicleFlying_Rush : public CState_Vehicle
{
private:
	CState_VehicleFlying_Rush(class CStateMachine* pMachine);
	~CState_VehicleFlying_Rush() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fAccSpeed = 10.f;
	_float m_fAccRadialBlurScale = 16.f;

public:
	static CState_VehicleFlying_Rush* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END