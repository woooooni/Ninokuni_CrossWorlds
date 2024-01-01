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
class CPhysX_Controller;
END


BEGIN(Client)
class CTrail;
class CCharacter abstract : public CGameObject
{

public:
#pragma region CHARACTER_STATES
	enum STATE { 
		NEUTRAL_IDLE, 
		NEUTRAL_WALK,
		NEUTRAL_RUN,
		NEUTRAL_JUMP,
		NEUTRAL_KICK,

		NEUTRAL_CROUCH_IDLE,
		NEUTRAL_CROUCH_MOVE,

		NEUTRAL_PICK_LARGE_IDLE,
		NEUTRAL_PICK_SMALL_IDLE,
		NEUTRAL_PICK_LARGE_WALK,
		NEUTRAL_PICK_SMALL_WALK,
		NEUTRAL_PICK_LARGE_RUN,
		NEUTRAL_PICK_SMALL_RUN,
		NEUTRAL_PICK_LARGE_THROW,
		NEUTRAL_PICK_SMALL_THROW,
		NEUTRAL_PICK_LARGE_FINISH,
		NEUTRAL_PICK_SMALL_FINISH,

		BATTLE_IDLE, 
		BATTLE_WALK,
		BATTLE_RUN,
		BATTLE_JUMP, 
		BATTLE_DASH,
		BATTLE_GUARD,

		BATTLE_ATTACK_0,
		BATTLE_ATTACK_1,
		BATTLE_ATTACK_2,
		BATTLE_ATTACK_3,

		CLASS_SKILL_0,
		CLASS_SKILL_1,
		CLASS_SKILL_2,

		SKILL_BURST,
		SKILL_SPECIAL_0,
		SKILL_SPECIAL_1,
		SKILL_SPECIAL_2,


		ABNORMALITY_STUN,

		DAMAGED_WEAK,
		DAMAGED_STRONG,
		DAMAGED_KNOCKDOWN,
		DAMAGED_IMPACT,


		DEAD,
		REVIVE,


		STATE_END
	};

#pragma endregion

#pragma region SOCKET_TYPE
enum SOCKET_TYPE { SOCKET_SWORD, SOCKET_SWEATH, SOCKET_RIGHT_HAND, SOCKET_LEFT_FOOT, SOCKET_RIGHT_FOOT, SOCKET_END };
#pragma endregion

#pragma region CHARACTER_STAT
public:
	typedef struct tagCharacterStat
	{
		_uint iLevel = 1;

		_uint iAtt = 1;
		_uint iDef = 1;

		_int iHp = 100;
		_int iMaxHp = 100;

		_int iExp = 0;
		_int iMaxExp = 100;
	}CHARACTER_STAT;

#pragma endregion
protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;


public:
	void Generate_MotionTrail(const MOTION_TRAIL_DESC& MotionTrailDesc);
	void Stop_MotionTrail();

	void Generate_Trail(SOCKET_TYPE eSocketType);
	void Stop_Trail(SOCKET_TYPE eSocketType);

public:
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite);
	_bool Is_Infinite() { return m_bInfinite; }

public:
	const CHARACTER_STAT& Get_Stat() { return m_tStat; }
	void Set_Stat(const CHARACTER_STAT& StatDesc) { m_tStat = StatDesc; }

public:
	CHARACTER_TYPE Get_CharacterType() { return m_eCharacterType; }

public:
	HRESULT Set_PartModel(PART_TYPE ePartType, class CModel* pModel)
	{
		if (ePartType >= PART_TYPE::PART_END)
			return E_FAIL;

		m_pCharacterPartModels[ePartType] = pModel;
		return S_OK;
	}

	class CModel* Get_PartModel(PART_TYPE ePartType)
	{
		if (ePartType >= PART_TYPE::PART_END)
			return nullptr;

		return m_pCharacterPartModels[ePartType];

	}

public:
	void Set_Target(class CGameObject* pTarget) { m_pTarget = pTarget; }
	class CGameObject* Get_Target() { return m_pTarget; }


public:
	HRESULT Disappear_Weapon();
	HRESULT Appear_Weapon();

public:
	virtual HRESULT Enter_Character();
	virtual HRESULT Exit_Character();

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders() PURE;
	virtual void On_Damaged(const COLLISION_INFO& tInfo);



protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CStateMachine* m_pStateCom = nullptr;
	class CNavigation* m_pNavigationCom = nullptr;
	class CPhysX_Controller* m_pControllerCom = nullptr;
	class CTrail* m_pTrails[SOCKET_END];
	class CWeapon* m_pWeapon = nullptr;
	class CModel* m_pCharacterPartModels[PART_TYPE::PART_END];

protected:
	class CGameObject* m_pTarget = nullptr;


protected:
	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 5.f;
	_bool m_bInfinite = false;

	CHARACTER_STAT m_tStat = {};
	CHARACTER_TYPE m_eCharacterType = CHARACTER_TYPE::CHARACTER_END;




	_bool m_bMotionTrail = false;
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};

private:
	void Tick_MotionTrail(_float fTimeDelta);

	


public:
	virtual void Free() override;
};

END
