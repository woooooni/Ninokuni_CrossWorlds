#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_SpinBomb final : public CGlanixState_Base
{
private:
	CGlanixState_SpinBomb(CStateMachine* pStateMachine);
	virtual ~CGlanixState_SpinBomb() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CGlanixState_SpinBomb* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

