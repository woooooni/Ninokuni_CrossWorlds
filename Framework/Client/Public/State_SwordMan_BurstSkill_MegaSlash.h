#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_BurstSkill_MegaSlash : public CState_Character
{

private:
	CState_SwordMan_BurstSkill_MegaSlash(class CStateMachine* pMachine);
	~CState_SwordMan_BurstSkill_MegaSlash() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_SwordMan_BurstSkill_MegaSlash* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END