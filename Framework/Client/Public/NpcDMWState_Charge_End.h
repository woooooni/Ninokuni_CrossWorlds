#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Charge_End final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Charge_End(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Charge_End() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsMove = false;
	Vec4 m_vDestPos = {};

	_bool m_bIsUp = false;
	LERP_FLOAT_DESC m_tUpLerp = {};
	_float m_fUpTime = 0.f;
	_float m_fDestY = 0.f;

	LERP_VEC4_DESC m_tMoveLerp = {};
	_float m_fMoveTime = 0.f;

public:
	static CNpcDMWState_Charge_End* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

