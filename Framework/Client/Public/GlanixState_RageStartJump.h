#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_RageStartJump final : public CGlanixState_Base
{
private:
	CGlanixState_RageStartJump(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageStartJump() = default;

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
	static CGlanixState_RageStartJump* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

