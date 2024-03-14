#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CKuu;

class CCamera_Action final : public CCamera
{
public:
	enum CAMERA_ACTION_TYPE { 
		LOBBY, DOOR, TALK, WINDMILL, 
		SWORDMAN_BURST, ENGINEER_BURST, DESTROYER_BURST, 
		STADIUM, ENDING, WITCH_INVASION, WITCH_ROAR, WITCH_AWAY, TOWER_DEFENSE,
		STELLIA_ROAR, STELLIA_DEAD, GLANIX_DEAD, STELLIA_GUARD,
		CAMERA_ACTION_END };

public:
	typedef struct tagActionLobbyDesc
	{
		const _float fLerpTime		= 1.7f;

		const Vec4 vCamPosition		= { 3.12f, 2.23f, -16.f, 1.f };

		const Vec4 vCamLookAtStart	= { -0.3f, 0.9f, 0.745f, 1.f };
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
		enum PROGRESS	{ READY, TALKING, FINISH };
		enum VIEW_TYPE	{ KUU, KUU_AND_PLAYER, NPC, ALL_RIGTH, ALL_LEFT, KUU_AND_PLAYER_FROM_BACK_NPC, NPC_FROM_BACK_KUU_AND_PLAYER };

		_bool bInit = false;
		_bool bSet = false;

		_bool	bFinalBoss = false;

		Vec4 vPrevLookAt;

		CTransform* pTransform_Kuu		= nullptr;
		CTransform* pTransform_Player	= nullptr;
		CTransform* pTransformNpc		= nullptr;

		CKuu* pKuu = nullptr;

		void Clear()
		{
			pTransformNpc = nullptr;
			bSet = false;
			bFinalBoss = false;
		}

	}ACTION_TALK_DESC;

	typedef struct tagWindMillDesc
	{
		Vec4			vOffset			= {0.f, 0.f, 0.f, 1.f};
		const wstring	strWindMillName	= L"Evermore_Wind_WindMillaA_02";
		const _float	fLerpDuration	= 2.f;
		_bool			bNpcToWindMill	= false;  /* NPC -> 풍차 여부 */
		LERP_VEC4_DESC	tLookAt			= {};
		_float			fPrevFov		= 0.f;
		_float			fAcc			= 0.f;
		const _float	fWaitTime		= 1.f;
	}ACTION_WINDMILL_DESC;

	typedef struct tagSwordManBurstDesc
	{
		_float			fAcc = 0.f;
		_float			fFinishTime = 1.f;

		_float			fCurFov = 0.f;
		_float			fDestFov = 40.f;

		LERP_FLOAT_DESC tLerpDesc = {};

		Vec4			vCamStartPosition = {};
		class CTransform* pSwordManTransform = nullptr;
	} ACTION_SWORDMAN_BURST_DESC;

	typedef struct tagDestroyerBurstDesc
	{
		_float			fAcc = 0.f;
		_float			fFinishTime = 1.f;

		class CTransform* pDestroyerTransform = nullptr;
		class CModel* pDestroyerModel = nullptr;
	} ACTION_DESTROYER_BURST_DESC;

	typedef struct tagEngineerBurstDesc
	{
		_float			fAcc = 0.f;
		_float			fFinishTime = 1.f;

		class CTransform* pEngineerTransform = nullptr;
	} ACTION_ENGINEER_BURST_DESC;

	typedef struct tagStadiumDesc
	{
		/* Time */
		_float fAcc = 0.f;
		_float fDurationPerView = 0.f;
	
		// 이전 토크 이벤트로 돌아가기 위한 데이터
		_float	fPrevTalkFov		= 0.f;
		Vec4	vPrevTalkLookAt		= {};
		Vec4	vPrevTalkPos		= {};


		// View ist 
		enum VIEW_NUM { V0_WALL, V1_WALL, V2_TRACK, V3_TRACK, V4_FINAL, VIEW_NUM_END };

		const Vec4 ViewPositions[VIEW_NUM::VIEW_NUM_END]
		{
			Vec4(-149.46f, -4.64f, 221.92f, 1.f),
			Vec4(-107.68f, -1.423f, 245.62f, 1.f),
			Vec4(-122.34f, 4.97f, 208.01f, 1.f),
			Vec4(-101.64f, 2.97f, 214.74f, 1.f),
			Vec4(-165.41f, 12.29f, 259.22, 1.f)
		};

		const Vec4 ViewLooks[VIEW_NUM::VIEW_NUM_END]
		{
			Vec4(0.895f, 0.144f, 0.421, 0.f).Normalized(),
			Vec4(-0.638f, 0.092f, -0.763f, 0.f).Normalized(),
			Vec4(-0.234f, -0.22f, 0.946f, 0.f).Normalized(),
			Vec4(-0.681f, -0.435, 0.587f, 0.f).Normalized(),
			Vec4(0.571f, -0.423f, -0.702f, 0.f).Normalized()
		};

		_uint iCurViewNum = V0_WALL;

	}ACTION_STADIUM_DESC;

