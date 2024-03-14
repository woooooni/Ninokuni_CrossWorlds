#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Walk : public CState_Animals
{
private:
	explicit CState_Animal_Walk(class CStateMachine* pMachine);
	virtual ~CState_Animal_Walk() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void SelectSound();

private:
	_bool m_bIsWalk = true;
	_float m_fRandom[2] = { 0.0f, 0.0f };

	Vec4 m_vRandomDir;
	_float m_fRandomDist = 0.f;

	_bool m_bExit = false;

public:
	static CState_Animal_Walk* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

