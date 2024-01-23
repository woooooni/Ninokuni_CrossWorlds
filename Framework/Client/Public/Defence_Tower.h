#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CDefence_Tower abstract : public CGameObject
{

public:
	enum DEFENCE_TOWER_STATE
	{
		TOWER_STATE_PREPARE,
		TOWER_STATE_IDLE,
		TOWER_STATE_ATTACK,
		TOWER_STATE_DEAD,
		TOWER_STATE_END
	};

protected:
	CDefence_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CDefence_Tower(const CDefence_Tower& rhs);
	virtual ~CDefence_Tower() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

public:
	virtual void Fire() PURE;
	
public:
	void Look_For_Target(_bool bEnemy);


public:
	void Set_Preview(_bool bPreview) { m_bPrevObject = bPreview; }
	void Set_Install_Possible(_bool bPossible) { m_bInstallPossible = bPossible; }

public:
	_uint Get_TowerType() { return m_eTowerType; }

protected:
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

protected:
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders() PURE;
	

private:
	void Tick_Target(_float fTimeDelta);
	void Decide_Target(COLLISION_INFO tInfo);


protected:
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CModel* m_pBarrelModelCom = nullptr;
	class CModel* m_pBaseModelCom = nullptr;

	class CStateMachine* m_pStateCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;

protected:
	_bool m_bPrevObject = false;
	_bool m_bInstallPossible = true;

protected:
	Matrix m_BaseMatrix = Matrix::Identity;
	_bool m_bInitMatrix = false;

protected:
	class CGameObject* m_pTarget = nullptr;
	DEFENCE_TOWER_STATE m_eCurrentState = DEFENCE_TOWER_STATE::TOWER_STATE_END;
	
	_float m_fAccFireTime = 0.f;
	_float m_fFireTime = 1.f;

	_uint m_eTowerType = 99;

protected:
	_float m_fTargetLength = 10.f;

public:
	virtual void Free() override;
};

END
