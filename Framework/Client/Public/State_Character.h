#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_Character abstract : public CState
{

protected:
	CState_Character(class CStateMachine* pStateMachine);
	virtual ~CState_Character() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	_bool Skill_Input(_float fTimeDelta);


protected:
	class CCharacter* m_pCharacter = nullptr;

public:
	virtual void Free() override;

};

END