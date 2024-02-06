#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CUniqueNpcState_Attack final : public CNpcState_Base
{
private:
	CUniqueNpcState_Attack(CStateMachine* pStateMachine);
	virtual ~CUniqueNpcState_Attack() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CUniqueNpcState_Attack* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

