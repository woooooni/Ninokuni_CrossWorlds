#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Move_Character final : public CState_CurlingGame_Base
{
	enum class NPC_STATE { IDLE, WALK, PUTDOWN, STONE_LERPING, TYPEEND };

	typedef struct tagNpcStateDesc
	{
		NPC_STATE	eState = NPC_STATE::IDLE;
		_bool		bPutDown = false;


		_float			fAcc = 0.f;
		const _float	fIdleWaitDuration = 1.f;

		Vec4			vWalkDir = {};
		const _float	fWalkSpeed = 2.f;
		const _float	fWalkDistThreshold = 0.1f;



		void Clear()
		{
			bPutDown	= false;
			eState		= NPC_STATE::IDLE;
			fAcc		= 0.f;
			vWalkDir	= {};
		}

	}NPC_STATE_DESC;

private:
	CState_CurlingGame_Move_Character(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Move_Character() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

public:
	static void Set_NpcStoneTransform();

private:
	void Tick_NpcState(const _float& fTimeDelta);

private:
	_bool m_bFirstTurn			 = true;
	_bool m_bChangeCameraToStone = false;

private:
	/* Npc */
	NPC_STATE_DESC m_tNpcStateDesc = {};


public:
	static CState_CurlingGame_Move_Character* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
