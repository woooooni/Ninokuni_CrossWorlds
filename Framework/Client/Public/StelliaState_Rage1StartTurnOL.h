#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage1StartTurnOL final : public CStelliaState_Base
{
private:
	CStelliaState_Rage1StartTurnOL(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage1StartTurnOL() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float  m_fTurnSpeed = 0.f;

public:
	static CStelliaState_Rage1StartTurnOL* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

