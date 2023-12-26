#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Idle : public CState_Animals
{
private:
	explicit CState_Animal_Idle(class CStateMachine* pMachine);
	virtual ~CState_Animal_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_bool m_bExit = false;

public:
	static CState_Animal_Idle* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

class State_Animal_Idle
{
};

END

