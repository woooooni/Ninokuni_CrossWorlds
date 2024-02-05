#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Attack : public CState_Vehicle
{
private:
	CState_EnemyBiplane_Attack(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Attack() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Tick_NormalAttack0(_float fTimeDelta);
	void Tick_NormalAttack1(_float fTimeDelta);
	void Tick_NormalAttack2(_float fTimeDelta);

private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = { nullptr };
	_float m_fMovingSpeed = { 5.f };

	_int m_iPrevAttackNum = -1;

public:
	static CState_EnemyBiplane_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END