	typedef struct tagEndingDesc
	{
		_bool bActive = false;

		enum VIEW_NUM { V0, V1, V2, VIEW_NUM_END };

		/* Blending */
		const Vec3 ViewPositions[VIEW_NUM::VIEW_NUM_END]
		{
			Vec3(-3.763f, 23.654f, 125.07f),
			Vec3(-0.931f, 28.05f, 132.64f),
			Vec3(-2.254f, 27.192f, 126.271f),
		};

		const Vec3 ViewLooks[VIEW_NUM::VIEW_NUM_END]
		{
			Vec3(0.869f, -0.213f, -0.4465f).Normalized(),
			Vec3(0.513f, -0.219f, -0.829f).Normalized(),
			Vec3(0.756f, -0.428f, -0.493f).Normalized(),
		};

		const _float    fLerpDuration = 6.f;
		const LERP_MODE eLerpMode = LERP_MODE::SMOOTHER_STEP;
		const _float	fTargetFov = Cam_Fov_Default;

		LERP_VEC3_DESC	tLerpPos = {};
		LERP_VEC3_DESC	tLerpLook = {};

		/* Height Move*/
		LERP_FLOAT_DESC tLerpHeight;
		_bool bUp = false;
		const _float fHeightDelta = 1.f;
		const _float fHeightLerpDuration = 4.f;
		const LERP_MODE eHeightLerpMode = LERP_MODE::SMOOTHER_STEP;
		_float fOriginHeight = 0;

	}ACTION_ENDING_DESC;

	typedef struct tagTalkBackUpDesc
	{
		Vec4 vOriginLookAt = {};
		Vec4 vOriginPosition = {};

	}ACTION_TALK_BACKUP_DESC;

	typedef struct tagWitchInvasionDesc
	{
		_uint iBoneNumber = 0;
		class CGameObject* pWitchObject = nullptr;

		Vec4 vLookAtOffset = { 0.5f, -1.f, 0.f, 1.f };

	}ACTION_WITCH_INVASION_DESC;

	typedef struct tagWitchRoarDesc
	{
		_uint iBoneNumber = 1;
		class CGameObject* pWitchObject = nullptr;

		_bool	bZoomIn = false;

		LERP_VEC4_DESC tTargetOffset = {};
		LERP_VEC4_DESC tLookAtOffset = {};
		const _float fOriginDist = 3.f;

		_uint iCount = 0;

	}ACTION_WITCH_ROAR_DEAC;

	typedef struct tagWitchAwayDesc
	{
		_uint iBoneNumber = 1;
		class CGameObject* pWitchObject = nullptr;

		Vec4 vPervLookAt = {};
	}ACTION_WITCH_AWAY_DESC;

	typedef struct tagTowerDefenseGateDesc
	{
		enum VIEW_NUM { CENTER, LEFT, RIGHT, VIEW_NUM_END };

		_uint iCurViewIndex		= CENTER;

		_float fAcc = 0.f;
		const _float fLimitPerView = 2.f;

		Vec4 vPositions[VIEW_NUM_END] =
		{
			Vec4{-4.29f, 1.418f, 11.02f, 1.f},
			Vec4{98.f, 0.56f, 115.f, 1.f},
			Vec4{-101.3f, 4.25f, 115.91f, 1.f}
		};

		Vec4 vLooks[VIEW_NUM_END] =
		{
			Vec4{0.333f, 0.1218f, -0.935f, 0.f}.Normalized(),
			Vec4{0.78f, 0.2f, -0.58f, 0.f}.Normalized(),
			Vec4{0.8f, -0.262f, -0.53914f, 0.f}.Normalized()
		};


	}ACTION_TOWER_DEFENSE_DESC;

