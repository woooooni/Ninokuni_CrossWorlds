#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Skill_2 : public CState_Vehicle
{
private:
	CState_EnemyBiplane_Skill_2(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Skill_2() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = { nullptr };
	_float m_fMovingSpeed = { 5.f };

private:
	void Shoot_Feathers();

private:
	class CCamera_Follow* m_pFollowCamera = nullptr;


public:
	static CState_EnemyBiplane_Skill_2* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END