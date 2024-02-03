#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_RagePull final : public CGlanixState_Base
{
private:
	CGlanixState_RagePull(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RagePull() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float fEffectTimeAcc = 0.f;

public:
	static CGlanixState_RagePull* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

