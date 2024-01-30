#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
class CState_Enemy_Flying_Stand : public CState_Enemy
{

private:
	CState_Enemy_Flying_Stand(class CStateMachine* pMachine);
	~CState_Enemy_Flying_Stand() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_Enemy_Flying_Stand* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};
END