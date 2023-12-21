#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_JumpStamp final : public CGlanixState_Base
{
private:
	CGlanixState_JumpStamp(CStateMachine* pStateMachine);
	virtual ~CGlanixState_JumpStamp() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

public:
	static CGlanixState_JumpStamp* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

