#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_BackStep final : public CStelliaState_Base
{
private:
	CStelliaState_BackStep(CStateMachine* pStateMachine);
	virtual ~CStelliaState_BackStep() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	Vec3 m_vDestLook = {};

public:
	static CStelliaState_BackStep* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

