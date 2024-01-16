#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_JumpStamp final : public CStelliaState_Base
{
private:
	CStelliaState_JumpStamp(CStateMachine* pStateMachine);
	virtual ~CStelliaState_JumpStamp() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

public:
	static CStelliaState_JumpStamp* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

