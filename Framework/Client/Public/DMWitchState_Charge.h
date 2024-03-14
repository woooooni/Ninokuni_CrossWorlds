#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_Charge final : public CDMWitchState_Base
{
private:
	CDMWitchState_Charge(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fChargeSpeed = 0.f;

public:
	static CDMWitchState_Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

