#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3ClawEndCharge final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3ClawEndCharge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3ClawEndCharge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fAccChargeTime = 0.f;
	_float m_fChargeTime = 0.f;

public:
	static CStelliaState_Rage3ClawEndCharge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

