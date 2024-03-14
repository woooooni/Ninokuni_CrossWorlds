#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Rage2StartTurnOL final : public CGlanixState_Base
{
private:
	CGlanixState_Rage2StartTurnOL(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Rage2StartTurnOL() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float  m_fTurnSpeed = 0.f;

public:
	static CGlanixState_Rage2StartTurnOL* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

