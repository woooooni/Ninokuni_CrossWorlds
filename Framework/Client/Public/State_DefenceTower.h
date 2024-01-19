#pragma once

#include "Client_Defines.h"
#include "State.h"
#include "UI_Manager.h"
#include "Camera_Manager.h"
#include "Camera_Action.h"
#include "Camera_Follow.h"



BEGIN(Client)

class CState_DefenceTower abstract : public CState
{

protected:
	CState_DefenceTower(class CStateMachine* pStateMachine);
	virtual ~CState_DefenceTower() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;	

protected:
	class CDefence_Tower* m_pTower = nullptr;

public:
	virtual void Free() override;

};

END