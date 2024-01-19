#pragma once
#include "Client_Defines.h"
#include "State_DefenceTower.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_DefenceTower_Prepare : public CState_DefenceTower
{

private:
	CState_DefenceTower_Prepare(class CStateMachine* pMachine);
	~CState_DefenceTower_Prepare() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_DefenceTower_Prepare* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END