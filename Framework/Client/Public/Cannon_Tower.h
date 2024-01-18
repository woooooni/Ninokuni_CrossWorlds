#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CCannon_Tower final : public CGameObject
{
private:
	CCannon_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCannon_Tower(const CCannon_Tower& rhs);
	virtual ~CCannon_Tower() = default;

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
	void Look_For_Target(_bool bEnemy);

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();
	virtual void On_Damaged(const COLLISION_INFO& tInfo);	


protected:
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	// 포신과 받침으로 구성.
	class CModel* m_pBarrelModelCom = nullptr;
	class CModel* m_pBaseModelCom = nullptr;

	class CStateMachine* m_pStateCom = nullptr;


public:
	static CCannon_Tower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
