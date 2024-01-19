#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcCriminalState_EscapeRun final : public CNpcState_Base
{
private:
	CNpcCriminalState_EscapeRun(CStateMachine* pStateMachine);
	virtual ~CNpcCriminalState_EscapeRun() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CNpcCriminalState_EscapeRun* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

