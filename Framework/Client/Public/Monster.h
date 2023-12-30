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
	enum class MONSTER_BOOLTYPE { 
		MONBOOL_COMBAT, MONBOOL_COMBATIDLE,
		MONBOOL_ATK, MONBOOL_ATKAROUND,
		MONBOOL_STUN, MONBOOL_HIT, MONBOOL_HITANIM, MONBOOL_END };
public:
	typedef struct tagMonsterStat
	{
		_TCHAR strKorName[MAX_PATH] = {};
		_int   iLv = 0;

		_float fHp = 100.f;
		_float fMp = 0.f;

		_float fMaxHp = 100.f;
		_float fMaxMp = 10.f;
	} MONSTER_STAT;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render();
	HRESULT Render_ShadowDepth();

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc) override;

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
	CHierarchyNode* Get_Socket(const wstring& strSocketName);
	const MONSTER_STAT& Get_Stat() { return m_tStat; }
	const wstring& Get_KorName() { return m_strKorName; }

public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

	virtual _vector Get_OriginPos() { return m_vOriginPos; }

	/* Bool 정보들 */
	virtual _bool  Get_Bools(MONSTER_BOOLTYPE eType) { return m_bBools[(_uint)eType]; }
	virtual void   Set_Bools(MONSTER_BOOLTYPE eType, _bool bIsBool) { m_bBools[(_uint)eType] = bIsBool; }

	/* 주변 순회 */
	virtual vector<_vector>* Get_RoamingArea() { return &m_vecRoamingArea; }
	virtual void Add_RoamingPoint(_vector vPos) { m_vecRoamingArea.push_back(vPos); }

	/* 스턴 시간 */
	virtual _float Get_StunTime() { return m_fStunTime; }
	virtual void   Set_StunTime(_float fStunTime) { m_fStunTime = fStunTime; }

public:
	MONSTER_TYPE Get_Monster_Type() { return m_eMonsterType; }

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders() PURE;
	virtual HRESULT Ready_RoamingPoint();

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
	vector<CGameObject*>				m_Parts;
	typedef vector<CGameObject*>		PARTS;

	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	MONSTER_STAT m_tStat = {};
	wstring m_strKorName = TEXT("");

	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 0.2f;
	_bool m_bInfinite = false;

	_vector	m_vOriginPos = {}; // 최초 위치, 돌아갈 위치.
	_bool   m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_END] = { false, }; // 몬스터가 사용하는 bool모음.

	vector<_vector> m_vecRoamingArea; // 순회 포인트 배열

	_float	m_fStunTime = 0.f; // 스턴 시간

protected:
	class CTrail* m_pTrails[SOCKET_TYPE::SOCKET_END];

protected:
	_float m_fDissolveWeight = 0.f;
	MONSTER_TYPE m_eMonsterType = MONSTER_TYPE::TYPE_END;

private:
	void LookAt_DamagedObject(CGameObject* pAttacker);
	void Play_DamagedSound();


public:
	virtual void Free() override;


};

END

