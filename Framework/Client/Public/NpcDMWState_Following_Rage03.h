#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Following_Rage03 final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Following_Rage03(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Following_Rage03() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fCoolTime = 0.f;
	_float m_fAccCoolTime = 0.f;

public:
	static CNpcDMWState_Following_Rage03* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

