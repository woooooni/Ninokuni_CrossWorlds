#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Charge final : public CStelliaState_Base
{
private:
	CStelliaState_Charge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	LERP_FLOAT_DESC m_tChargeLerp = {};
	_float m_fChargeSpeed = 0.f;

public:
	static CStelliaState_Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

