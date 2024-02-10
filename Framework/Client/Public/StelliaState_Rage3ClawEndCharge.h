#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3ClawEndCharge final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3ClawEndCharge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3ClawEndCharge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	// 시작하자마자 브레이크 밟는거 방지
	Vec4 m_vStartPos;
	_float m_fCurChargeLength = 0.f;
	_float m_fMinChargeLength = 0.f;

public:
	static CStelliaState_Rage3ClawEndCharge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

