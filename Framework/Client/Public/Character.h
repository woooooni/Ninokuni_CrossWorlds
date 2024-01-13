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

		NEUTRAL_DOOR_ENTER,

		NEUTRAL_CROUCH_IDLE,
		NEUTRAL_CROUCH_MOVE,

		NEUTRAL_PICK_LARGE_ENTER,
		NEUTRAL_PICK_SMALL_ENTER,
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


		LOBBY_SIT,
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

		_float fSpeedWeight = 1.f;

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
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite) {
		m_bInfinite = bInfinite;
		m_fInfiniteTime = fInfiniteTime;
		m_fAccInfinite = 0.f;
	};
	_bool Is_Infinite() { return m_bInfinite; }

	void Set_SuperArmor(_float fSuperArmorTime, _bool bSuperArmor)
	{
		m_bSuperArmor = bSuperArmor;
		m_fSuperArmorTime = fSuperArmorTime;
		m_fAccSuperArmor = 0.f;
	}

	_bool Is_SuperArmor() { return m_bSuperArmor; }

	void Set_Input(_bool bInput) { m_bInputable = bInput; }
	_bool Is_Input() { return m_bInputable; }

public:
	const CHARACTER_STAT& Get_Stat() { return m_tStat; }
	void Set_Stat(const CHARACTER_STAT& StatDesc) { m_tStat = StatDesc; }

	void Set_Speed_Weight(_float fSpeedWeight) { m_tStat.fSpeedWeight = fSpeedWeight; }
	_float Get_Speed_Weight() { return m_tStat.fSpeedWeight; }

	void Set_Hp(_int iHp) { m_tStat.iHp = iHp; }
	_int Get_Hp() { return m_tStat.iHp; }

	void Set_MaxHp(_int iMaxHp) { m_tStat.iMaxHp = iMaxHp; }
	_int Get_MaxHp() { return m_tStat.iMaxHp; }

	void Set_Exp(_int iExp) { m_tStat.iExp = iExp; }
	_int Get_Exp() { return m_tStat.iExp; }

	void Set_Att(_int iAtt) { 
		if (iAtt < 0)
		{
			iAtt = 0;
		}
		m_tStat.iAtt = iAtt;
	}

	_uint Get_Att() { return m_tStat.iAtt; }

	void Set_Def(_int iDef) {
		if (iDef < 0)
		{
			iDef = 0;
		}
		m_tStat.iDef = iDef;
	}

	_uint Get_Def() { return m_tStat.iDef; }

	void Decrease_HP(_int iDecrease)
	{ 
		m_tStat.iHp = max(0, m_tStat.iHp - iDecrease);
	}

	void Increase_HP(_int iIncrease)
	{
		m_tStat.iHp = min(m_tStat.iMaxHp, m_tStat.iHp + iIncrease);
	}


	ELEMENTAL_TYPE Get_ElementalType() { return m_eElemental; }
	void Set_ElementalType(ELEMENTAL_TYPE eElemental) 
	{ 
		Set_Collider_Elemental(eElemental);
		m_eElemental = eElemental; 
	}


public:
	void Set_EnterLevelPosition(Vec4 vPosition);
	void Set_InitialPosition(Vec4 vPosition);

public:
	_float Get_Default_WalkSpeed() { return m_fDefaultWalkSpeed; }
	_float Get_Default_RunSpeed() { return m_fDefaultRunSpeed; }


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
	class CWeapon* Get_Weapon() { return m_pWeapon; }
	HRESULT Disappear_Weapon();
	HRESULT Appear_Weapon();

public:
	void PickUp_Target();
	void PickDown_Target();

	void Look_For_Target();

public:
	virtual HRESULT Enter_Character();
	virtual HRESULT Exit_Character();

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders() PURE;
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

protected:
	ELEMENTAL_TYPE m_eElemental = ELEMENTAL_TYPE::FIRE;
	ELEMENTAL_TYPE m_eDamagedElemental = ELEMENTAL_TYPE::BASIC;


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
	class CModel* m_pCharacterPartModels[PART_TYPE::PART_END] = {};

protected:
	class CGameObject* m_pTarget = nullptr;


protected:
	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 5.f;

	_float m_fSuperArmorTime = 5.f;
	_float m_fAccSuperArmor = 0.f;

	_bool m_bSuperArmor = false;
	_bool m_bInfinite = false;
	_bool m_bInputable = true;

	CHARACTER_STAT m_tStat = {};
	CHARACTER_TYPE m_eCharacterType = CHARACTER_TYPE::CHARACTER_END;

	_bool m_bMotionTrail = false;
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};


protected:
	_float m_fDefaultWalkSpeed = 3.f;
	_float m_fDefaultRunSpeed = 6.f;


private:
	void Tick_MotionTrail(_float fTimeDelta);
	void Tick_Target(_float fTimeDelta);
	void Decide_Target(COLLISION_INFO tInfo);
	

private:
	class CUI_World_NameTag* m_pName = { nullptr };

public:
	virtual void Free() override;
};

END
