#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_InvasionAppear final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_InvasionAppear(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_InvasionAppear() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	LERP_FLOAT_DESC m_tLerpDesc = {};

	_float m_fDuration = 0.f;
	_float m_fDestY = 0.f;

public:
	static CNpcDMWState_InvasionAppear* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