	typedef struct tagStelliaRoarDesc
	{
		const _uint iBoneNum = 3;

		const Vec4 vPosition = { 160.f, 3.87f, 132.8f, 1.f };

		CGameObject* pStellia = nullptr;

		const Vec4 vLookAtOffset = { 0.f, 3.4f, 4.f, 1.f };

		const _float fDeltaLookAtHeight = 2.f;
		_float	fOriginLookAtHeight = 0.f;

		_bool bLower = true;
		_bool bSet = false;

	}ACTION_STELLIA_ROAR_DESC;

	typedef struct tagStelliaDeadDesc
	{
		const _uint iBoneNum = 3;

		CGameObject* pStellia = nullptr;

	}ACTION_STELLIA_DEAD_DESC;

	typedef struct tagGlanixDeadDesc
	{
		const _uint iBoneNum = 0;

		CGameObject* pGlanix = nullptr;

	}ACTION_GLANIX_DEAD_DESC;

	typedef struct tagStelliaGuardDesc
	{
		enum class PROGRESS_TYPE { BLENDING_IN, ING, BLENDING_OUT, TYPEEND };
		enum class VIEW_TYPE { EAST, WEST, TYPEEND }; /* 플레잉 위치가 남, 스텔리아 위치가 북 기준*/

		PROGRESS_TYPE	eCurProgress = PROGRESS_TYPE::TYPEEND;
		VIEW_TYPE		eCurViewType = VIEW_TYPE::EAST; /* 누적되므로 Clear() 하면 안됨 */

		const Vec4 vTargetOffset = { 0.f, 4.f, 0.f, 1.f };
		const Vec4 vLookAtOffset = { 0.f, 2.f, 0.f, 1.f };
		Vec4 vOriginLookAt = {};

		const _float fDist = 5.f;
		const _float fFov = XMConvertToRadians(65.0f);

		CTransform* pStelliaTransform = nullptr;

		Vec4 vPrevLookAt = {};

		_bool bFirstIn = false;

		_bool bFinishBlending = false;

		void Clear()
		{
			eCurProgress = PROGRESS_TYPE::TYPEEND;

			pStelliaTransform = nullptr;

			vPrevLookAt = {};

			bFirstIn = false;

			bFinishBlending = false;
		}

	}ACTION_STELLIA_GUARD_DESC;

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
	HRESULT Start_Action_WindMill(const _bool& bNpcToWindMill); /* 현재 대화 룩 타겟이 NPC일 때 호출*/

	HRESULT Start_Action_Talk(CGameObject* pNpc, const _bool& bFinalBoss = false); /* 처음 대화 시작시 호출 (쿠우 혼자면 nullptr, Npc 있으면 Npc 넘겨줌 */
	HRESULT Change_Action_Talk_Object(const ACTION_TALK_DESC::VIEW_TYPE& eType); /* 중간 화자 변경시 호출 */
	HRESULT Finish_Action_Talk(const CAMERA_TYPE& eNextCameraType = CAMERA_TYPE::FOLLOW); /* 대화 종료시 호출 */	

	HRESULT Start_Action_Stadium(const _float& fDuration);
	HRESULT Finish_Action_Stadium();

	HRESULT Start_Action_Ending();

	HRESULT Ready_Action_Witch_Invasion(CGameObject* pGameObject);
	HRESULT Start_Action_Witch_Invasion();

	HRESULT Ready_Action_Witch_Roar(CGameObject* pGameObject);
	HRESULT Start_Action_Witch_Roar();

	HRESULT Start_Action_Witch_Away(CGameObject* pGameObject);

	HRESULT Start_Action_TowerDefense();

	HRESULT Start_Action_Stellia_Roar(CGameObject* pGameObject);
	HRESULT Start_Action_Stellia_Dead(CGameObject* pGameObject);

	HRESULT Start_Action_Glanix_Dead(CGameObject* pGameObject);

