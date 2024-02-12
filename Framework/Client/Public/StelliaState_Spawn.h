#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Spawn final : public CStelliaState_Base
{
private:
	CStelliaState_Spawn(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Spawn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float			m_fAcc = 0.f;
	const _float	m_fWaitDuration = 2.f;
	_bool			m_bFinishWait = false;

public:
	static CStelliaState_Spawn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

