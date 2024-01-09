#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_CounterStart final : public CGlanixState_Base
{
private:
	CGlanixState_CounterStart(CStateMachine* pStateMachine);
	virtual ~CGlanixState_CounterStart() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool  m_bIsSlow = false;
	_float m_fTime = 0.f;
	_float m_fSlowTime = 0.f;

public:
	static CGlanixState_CounterStart* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

