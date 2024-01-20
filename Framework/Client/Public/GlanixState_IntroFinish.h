#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_IntroFinish final : public CGlanixState_Base
{
private:
	CGlanixState_IntroFinish(CStateMachine* pStateMachine);
	virtual ~CGlanixState_IntroFinish() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float	m_fFadeInTime = 0.f;
	_float	m_fFadeOutTime = 0.f;

	_bool	m_bFadeOut = false;

public:
	static CGlanixState_IntroFinish* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

