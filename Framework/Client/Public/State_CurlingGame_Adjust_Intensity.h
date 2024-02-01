#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Adjust_Intensity final : public CState_CurlingGame_Base
{
	typedef struct tagGuageDesc
	{
		_bool				bActive = false;

		LERP_FLOAT_DESC		tLerpValue = {};
		_bool				bIncrease = true;

		const _float		fLerpTime = 1.f;
		const LERP_MODE		eLerpMode = LERP_MODE::EASE_IN;

		const _float		fMinValue = 0.f;
		const _float		fMaxValue = 1.f;

		const _float		fMaxPower = 70.f;

		void Start()
		{
			Reset();

			bActive = true;

			tLerpValue.Start(fMinValue, fMaxValue, fLerpTime, eLerpMode);
		}

		void Stop()
		{
			tLerpValue.bActive = false;
		}

		void Tick(const _float fTimeDelta)
		{
			if (tLerpValue.bActive)
			{
				tLerpValue.Update(fTimeDelta);
				if (!tLerpValue.bActive)
				{
					if (bIncrease)
						tLerpValue.Start(fMaxValue, fMinValue, fLerpTime, eLerpMode);
					else
						tLerpValue.Start(fMinValue, fMaxValue, fLerpTime, eLerpMode);

					bIncrease = !bIncrease;
				}
			}
		}

		void Reset()
		{
			tLerpValue.Clear();

			bIncrease = true;
		}

	}GUAGE_DESC;

private:
	CState_CurlingGame_Adjust_Intensity(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Adjust_Intensity() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

private:
	GUAGE_DESC				m_tGuageDesc = {};

public:
	static CState_CurlingGame_Adjust_Intensity* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
