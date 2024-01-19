#pragma once
#include "Client_Defines.h"
#include "State_DefenceTower.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_DefenceTower_Dead : public CState_DefenceTower
{

private:
	CState_DefenceTower_Dead(class CStateMachine* pMachine);
	~CState_DefenceTower_Dead() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_DefenceTower_Dead* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END