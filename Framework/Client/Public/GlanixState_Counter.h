#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Counter final : public CGlanixState_Base
{
private:
	CGlanixState_Counter(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Counter() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	_float m_fCounterTime = 0.f;

public:
	static CGlanixState_Counter* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

