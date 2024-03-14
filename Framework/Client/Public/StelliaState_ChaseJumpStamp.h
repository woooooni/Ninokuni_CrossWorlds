#pragma once

#include "StelliaState_Base.h"
BEGIN(Client)

class CStelliaState_ChaseJumpStamp final : public CStelliaState_Base
{
private:
	CStelliaState_ChaseJumpStamp(CStateMachine* pStateMachine);
	virtual ~CStelliaState_ChaseJumpStamp() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

public:
	static CStelliaState_ChaseJumpStamp* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

