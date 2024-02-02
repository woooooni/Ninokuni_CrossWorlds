#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CRigidBody;
class CPipeLine;
class CStateMachine;
class CNavigation;
class CHierarchyNode;
class CBehaviorTree;
class CPhysX_Controller;
END


BEGIN(Client)
class CMonster abstract : public CGameObject
{

public:
#pragma region MONSTER_STATES
	enum MONSTER_STATE {
		REGEN,
		IDLE,
		JUMP,
		ATTACK,
		BOSS_ATTACK_0,
		BOSS_ATTACK_1,
		BOSS_ATTACK_2,
		SKILL_0,
		SKILL_1,
		DASH,
		DAMAGED_BASIC,
		DAMAGED_BLOW,
		DAMAGED_AIRBORN,
		DAMAGED_BOUND,
		KNOCKDOWN,
		DIE,
		TRACE,
		DEFENCE_TRACE,
		STATE_END
	};

#pragma endregion
	enum SOCKET_TYPE { SOCKET_LEFT_FIST, SOCKET_RIGHT_FIST, SOCKET_LEFT_FOOT, SOCKET_RIGHT_FOOT, SOCKET_END };
	enum MONSTER_TYPE { NORMAL, BOSS, TYPE_END };
	enum MONSTER_INVASION_STATE { INVASION_STATE_ATTACK, INVASION_STATE_IDLE01, INVASION_STATE_IDLE02, INVASION_STATE_DEAD, INVASION_STATE_END };
	enum class MONSTER_BOOLTYPE { 
		MONBOOL_COMBAT, MONBOOL_COMBATIDLE,
		MONBOOL_ATK, MONBOOL_ATKAROUND,
		MONBOOL_BLOW, MONBOOL_AIR, MONBOOL_STUN, MONBOOL_WEAK, MONBOOL_ISHIT,
		MONBOOL_BLOWDEAD,
		MONBOOL_END };
public:
	typedef struct tagMonsterStat
	{
		_TCHAR strKorName[MAX_PATH] = {};
		_TCHAR strSubName[MAX_PATH] = {};
		
		ELEMENTAL_TYPE eElementType = ELEMENTAL_END;

		_int   iLv = 0;

		_float fHp = 100.f;
		_float fMp = 0.f;

		_float fMaxHp = 100.f;
		_float fMaxMp = 10.f;

		_int   iAtk = 0;
		_int   iDef = 0;

		_float fAirVelocity = 0.f;
		_float fAirDeadVelocity = 0.f;
	} MONSTER_STAT;

	typedef struct tagTargetDesc
	{
		CGameObject* pTarget = nullptr;
		CTransform*  pTragetTransform = nullptr;
	}TARGET_DESC;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

private:
	virtual void Search_Target(_float fTimeDelta);
	virtual void Search_Target_Tower(_float fTimeDelta);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

public:
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite)
	{
		m_bInfinite = bInfinite;
		m_fInfiniteTime = fInfiniteTime;
		m_fAccInfinite = 0.f;
	}
	_bool Is_Infinite() { return m_bInfinite; }

public:
	const MONSTER_STAT& Get_Stat() { return m_tStat; }
	const wstring& Get_KorName() { return strKorName; }
	const wstring& Get_SubName() { return strSubName; }

public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

	virtual Vec4 Get_OriginPos() { return m_vOriginPos; }

	/* Bool 정보들 */
	virtual _bool  Get_Bools(MONSTER_BOOLTYPE eType) { return m_bBools[(_uint)eType]; }
	virtual void   Set_Bools(MONSTER_BOOLTYPE eType, _bool bIsBool) { m_bBools[(_uint)eType] = bIsBool; }

	/* 주변 순회 */
	virtual vector<Vec4>* Get_RoamingArea() { return &m_vecRoamingArea; }
	virtual void Add_RoamingPoint(Vec4 vPos) { m_vecRoamingArea.push_back(vPos); }

	/* 스턴 시간 */
	virtual _float Get_StunTime() { return m_fStunTime; }
	virtual void   Set_StunTime(_float fStunTime) { m_fStunTime = fStunTime; }

	/* Target */
	virtual TARGET_DESC Get_TargetDesc() { return m_tTargetDesc; }

public:
	MONSTER_TYPE Get_Monster_Type() { return m_eMonsterType; }

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders() PURE;
	virtual HRESULT Ready_RoamingPoint();

	void Start_MonsterHittedEvent(CGameObject* pPlayer);

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CRigidBody* m_pRigidBodyCom = nullptr;
	CStateMachine* m_pStateCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;
	CBehaviorTree* m_pBTCom = nullptr;
	CPhysX_Controller* m_pControllerCom = { nullptr };
	CTexture* m_pDissoveTexture = nullptr;


protected:
	MONSTER_STAT m_tStat = {};
	wstring strKorName = TEXT("");
	wstring strSubName = TEXT("");

	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 0.2f;
	_bool m_bInfinite = false;

	Vec4	m_vOriginPos = {}; // 최초 위치, 돌아갈 위치.
	_bool   m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_END] = { false, }; // 몬스터가 사용하는 bool모음.

	vector<Vec4> m_vecRoamingArea; // 순회 포인트 배열

	_float	m_fStunTime = 0.f; // 스턴 시간

	// 마을 침공 관련
	_bool m_bIsInvasion = false;

	// 타워 디펜스 관련
	_bool  m_bIsTargetTower = false;
	_bool  m_bIsStartDefence = false;
	_bool  m_bIsInvasionMainTown = false;
	_float m_fNearDist = 0.f; // 가장 가까운 타겟 찾기.
	_float m_fDistToTree = 0.f; // 가을 할아범과의 거리
	_float m_fTargetSearchDist = 0.f;

protected:
	class CTrail* m_pTrails[SOCKET_TYPE::SOCKET_END];

	TARGET_DESC  m_tTargetDesc = {};

protected:
	MONSTER_TYPE m_eMonsterType = MONSTER_TYPE::TYPE_END;
	ELEMENTAL_TYPE m_eDamagedElemental = ELEMENTAL_TYPE::BASIC;

	// 림, 디졸브, 블룸 효과
protected:
	_float4 m_vRimLightColor = _float4(0.f, 0.f, 0.f, 0.f);
	_float  m_fRimDuration   = 0.5f;

	_bool m_bStartDissolve = false;
	_float4 m_vDissolveColor    = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal    = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight   = 0.f;
	_float  m_fDissolveSpeed    = 5.f;

	_float3 m_vBloomPower    = _float3(1.f, 1.f, 1.f);

	_bool   m_bIsRimUse = false;
	_float  m_fRimTimeAcc = 0.f;

	_bool   m_bDissolveEffect = false;
	class CParticle* m_pDissolveObject = nullptr;

protected:
	// 렌더러에 넘겨줄 애니메이션 인스턴싱 정보.
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

protected:
	void LookAt_DamagedObject(CGameObject* pAttacker);
	void Play_DamagedSound();
	void Start_RimLight();
	void Tick_RimLight(_float fTimeDelta);
	void Create_HitEffect(class CCharacter* pCharacter);

public:
	virtual void Free() override;
};

END

