#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_RageOffsetJump final : public CGlanixState_Base
{
private:
	CGlanixState_RageOffsetJump(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageOffsetJump() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

public:
	static CGlanixState_RageOffsetJump* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

