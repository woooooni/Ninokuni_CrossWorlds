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
class CGameNpc abstract : public CGameObject
{
public:
	enum NPC_STATE { NPC_IDLE, NPC_TALK, NPC_MOVE_ONEWAY, NPC_MOVE_TWOWAY, NPC_END };

public:
	typedef struct tagNpcStat
	{
		_uint iLevel = 1;

		_uint iAtt = 1;
		_uint iDef = 1;

		_int iHp = 100;
		_int iMaxHp = 100;

		_float fSpeed = 3.f;
	}NPC_STAT;

protected:
	CGameNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGameNpc(const CGameNpc& rhs);
	virtual ~CGameNpc() = default;

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
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite);
	_bool Is_Infinite() { return m_bInfinite; }

public:
	const NPC_STAT& Get_Stat() { return m_tStat; }
	void Set_Stat(const NPC_STAT& StatDesc) { m_tStat = StatDesc; }

	vector<_vector>* Get_RoamingArea() { return &m_vecRoaming; }
	_vector			 Get_RoamingIndex(_int iIndex) { return m_vecRoaming[iIndex]; }
	void			 Set_RoamingArea(vector<_vector> vecRoaming) { m_vecRoaming = vecRoaming; }

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

	class CWeapon* m_pWeapon = nullptr;

protected:
	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 5.f;
	_bool m_bInfinite = false;

	NPC_STAT m_tStat = {};

	vector<_vector> m_vecRoaming = {};

	///* Npc가 사용하는 AnimName 모음. */
	//wstring m_strMoveName = TEXT();
	//vector<wstring> m_vecIdleName = {};

public:
	virtual void Free() override;
};

END
