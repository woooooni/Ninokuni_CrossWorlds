#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Stun final : public CGlanixState_Base
{
private:
	CGlanixState_Stun(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Stun() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	_float m_fStunTime = 0.f;

public:
	static CGlanixState_Stun* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

