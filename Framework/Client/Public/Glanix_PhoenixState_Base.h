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

class CGlanix_Phoenix;
class CGlanixPhoenixState_Base abstract : public CState
{
protected:
	CGlanixPhoenixState_Base(CStateMachine* pStateMachine);
	virtual ~CGlanixPhoenixState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	CCharacter* m_pPlayer = nullptr; // 후에 플레이어로 교체.
	CTransform* m_pPlayerTransform = nullptr;

	CGlanix_Phoenix* m_pPhoenix = nullptr;
private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
