#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_SnowBall final : public CGlanixState_Base
{
private:
	CGlanixState_SnowBall(CStateMachine* pStateMachine);
	virtual ~CGlanixState_SnowBall() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsRoll = false;

public:
	static CGlanixState_SnowBall* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

