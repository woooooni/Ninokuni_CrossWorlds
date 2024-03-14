#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3ClawEndBreak final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3ClawEndBreak(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3ClawEndBreak() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	LERP_FLOAT_DESC m_tChargeLerp = {};

public:
	static CStelliaState_Rage3ClawEndBreak* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

