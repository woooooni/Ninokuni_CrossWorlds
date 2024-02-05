#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Rage2Sphere final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Rage2Sphere(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Rage2Sphere() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iCount = 0;
	_bool m_bIsAttack = false;

	_float m_fAccPlayerAtkTime = 0.f;
	_float m_fPlayerAtkTime = 0.f;

	_float m_fAccAllAtkTime = 0.f;
	_float m_fAllAtkTime = 0.f;

public:
	static CNpcDMWState_Rage2Sphere* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

