#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Enemy_VehicleFlying_Attack : public CState_Vehicle
{
private:
	CState_Enemy_VehicleFlying_Attack(class CStateMachine* pMachine);
	~CState_Enemy_VehicleFlying_Attack() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Shoot();

private:
	_bool m_bShoot = { false };
	_float m_fTimeAcc = { 0.f };

public:
	static CState_Enemy_VehicleFlying_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END