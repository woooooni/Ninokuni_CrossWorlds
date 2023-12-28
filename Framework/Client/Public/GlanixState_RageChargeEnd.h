#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_RageChargeEnd final : public CGlanixState_Base
{
private:
	CGlanixState_RageChargeEnd(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageChargeEnd() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fSlidingSpeed = 0.f;

public:
	static CGlanixState_RageChargeEnd* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

