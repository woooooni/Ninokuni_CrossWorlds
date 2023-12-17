#pragma once

#include "GameObject.h"
BEGIN(Engine)

class CShader;
class CRenderer;
class CTransform;
class CModel;
class CRigidBody;

END
BEGIN(Client)
class CCharacter_Dummy final : public CGameObject
{

private:
	CCharacter_Dummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCharacter_Dummy(const CCharacter_Dummy& rhs);
	virtual ~CCharacter_Dummy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

	

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Colliders();

private:
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	

public:
	static CCharacter_Dummy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

