#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcState_TwoWay final : public CNpcState_Base
{
private:
	CNpcState_TwoWay(CStateMachine* pStateMachine);
	virtual ~CNpcState_TwoWay() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsMove = true;
	_bool m_bReverse = false;
public:
	static CNpcState_TwoWay* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

