#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3Damage final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3Damage(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3Damage() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool  m_bIsSlow = false;
	_float m_fTime = 0.f;
	_float m_fSlowTime = 0.f;

	_float m_fStunTime = 0.f;

	_bool m_bIsTimeSlep = false;

public:
	static CStelliaState_Rage3Damage* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

