#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3Charge final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3Charge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iBreakCount = 0;
	_float m_fAccChargeTime = 0.f;
	_float m_fChargeTime = 0.f;

public:
	static CStelliaState_Rage3Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

