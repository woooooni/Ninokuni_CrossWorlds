#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_VulcanGun_Attack final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_VulcanGun_Attack(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_VulcanGun_Attack() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iAtkCount = 0;
	_bool m_bIsAttack = false;

public:
	static CNpcDMWState_VulcanGun_Attack* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

