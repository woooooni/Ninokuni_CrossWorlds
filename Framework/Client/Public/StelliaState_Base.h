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

	void Reset_Transform(); // 스텔리아의 현재 트랜스폼을 초기 생성 포지션과 룩으로 되돌림

protected:
	CCharacter* m_pPlayer = nullptr; // 후에 플레이어로 교체.
	CTransform* m_pPlayerTransform = nullptr;

	CStellia* m_pStellia = nullptr;

	_float m_fRunSpeed = 0.f;
	_float m_fSprintSpeed = 0.f;
	_float m_fRage3AroundSpeed = 0.f;

	/* 스텔리아 애니메이션 원래 속도 저장용 */
	_float m_fOriginStelliaAnimSpeed = 0.f;

	/* 공격 패턴 위한 변수들 */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;
	
	/* Rage3 변수 */
	_float m_fAroundDist = 0.f;

protected:
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};

private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
