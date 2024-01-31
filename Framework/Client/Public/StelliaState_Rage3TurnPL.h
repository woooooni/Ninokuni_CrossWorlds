#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3TurnPL final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3TurnPL(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3TurnPL() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	Vec4 m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

	vector<_uint> m_vecRage3AtkState;
	_int m_iRageAtkIndex = 0;
public:
	static CStelliaState_Rage3TurnPL* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END
