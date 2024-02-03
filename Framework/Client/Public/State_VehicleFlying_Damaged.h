#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_VehicleFlying_Damaged : public CState_Vehicle
{
private:
	CState_VehicleFlying_Damaged(class CStateMachine* pMachine);
	~CState_VehicleFlying_Damaged() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fAccSpeed = 10.f;
	_float m_fAccRadialBlurPower = 0.1f;

private:
	class CVehicle_Flying* m_pFlying_Vehicle = nullptr;

public:
	static CState_VehicleFlying_Damaged* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END