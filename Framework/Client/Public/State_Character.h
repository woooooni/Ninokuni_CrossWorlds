#pragma once

#include "Client_Defines.h"
#include "Skill_Manager.h"
#include "UI_Manager.h"
#include "Camera_Manager.h"
#include "Camera_Action.h"
#include "Camera_Follow.h"
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
	virtual void Neutral_Idle_Input(_float fTimeDelta);
	virtual void Battle_Idle_Input(_float fTimeDelta);
	virtual void Pick_Idle_Input(_float fTimeDelta);
	virtual void Crouch_Idle_Input(_float fTimeDelta);

	virtual void Neutral_Walk_Input(_float fTimeDelta);
	virtual void Battle_Walk_Input(_float fTimeDelta);
	virtual void Pick_Walk_Input(_float fTimeDelta);
	virtual void Crouch_Walk_Input(_float fTimeDelta);

	virtual void Neutral_Run_Input(_float fTimeDelta);
	virtual void Battle_Run_Input(_float fTimeDelta);
	virtual void Pick_Run_Input(_float fTimeDelta);

	virtual void Skill_Input(_float fTimeDelta);
	virtual void Attack_Input(_float fTimeDelta);

	virtual _bool Tag_Input(_float fTimeDelta);
	virtual void Dead_Input(_float fTimeDelta);

	virtual void OnBoard_Input(_float fTimeDelta);
	
protected:
	virtual void Reset_Camera_WideToDeafult();

protected:
	class CCharacter* m_pCharacter = nullptr;

public:
	virtual void Free() override;

};

END