#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_RageReturn final : public CGlanixState_Base
{
private:
	CGlanixState_RageReturn(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageReturn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

	_bool	m_bIsInit = false;

public:
	static CGlanixState_RageReturn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

