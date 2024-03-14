#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcDMWState_Idle final : public CNpcState_Base
{
private:
	CNpcDMWState_Idle(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CNpcDMWState_Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

