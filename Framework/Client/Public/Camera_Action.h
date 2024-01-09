#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_Action final : public CCamera
{
public:
	enum CAMERA_ACTION_TYPE { LOBBY, DOOR, TALK, CAMERA_ACTION_END };

public:
	typedef struct tagActionLobbyDesc
	{
		const _float fLerpTime		= 2.f;

		const Vec4 vCamPosition		= { 3.12f, 2.23f, -16.f, 1.f };

		const Vec4 vCamLookAtStart	= { -0.3f, 0.59f, 0.745f, 1.f };
		const Vec4 vCamLookAtFinish = { 0.13f, 0.1f, 0.969f, 1.f };

		LERP_VEC4_DESC vLerpCamLookAt;

		const _float fLookMag = 10000.f;

	}ACTION_LOBBY_DESC;

	typedef struct tagActionDoorDesc
	{
		enum PROGRESS { DELAY, INTRO, FIX, OUTTRO, PROGRESS_END };

		PROGRESS		eProgress = PROGRESS::PROGRESS_END;

		LERP_FLOAT_DESC	tLerpRotateSpeed;
		LERP_FLOAT_DESC	tLerpDistance;			// DELAY, FIX
		const _float	fDistanceLerpTime		= 1.f;

		_float			fAcc = 0.f;				// DELAY, FIX

		const _float	fMaxRotateSpeed = XMConvertToRadians(45.f);
		
		const _float	fDelayTime		= 0.75f;// DELAY
		const _float	fFixedTime		= 0.9f;	// FIX
		const _float	fBlendingTime	= 1.4f; // INTRO, OUTTRO

		Vec4			vTargetOffset = { 0.f, 1.5f, 3.5f, 1.f };
		const Vec4		vLookAtOffset = { 0.f, 1.f, 0.f, 1.f };

		void Clear()
		{
			eProgress = PROGRESS::PROGRESS_END;

			fAcc = 0.f;
		}
		
	}ACTION_DOOR_DESC;

	typedef struct tagActionTalkDesc
	{
		const _float	fPlayerNpcDistance = 4.f;

		const Vec4		vTargetOffset = { 0.7f, 1.f, 2.5f, 1.f };
		const Vec4		vLookAtOffset = { 0.f, 1.f, 0.f, 1.f };

		Vec4			vPrevLookAt; /* 블렌딩용 */
		
		CGameObject*	pNpc1	= nullptr;
		CGameObject*	pNpc2	= nullptr;

		CGameObject*	pPlayer = nullptr;
		CGameObject*	pPet	= nullptr;

		CGameObject*	pCurTalker = nullptr;

		_uint			iTalker = 0; /* 대화에 참여하는 모든 인원 수 (플레이어, 쿠우 포함)*/

		void Clear()
		{
			pNpc1		= nullptr;
			pNpc2		= nullptr;
			pPlayer		= nullptr;
			pPet		= nullptr;
			pCurTalker	= nullptr;
			iTalker = 0;
		}

	}ACTION_TALK_DESC;

private:
	CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Action(const CCamera_Action& rhs);
	virtual ~CCamera_Action() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override { return S_OK; }

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	HRESULT Start_Action_Lobby();
	HRESULT Start_Action_Door();

	/* 첫 대화 시작시, 대화에 참여하는 Npc를 모두 넘겨준다. (플레이어, 쿠우 제외) */
	/* pNpc1는 대화를 처음 시작하게 되는 Npc이다. pNpc2는 두명 이상의 Npc가 필요한 경우 넘겨준다. */
	HRESULT Start_Action_Talk(CGameObject* pNpc1, CGameObject* pNpc2 = nullptr); 

	/* 대화 도중 말하는 사람이 바뀔 때 호출 */
	HRESULT Set_Action_Talk_Target(CGameObject* pTalker);
	
	/* 마지막 다이얼로그(Npc)가 사라질 때 호출 */
	HRESULT Finish_Action_Talk();	

	
	const _bool& Is_Finish_Action() const { return m_bAction; }
	virtual Vec4 Get_LookAt() override;

private:
	void Tick_Lobby(_float fTimeDelta);
	void Tick_Door(_float fTimeDelta);
	void Tick_Talk(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;

private:
	_bool				m_bAction = false;
	CAMERA_ACTION_TYPE	m_eCurActionType = CAMERA_ACTION_TYPE::CAMERA_ACTION_END;

	ACTION_LOBBY_DESC	m_tActionLobbyDesc = {};
	ACTION_DOOR_DESC	m_tActionDoorDesc = {};
	ACTION_TALK_DESC	m_tActionTalkDesc = {};
	
public:
	static CCamera_Action* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END