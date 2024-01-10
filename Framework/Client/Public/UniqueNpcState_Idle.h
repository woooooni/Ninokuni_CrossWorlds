#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CUniqueNpcState_Idle final : public CNpcState_Base
{
private:
	CUniqueNpcState_Idle(CStateMachine* pStateMachine);
	virtual ~CUniqueNpcState_Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CUniqueNpcState_Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

