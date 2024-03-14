#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Character.h" // 후에 플레이어로 교체

BEGIN(Client)

class CGameNpc;
class CNpcState_Base abstract : public CState
{
protected:
	CNpcState_Base(CStateMachine* pStateMachine);
	virtual ~CNpcState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	_bool State_Wait(_bool bIsLoop, wstring strAnimName, _float fDestTime, _float fTimeDelta);

protected:
	CCharacter* m_pPlayer = nullptr; // 후에 플레이어로 교체.
	CTransform* m_pPlayerTransform = nullptr;

	CGameNpc* m_pNpc = nullptr;

private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
