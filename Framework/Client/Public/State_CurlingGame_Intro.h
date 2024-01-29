#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Intro final : public CState_CurlingGame_Base
{
	typedef struct tagStaiumDesc
	{
		LERP_FLOAT_DESC			tLerHeight;
		const _float			fTargetHeight = 27.3f;
		const _float			fLerpTime = 3.f;
		const LERP_MODE			eLerpMode = LERP_MODE::SMOOTHER_STEP;

		_float					fPrevHeight = 0.f;

	}STADIUM_DESC;

private:
	CState_CurlingGame_Intro(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Intro() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

private:
	void Tick_Stadium(const _float& fTimeDelta);

private:
	HRESULT Ready_Stauium();
	HRESULT Ready_UIs();
	HRESULT Ready_Decals();
	HRESULT Ready_Cameras();
	HRESULT Ready_Characters();

private:
	HRESULT Set_Character_Transform();

private:
	/* Stadium */
	STADIUM_DESC			m_tStadiumDesc = {};

public:
	static CState_CurlingGame_Intro* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
