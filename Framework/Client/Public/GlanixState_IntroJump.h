#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_IntroJump final : public CGlanixState_Base
{
private:
	CGlanixState_IntroJump(CStateMachine* pStateMachine);
	virtual ~CGlanixState_IntroJump() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};
	_float m_fTime = 0.f;

public:
	static CGlanixState_IntroJump* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

