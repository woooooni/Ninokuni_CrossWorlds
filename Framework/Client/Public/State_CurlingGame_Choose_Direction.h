#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CCurlingGame_Arrow;
class CState_CurlingGame_Choose_Direction final : public CState_CurlingGame_Base
{
private:
	CState_CurlingGame_Choose_Direction(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Choose_Direction() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

private:
	void Control_Direction(const _float& fTimeDelta);

private:

	CCurlingGame_Arrow*		m_pArrow = nullptr;
	CTransform*				m_pArrowTransform = nullptr;
	_bool					m_bCloned = false;

	const _float			m_fRotSpeed = 5.f;

public:
	static CState_CurlingGame_Choose_Direction* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
