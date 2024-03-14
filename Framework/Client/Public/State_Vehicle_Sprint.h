#pragma once
#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Vehicle_Sprint : public CState_Vehicle
{

private:
	CState_Vehicle_Sprint(class CStateMachine* pMachine);
	~CState_Vehicle_Sprint() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fMoveSpeed = 2.f;
	_float m_fEffectAcc = 0.f;

public:
	static CState_Vehicle_Sprint* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END