#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Intro final : public CState_CurlingGame_Base
{
	typedef struct tagStaiumDesc
	{
		LERP_FLOAT_DESC			tLerHeight;

		const _float			fStartHeitht	= 21.f;
		_float					fPrevHeight		= fStartHeitht;
		const _float			fTargetHeight	= 27.3f;// -fStartHeitht;

		const _float			fLerpTime = 4.f;
		const LERP_MODE			eLerpMode = LERP_MODE::EASE_OUT;

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
	HRESULT Ready_Stadium();
	HRESULT Ready_Decals();
	HRESULT Ready_CharacterData();

	void Tick_Stadium(const _float& fTimeDelta);

	HRESULT Finish_Stadium();

private:
	HRESULT Set_CameraTransform();
	HRESULT Set_CharacterTransform();

private:
	/* Stadium */
	STADIUM_DESC			m_tStadiumDesc = {};

public:
	static CState_CurlingGame_Intro* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
