#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStellia;
class CStelliaState_Rage1Loop_Idle final : public CStelliaState_Base
{
private:
	CStelliaState_Rage1Loop_Idle(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage1Loop_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	CStellia::STELLIA_STATE m_eNextState = CStellia::STELLIA_RAGE1LOOP_TURN;

public:
	static CStelliaState_Rage1Loop_Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

