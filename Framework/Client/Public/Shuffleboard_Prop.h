#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CRigidBody;
class CTransform;
END

BEGIN(Client)

class CShuffleboard_Prop abstract : public CGameObject
{
protected:
	CShuffleboard_Prop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CShuffleboard_Prop(const CShuffleboard_Prop& rhs);
	virtual ~CShuffleboard_Prop() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Render_ShadowDepth() override;

protected:
	class CModel* m_pModelCom = nullptr;
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() PURE;

public:
	virtual void Free() override;
};

END

