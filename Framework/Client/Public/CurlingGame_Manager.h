#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CCurlingGame_Barrel;

class CCurlingGame_Manager : public CBase
{
	DECLARE_SINGLETON(CCurlingGame_Manager)

	enum PARTICIPANT_TYPE { PARTICIPANT_PLAYER, PARTICIPANT_NPC, PARTICIPANT_TYPEEND };

	typedef struct tagStandardDesc
	{
		/* Ring, Goal */
		enum RING_TYPE { FIRST, SECOND, THIRD, RING_TYPEEND };

		wstring wstrRingNames[RING_TYPE::RING_TYPEEND] =
		{
			L"Decal_CurlingGame_RingBoard_Red",
			L"Decal_CurlingGame_RingBoard_Yellow",
			L"Decal_CurlingGame_RingBoard_Green",
		};

		const _float fRingScales[RING_TYPE::RING_TYPEEND] =
		{
			4.f, 9.f, 19.3f /* 수치 바꾸면 z 파이팅 발생 -> 데칼이라 높이 적용 안됨 */
		};

		const _float fHeight = 3.f;

		const Vec3	vGoalPosition	= { -150.f, -5.1f, 238.f }; /* 변경시 높이만 플레이어 포지션으로 맞춰주면 됨 */

	}STANDARD_DESC;

	typedef struct tagGuageDesc
	{
		_bool				bActive		= false;

		LERP_FLOAT_DESC		tLerpValue	= {};
		_bool				bIncrease	= true;

		const _float		fLerpTime	= 1.f;
		const LERP_MODE		eLerpMode	= LERP_MODE::EASE_IN;
		
		const _float		fMinValue	= 0.f;
		const _float		fMaxValue	= 1.f;

		const _float		fMaxPower	= 60.f;

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

	typedef struct tagStaiumDesc
	{
		_bool bActive = false;
		vector<CGameObject*>	pStadiumObjects;

	}STADIUM_DESC;

	typedef struct tagParticipantInfoDesc
	{
		_int	iOwnerType	= -1;

		_uint	iNumStone	= 5;

		_uint	iScore = 0;
	
	}PARTICIPANT_INFO_DESC;

private:
	CCurlingGame_Manager();
	virtual ~CCurlingGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);

public:
	HRESULT Set_Game(const _bool& bStart);

public:
	/* Stadium */
	HRESULT Start_StadiumAction();
	HRESULT Finish_StaduimAction();

	/* UI */
	const _float& Get_GuageValue() const { return m_tGuageDesc.tLerpValue.fCurValue; }

private:
	void Tick_Guage(const _float& fTimeDelta);
	void Tick_StadiumAction(const _float& fTimeDelta);

private:
	HRESULT Ready_Objects();
	HRESULT Ready_Decal();

	void Calculate_Score();

private:
	void Test(const _float& fTimeDelta);
	void Debug();

private:
	/* Default */
	ID3D11Device*			m_pDevice	= nullptr;
	ID3D11DeviceContext*	m_pContext	= nullptr;
	_bool					m_bReserved = false;

	/* Active */
	_bool					m_bPlaying	= false;

	/* Guage */
	GUAGE_DESC				m_tGuageDesc = {};

	/* Stadium */
	STADIUM_DESC			m_tStadiumDesc = {};

	/* Barrels */
	vector<CCurlingGame_Barrel*> m_pBarrelsLaunched;

	/* Participant */
	PARTICIPANT_INFO_DESC m_tParticipants[PARTICIPANT_TYPE::PARTICIPANT_TYPEEND];

	/* Standard */ 
	STANDARD_DESC		m_tStandardDesc = {};

	/* Test */
	_bool					m_bLoadMapTest = false;

public:
	virtual void Free() override;
};

END
