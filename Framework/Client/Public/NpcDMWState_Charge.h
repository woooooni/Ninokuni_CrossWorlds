#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Charge final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Charge(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fChargeSpeed = 0.f;

	_float m_fAtkCoolTime = 0.f;
	_float m_fAccAtkCoolTime = 0.f;

public:
	static CNpcDMWState_Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

