#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Charge_Ready final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Charge_Ready(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Charge_Ready() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsMove = false;
	Vec4 m_vDestPos = {};
	_float m_fDist = 0.f;

	LERP_VEC4_DESC m_tMoveLerp = {};
	_float m_fMoveTime = 0.f;

	_bool m_bIsDown = false;
	LERP_FLOAT_DESC m_tDownLerp = {};
	_float m_fDownTime = 0.f;

public:
	static CNpcDMWState_Charge_Ready* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

