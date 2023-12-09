#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer_Manager : public CBase
{
public:
	
	DECLARE_SINGLETON(CTimer_Manager)

private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	/* 타임델타를 얻어온다. */
	/* 타임델타를 계산한다. */
	_float Compute_TimeDelta(TIMER_TYPE eTimerType);


public:
	HRESULT Set_TimeScale(TIMER_TYPE eTimerType, _float fTimeScale);
	_float Get_TimeScale(TIMER_TYPE eTimerType);

	_float Get_TimeDelta(TIMER_TYPE eTimerType);
	HRESULT Set_Slow(TIMER_TYPE eTimerType, _float fSlowTime, _float fTimeScale, _bool bForce);

public:
	HRESULT			Add_Timer(TIMER_TYPE eTimerType);
	class CTimer* Find_Timer(TIMER_TYPE eTimerType);



private:
	map<TIMER_TYPE, class CTimer*>		m_Timers;



public:
	virtual void	Free(void);
};

END