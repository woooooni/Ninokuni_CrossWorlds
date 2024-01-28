#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcDMWState_InvasionDisappear final : public CNpcState_Base
{
private:
	CNpcDMWState_InvasionDisappear(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_InvasionDisappear() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	_float m_fWalkTime = 0.f;

public:
	static CNpcDMWState_InvasionDisappear* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

