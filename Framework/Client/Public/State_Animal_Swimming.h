#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Swimming : public CState_Animals
{
private:
	explicit CState_Animal_Swimming(class CStateMachine* pMachine);
	virtual ~CState_Animal_Swimming() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float CalculateAngle(const Vec3& v1, const Vec3& v2);

private:
	Matrix m_OriginWorld;

	_float m_fAngle = 0.0f;
	_bool m_bResetMaxDistance = false;
	
	_float m_fMaxDistance = 0.0f;

public:
	static CState_Animal_Swimming* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

