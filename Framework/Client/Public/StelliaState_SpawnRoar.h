#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_SpawnRoar final : public CStelliaState_Base
{
private:
	CStelliaState_SpawnRoar(CStateMachine* pStateMachine);
	virtual ~CStelliaState_SpawnRoar() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsFadeOut = false;

public:
	static CStelliaState_SpawnRoar* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