	HRESULT Start_Action_Stellia_Guard(CTransform* pStelliaTransform);
	HRESULT Finish_Action_Stellia_Guard(const _bool& bBlending);

	// 캐릭터 버스트 스킬 액션.
	HRESULT Start_Action_SwordManBurst(class CTransform* pSwordManTransform);
	HRESULT Start_Action_EngineerBurst(class CTransform* pEngineerTransform);
	HRESULT Start_Action_DestroyerBurst(class CTransform* pDestroyerTransform);
	HRESULT Stop_ActionSwordMan_Burst();
	HRESULT Stop_ActionEngineer_Burst();
	HRESULT Stop_ActionDestroyer_Burst();

	void Set_TalkBackupDesc(class CTransform* pNpcTransform);
	void Set_NpcTransformByBackupDesc(class CTransform* pNpcTransform);

	virtual void Set_Blending(const _bool& bBlending) override;

public:
	const _bool& Is_Finish_Action() const { return m_bAction; }
	virtual Vec4 Get_LookAt() override;
	const CAMERA_ACTION_TYPE& Get_Camera_ActionType() const { return m_eCurActionType; }

private:
	void Tick_Lobby(_float fTimeDelta);
	void Tick_Door(_float fTimeDelta);
	void Tick_Talk(_float fTimeDelta);
	void Tick_WindMill(_float fTimeDelta);
	void Tick_Stadium(_float fTimeDelta);
	void Tick_Ending(_float fTimeDelta);
	void Tick_Criminal(_float fTimeDelta);
	void Tick_Witch_Invasion(_float fTimeDelta);
	void Tick_Witch_Roar(_float fTimeDelta);
	void Tick_Witch_Away(_float fTimeDelta);
	void Tick_TowerDefense(_float fTimeDelta);
	void Tick_Stellia_Roar(_float fTimeDelta);
	void Tick_Stellia_Dead(_float fTimeDelta);
	void Tick_Glanix_Dead(_float fTimeDelta);
	void Tick_Stellia_Guard(_float fTimeDelta);

	void Tick_SwordManBurst(_float fTimeDelta);
	void Tick_EngineerBurst(_float fTimeDelta);
	void Tick_DestroyerBurst(_float fTimeDelta);

private:
	void Set_Talk_Transform(const ACTION_TALK_DESC::VIEW_TYPE& eType);

private:
	void Test(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;

private:
	_bool					m_bAction = false;
	CAMERA_ACTION_TYPE		m_eCurActionType = CAMERA_ACTION_TYPE::CAMERA_ACTION_END;

	ACTION_LOBBY_DESC			m_tActionLobbyDesc			= {};
	ACTION_DOOR_DESC			m_tActionDoorDesc			= {};
	ACTION_TALK_DESC			m_tActionTalkDesc			= {};
	ACTION_WINDMILL_DESC		m_tActionWindMillDesc		= {};
	ACTION_STADIUM_DESC			m_tActionStadiumDesc		= {};
	ACTION_ENDING_DESC			m_tActionEndingDesc			= {};
	ACTION_TALK_BACKUP_DESC		m_tActionTalkBackUpDesc		= {};
	ACTION_WITCH_INVASION_DESC	m_tActionWitchInvasionDesc	= {};
	ACTION_WITCH_ROAR_DEAC		m_tActionWitchRoarDesc		= {};
	ACTION_WITCH_AWAY_DESC		m_tActionWitchAwayDesc		= {};
	ACTION_TOWER_DEFENSE_DESC	m_tActionTowerDefenseDesc	= {};
	ACTION_STELLIA_ROAR_DESC	m_tActionStelliaRoarDesc	= {};
	ACTION_STELLIA_DEAD_DESC	m_tActionStelliaDeadDesc	= {};
	ACTION_GLANIX_DEAD_DESC		m_tActionGlanixDeadDesc		= {};
	ACTION_STELLIA_GUARD_DESC	m_tActionStelliaGuardDesc	= {};

	ACTION_SWORDMAN_BURST_DESC m_tActionSwordManBurstDesc	= {};
	ACTION_ENGINEER_BURST_DESC m_tActionEngineerBurstDesc	= {};
	ACTION_DESTROYER_BURST_DESC m_tActionDestroyerBurstDesc = {};

public:
	static CCamera_Action* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END