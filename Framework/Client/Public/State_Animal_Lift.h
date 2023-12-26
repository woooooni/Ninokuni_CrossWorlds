#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Lift : public CState_Animals
{
private:
	explicit CState_Animal_Lift(class CStateMachine* pMachine);
	virtual ~CState_Animal_Lift() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fChangeMotionTime = 5.0f;
	_float m_fNextTime = 0.0f;

public:
	static CState_Animal_Lift* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

