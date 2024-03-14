#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_SpawnIdle final : public CStelliaState_Base
{
private:
	CStelliaState_SpawnIdle(CStateMachine* pStateMachine);
	virtual ~CStelliaState_SpawnIdle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool	m_bStartFadeOut = false;

public:
	static CStelliaState_SpawnIdle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

