#pragma once
#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Vehicle_Jump : public CState_Vehicle
{

private:
	CState_Vehicle_Jump(class CStateMachine* pMachine);
	~CState_Vehicle_Jump() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;


private:
	_bool m_bGroundChange = false;

public:
	static CState_Vehicle_Jump* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END