#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Fly : public CState_Animals
{
private:
	explicit CState_Animal_Fly(class CStateMachine* pMachine);
	virtual ~CState_Animal_Fly() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	Matrix m_RevolutionMatrix;
	Matrix m_OriginMatrix;
	_float m_fAngle = 0.1f;
public:
	static CState_Animal_Fly* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

