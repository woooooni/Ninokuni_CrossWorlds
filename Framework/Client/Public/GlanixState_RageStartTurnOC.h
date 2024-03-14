#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_RageStartTurnOC final : public CGlanixState_Base
{
private:
	CGlanixState_RageStartTurnOC(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageStartTurnOC() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	Vec4 m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

public:
	static CGlanixState_RageStartTurnOC* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

