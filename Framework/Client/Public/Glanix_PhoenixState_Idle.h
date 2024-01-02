#pragma once

#include "Glanix_PhoenixState_Base.h"

BEGIN(Client)

class CGlanix_PhoenixState_Idle final : public CGlanixPhoenixState_Base
{
private:
	CGlanix_PhoenixState_Idle(CStateMachine* pStateMachine);
	virtual ~CGlanix_PhoenixState_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CGlanix_PhoenixState_Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

