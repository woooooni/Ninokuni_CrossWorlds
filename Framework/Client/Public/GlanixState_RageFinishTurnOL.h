#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_RageFinishTurnOL final : public CGlanixState_Base
{
private:
	CGlanixState_RageFinishTurnOL(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageFinishTurnOL() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float  m_fTurnSpeed = 0.f;

public:
	static CGlanixState_RageFinishTurnOL* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END
