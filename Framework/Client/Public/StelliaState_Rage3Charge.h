#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3Charge final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3Charge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	// 클릭 액션
	_int m_iClickPower = 0;
	_int m_iClickDest = 0;

	_float m_fAccClickTime = 0.f;
	_float m_fLimitTime = 0.f;

	// 클릭 액션 시 사용할 카메라 쉐이크 시간
	_float m_fAccShakeTime = 0.f;
	_float m_fShakeTime = 0.f;

	_int m_iBreakCount = 0;

	// 시작하자마자 브레이크 밟는거 방지
	Vec4 m_vStartPos;
	_float m_fCurChargeLength = 0.f;
	_float m_fMinChargeLength = 0.f;

public:
	static CStelliaState_Rage3Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

