#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Berserk final : public CGlanixState_Base
{
private:
	CGlanixState_Berserk(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Berserk() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CGlanixState_Berserk* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

