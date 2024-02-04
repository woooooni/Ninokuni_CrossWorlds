#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_Rage2StartJump final : public CGlanixState_Base
{
private:
	CGlanixState_Rage2StartJump(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Rage2StartJump() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};
	_bool m_bJumpEffectCreate = false;
	_bool m_bDownEffectCreate = false;

public:
	static CGlanixState_Rage2StartJump* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

