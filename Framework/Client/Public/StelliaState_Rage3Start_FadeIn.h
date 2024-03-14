#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3Start_FadeIn final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3Start_FadeIn(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3Start_FadeIn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsFadeOut = false;

public:
	static CStelliaState_Rage3Start_FadeIn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

