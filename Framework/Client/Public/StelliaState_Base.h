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

class CStellia;
class CStelliaState_Base abstract : public CState
{
protected:
	CStelliaState_Base(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

public:
	void	Init_Pattern();

protected:
	virtual _bool State_Wait(_float fDestTime, _float fTimeDelta);
	virtual void  Rage1_Tick(_float fTimeDelta);

	void Generate_Explosion(_uint iCount);

protected:
	CCharacter* m_pPlayer = nullptr; // 후에 플레이어로 교체.
	CTransform* m_pPlayerTransform = nullptr;

	CStellia* m_pStellia = nullptr;

	_float m_fRunSpeed = 0.f;

	/* 공격 패턴 위한 변수들 */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;

private:
	_float m_fTime = 0.f;

protected:
	// 임시 Decal, 나중에는 vfx인지 뭐시긴지 걸로 옮기지 않을까?
	class CDecal* m_pDecal = nullptr;

public:
	virtual void Free() override;
};

END
