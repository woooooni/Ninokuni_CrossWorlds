#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_RageCharge final : public CGlanixState_Base
{
private:
	CGlanixState_RageCharge(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageCharge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fChargeSpeed = 0.f;
	_float m_fChargeTime = 0.f;
	_float m_fTime = 0.f;

	_int   m_iChargeCount = 0;

public:
	static CGlanixState_RageCharge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

