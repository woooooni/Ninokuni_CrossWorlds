#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CUniqueNpcState_Run final : public CNpcState_Base
{
private:
	CUniqueNpcState_Run(CStateMachine* pStateMachine);
	virtual ~CUniqueNpcState_Run() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CUniqueNpcState_Run* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

