#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3AroundBreak final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3AroundBreak(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3AroundBreak() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	LERP_FLOAT_DESC m_tChargeLerp = {};



public:
	static CStelliaState_Rage3AroundBreak* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

