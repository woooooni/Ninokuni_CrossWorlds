#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_RunAway : public CState_Vehicle
{
private:
	CState_EnemyBiplane_RunAway(class CStateMachine* pMachine);
	~CState_EnemyBiplane_RunAway() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = nullptr;
	_float m_fMovingSpeed = { 10.f };

private:
	void Move(_float fTimeDelta);

public:
	static CState_EnemyBiplane_RunAway* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END