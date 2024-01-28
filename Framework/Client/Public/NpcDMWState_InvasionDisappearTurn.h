#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcDMWState_InvasionDisappearTurn final : public CNpcState_Base
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
	Vec4 m_vDestLook = {};
	_float m_fTurnSpeed = 0.f;

public:
	static CNpcDMWState_InvasionDisappearTurn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

