#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "Monster.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CModel;
class CPhysX_Controller;
class CRigidBody;
END

BEGIN(Client)
class CMonsterProjectile abstract : public CGameObject
{
protected:
	CMonsterProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CMonsterProjectile(const CMonsterProjectile& rhs);
	virtual ~CMonsterProjectile() = default;

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
	virtual CMonster* Get_Owner() { return m_pOwner; }

protected:
	virtual HRESULT Ready_Components();

protected:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CPhysX_Controller* m_pControllerCom = nullptr;
	CRigidBody* m_pRigidBodyCom = nullptr;

	CMonster* m_pOwner = nullptr;

public:
	virtual void Free() override;
};

END

