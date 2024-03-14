#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_IntroIdle final : public CGlanixState_Base
{
private:
	CGlanixState_IntroIdle(CStateMachine* pStateMachine);
	virtual ~CGlanixState_IntroIdle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float	m_fTime = 0.f;
	_float	m_fWaitTime = 0.f;

	_float	m_fFadeInTime = 0.f;
	_float	m_fFadeOutTime = 0.f;

	_bool	m_bFadeIn = false;

public:
	static CGlanixState_IntroIdle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

