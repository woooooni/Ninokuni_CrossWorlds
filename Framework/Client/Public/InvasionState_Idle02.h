#pragma once

#include "InvasionState_Base.h"

BEGIN(Client)

class CInvasionState_Idle02 final : public CInvasionState_Base
{
private:
	CInvasionState_Idle02(CStateMachine* pStateMachine);
	virtual ~CInvasionState_Idle02() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CInvasionState_Idle02* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

