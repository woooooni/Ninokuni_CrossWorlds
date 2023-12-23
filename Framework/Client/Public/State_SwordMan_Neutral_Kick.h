#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_Neutral_Kick : public CState_Character
{

private:
	CState_SwordMan_Neutral_Kick(class CStateMachine* pMachine);
	~CState_SwordMan_Neutral_Kick() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Input(_float fTimeDelta);

private:
	_float m_fIdleMotionTime = 5.f;
	_float m_fAccIdleMotion = 0.f;

public:
	static CState_SwordMan_Neutral_Kick* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END