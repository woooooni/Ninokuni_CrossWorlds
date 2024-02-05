#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Run : public CState_Vehicle
{
private:
	CState_EnemyBiplane_Run(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Run() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = { nullptr };
	_bool m_bUpdate = { false };
	_float m_fMovingSpeed = { 5.f };


private:
	void Move(_float fTimeDelta);

public:
	static CState_EnemyBiplane_Run* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END