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

		NEUTRAL_CROUCH_IDLE,
		NEUTRAL_CROUCH_MOVE,


		NEUTRAL_PICK_BIG,
		NEUTRAL_PICK_MIDDLE,
		NEUTRAL_PICK_SMALL,

		BATTLE_IDLE, 
		BATTLE_WALK,
		BATTLE_RUN,
		BATTLE_JUMP, 
		BATTLE_DASH,
		BATTLE_ATTACK,
		BATTLE_GUARD,

		SKILL_0,
		SKILL_1,
		SKILL_2,
		SKILL_BURST,
		SKILL_SPECIAL,

		DAMAGED_WEAK,
		DAMAGED_FORCE,
		DAMAGED_BLOW,
		DAMAGED_KNOCKBACK,
		KNOCKDOWN,
		DEAD,


		STATE_END
	};

#pragma endregion
	enum SOCKET_TYPE { SOCKET_SWORD, SOCKET_SWEATH, SOCKET_RIGHT_HAND, SOCKET_LEFT_FOOT, SOCKET_RIGHT_FOOT, SOCKET_END };

public:
	typedef struct tagCharacterStat
	{
		_float fHp = 100.f;
		_float fMp = 100.f;

		_float fMaxHp = 100.f;
		_float fMaxMp = 100.f;
	}CHARACTER_STAT;

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
	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 5.f;
	_bool m_bInfinite = false;

	CHARACTER_STAT m_tStat = {};
	CHARACTER_TYPE m_eCharacterType = CHARACTER_TYPE::CHARACTER_END;

public:
	virtual void Free() override;
};

END

