#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcState_Talk final : public CNpcState_Base
{
private:
	CNpcState_Talk(CStateMachine* pStateMachine);
	virtual ~CNpcState_Talk() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool	m_bIsTalk = true;

public:
	static CNpcState_Talk* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

