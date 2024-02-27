﻿#pragma once

#include "Client_Defines.h"
#include "Base.h"

using Ray = SimpleMath::Ray;

BEGIN(Engine)
class CGameObject;
class CTransform;
class CModel;
END

BEGIN(Client)
class CCurlingGame_Prop;
class CCurlingGame_Stone;
class CManager_StateMachine;

class CCurlingGame_Manager : public CBase
{
	DECLARE_SINGLETON(CCurlingGame_Manager)

public:
	enum PARTICIPANT_TYPE  { PARTICIPANT_PLAYER, PARTICIPANT_NPC, PARTICIPANT_TYPEEND };

private:
	enum CURLINGGAME_STATE { INTRO, MOVE, DIRECTION, INTENSITY, LAUNCH, ENDING, CURLINGGAME_STATE_TYPEEND };
	
	typedef struct tagStandardDesc
	{
		/* Ring, Goal */
		enum RING_TYPE { FIRST, SECOND, THIRD, RING_TYPEEND };

		const wstring wstrRingNames[RING_TYPE::RING_TYPEEND] =
		{
			L"Decal_CurlingGame_RingBoard_Red",
			L"Decal_CurlingGame_RingBoard_Yellow",
			L"Decal_CurlingGame_RingBoard_Green",
		};

		/* 수치 바꾸면 z 파이팅 발생 -> 데칼이라 높이 적용 안됨 */
		const _float	fRingScalesForRender[RING_TYPE::RING_TYPEEND]		= { 4.f, 9.f, 19.3f };
		const _float	fRingScalesForDetection[RING_TYPE::RING_TYPEEND]	= { 1.9f, 4.25f, 9.25f };
		const _uint		iPoints[RING_TYPE::RING_TYPEEND]					= { 50, 30, 15 };

		const _float	fHeight = 3.f;

		const Vec3		vGoalPosition	= { -148.f, -3.4f, 237.2f }; /* 변경시 높이만 플레이어 포지션으로 맞춰주면 됨 */

		/* Start Line */
		const wstring	wstrStartLineName = L"Decal_CurlingGame_StartLine";
		const Vec3		vStartLinePosition = { -112.f, -3.4f, 226.7f };
		const Vec3		vStartLineRotation = { 0.f, 15.f, 0.f };
		const Vec3		vStartLineScale = { 0.2f, 11.f, 22.9f };

		const Vec3		vStartLook = { -0.9661f, 0.f, 0.2671f };
		const _float	vStartPosDelta = { 3.f };

	}STANDARD_DESC;

	typedef struct tagStaiumDesc
	{
		vector<CGameObject*>	pStadiumObjects;

		LERP_FLOAT_DESC			tLerHeight;
		const _float			fTargetHeight	= 27.3f;
		const _float			fLerpTime		= 3.f;
		const LERP_MODE			eLerpMode		= LERP_MODE::SMOOTHER_STEP;

		_float					fPrevHeight		= 0.f;

	}STADIUM_DESC;

	typedef struct tagParticipantInfoDesc
	{
		_uint			iScore		= 0;
		_uint			iNumStone	= 0;
		CGameObject*	pOwner		= nullptr;
	
	}PARTICIPANT_INFO_DESC;

	typedef struct tagAIPathDesc
	{
		_float	fPower		= 0.f;
		Vec4	vLaunchDir	= {};
		Vec4	vStartPoint = {};

		tagAIPathDesc() {};
		tagAIPathDesc(const _float& _fPower, const Vec4& _vStartPoint, const Vec4& _vLaunchDir)
			: fPower(_fPower), vStartPoint(_vStartPoint), vLaunchDir(_vLaunchDir) {}

		static const _bool Equal(const tagAIPathDesc& path1, const tagAIPathDesc& path2)
		{
			if (0.01f < (path1.fPower - path2.fPower))
				return false; 
			
			if (0.5f < XMConvertToDegrees(acos(XMVectorGetX(XMVector3Dot(path1.vLaunchDir.xyz(), path2.vLaunchDir.xyz())))))
				return false;

			if (0.1f < (path1.vStartPoint - path2.vStartPoint).Length())
				return false;
			
			return true;
		}

	}AI_PATH_DESC;

private:
	CCurlingGame_Manager();
	virtual ~CCurlingGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);
	void Render_Debug();

public:
	HRESULT Ready_Game();
	HRESULT Start_Game();
	HRESULT Finish_Game();

	HRESULT Change_Turn();
	HRESULT Set_AiPath();

	CGameObject* Get_CurParticipant() const { return m_pCurParticipant; }
	
	const _bool& Is_PlayerWin() const { return m_bPlayerWin; }
	const _bool& Is_FinishGame() const { return !m_bPlaying; }

public:
	vector<CGameObject*>* Get_Stadium() { return &m_pStadiumObjects; }

private:
	HRESULT Ready_Components();
	HRESULT Ready_Objects();
	HRESULT Ready_AiPathQueue();
	HRESULT Ready_EndingTransform();

private:
	void Test(const _float& fTimeDelta);
	void Debug();
	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();

private:
	ID3D11Device*			m_pDevice	= nullptr;
	ID3D11DeviceContext*	m_pContext	= nullptr;
	_bool					m_bReserved = false;

	_bool					m_bPlaying	= false;
	CManager_StateMachine*	m_pManagerStateMachineCom = nullptr;
	
	/* Data */
	STANDARD_DESC			m_tStandardDesc = {};	
	
	/* Participant*/
	PARTICIPANT_INFO_DESC	m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_TYPEEND];
	CGameObject*			m_pCurParticipant	= nullptr;
	CGameObject*			m_pPrevParticipant	= nullptr;
	
	/* Stone */
	CCurlingGame_Stone*		m_pCurStone		= nullptr;
	Vec4					m_vCurStoneLook = {};
	vector<CCurlingGame_Stone*> m_pStonesLaunched;

	/* Turn */				
	_bool					m_bPlayerTurn = false;

	/* NPC */
	queue<AI_PATH_DESC>		m_tAiPathQueue;
	vector<Vec3>			m_StartPointDeltas;
	AI_PATH_DESC			m_tCurAiPath;
	AI_PATH_DESC			m_tPrevAiPath;
	_bool					m_bPathTurn = false;

	/* Etc */
	vector<CGameObject*>	m_pStadiumObjects;

	/* Result, Ending */
	_bool					m_bPlayerWin = false;
	_bool					m_bFadeOut = false;

#ifdef NDEBUG
	const _bool	m_bDebugRender						= false;
	BasicEffect* m_pEffect							= nullptr;
	BoundingSphere* m_pSphere						= nullptr;
	ID3D11InputLayout* m_pInputLayout				= nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch	= nullptr;
#endif

public:
	virtual void Free() override;

private :
	friend class CState_CurlingGame_Intro;
	friend class CState_CurlingGame_Move_Character;
	friend class CState_CurlingGame_Choose_Direction;
	friend class CState_CurlingGame_Adjust_Intensity;
	friend class CState_CurlingGame_Launch_Stone;
	friend class CState_CurlingGame_Ending;

};

END
