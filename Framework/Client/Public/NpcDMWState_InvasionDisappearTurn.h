#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_InvasionDisappearTurn final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_InvasionDisappearTurn(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_InvasionDisappearTurn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	LERP_FLOAT_DESC	m_fLerpHeight = {};
	_float	m_fLerpDuration = 0.f;
	LERP_MODE m_eLerpMode = LERP_MODE::TYPEEND;
	
public:
	static CNpcDMWState_InvasionDisappearTurn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

