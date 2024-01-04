#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcState_Idle final : public CNpcState_Base
{
private:
	CNpcState_Idle(CStateMachine* pStateMachine);
	virtual ~CNpcState_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CNpcState_Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

