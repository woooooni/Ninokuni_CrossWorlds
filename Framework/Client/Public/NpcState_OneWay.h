#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcState_OneWay final : public CNpcState_Base
{
private:
	CNpcState_OneWay(CStateMachine* pStateMachine);
	virtual ~CNpcState_OneWay() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iCurRoamingIndex = 0; 
	_bool m_bIsMove = true;

public:
	static CNpcState_OneWay* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

