#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
class CModel;
END

BEGIN(Client)
class CCurlingGame_Stone;
class CManager_StateMachine;

class CCurlingGame_Manager : public CBase
{
	DECLARE_SINGLETON(CCurlingGame_Manager)

	enum CURLINGGAME_STATE { INTRO, MOVE, DIRECTION, INTENSITY, LAUNCH, CURLINGGAME_STATE_TYPEEND };
	enum PARTICIPANT_TYPE  { PARTICIPANT_PLAYER, PARTICIPANT_NPC, PARTICIPANT_TYPEEND };

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
		const _uint		iPoints[RING_TYPE::RING_TYPEEND]					= { 5, 3, 1 };

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
		_uint	iNumStone	= 10;

		_uint	iScore = 0;

		CGameObject* pOwner			= nullptr;
		CModel* pModelCom			= nullptr;
		CTransform* pTransformCom	= nullptr;
	
	}PARTICIPANT_INFO_DESC;

private:
	CCurlingGame_Manager();
	virtual ~CCurlingGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);
	void Render_Debug();

public:
	HRESULT Start_Game();

public:
	vector<CGameObject*>* Get_Stadium() { return &m_pStadiumObjects; }

private:
	void Tick_Score();

private:
	HRESULT Ready_Components();
	HRESULT Ready_Objects();

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
	
	STANDARD_DESC			m_tStandardDesc = {};	
	vector<CGameObject*>	m_pStadiumObjects;

	vector<CCurlingGame_Stone*> m_pBarrelsLaunched;

	PARTICIPANT_INFO_DESC	m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_TYPEEND];
	
	CCurlingGame_Stone*		m_pCurStone = nullptr;
	Vec4					m_vCurStoneLook = {};

	/* Turn */				
	_bool					m_bPlayerTurn = true;


#pragma region Debug Draw 
	const _bool	m_bDebugRender						= false;
	BasicEffect* m_pEffect							= nullptr;
	BoundingSphere* m_pSphere						= nullptr;
	ID3D11InputLayout* m_pInputLayout				= nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch	= nullptr;
#pragma endregion

public:
	virtual void Free() override;

private :
	friend class CState_CurlingGame_Intro;
	friend class CState_CurlingGame_Move_Character;
	friend class CState_CurlingGame_Choose_Direction;
	friend class CState_CurlingGame_Adjust_Intensity;
	friend class CState_CurlingGame_Launch_Stone;
};

END
