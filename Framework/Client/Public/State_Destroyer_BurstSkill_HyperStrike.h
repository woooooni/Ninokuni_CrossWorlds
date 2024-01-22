#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Destroyer_BurstSkill_HyperStrike : public CState_Character
{

private:
	CState_Destroyer_BurstSkill_HyperStrike(class CStateMachine* pMachine);
	~CState_Destroyer_BurstSkill_HyperStrike() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	HRESULT Shoot_Hammer();

private:
	_bool m_bShoot = false;

public:
	static CState_Destroyer_BurstSkill_HyperStrike* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END