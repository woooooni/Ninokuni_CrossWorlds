#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CDefenceInvasion_Portal final : public CGameObject
{

private:
	CDefenceInvasion_Portal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDefenceInvasion_Portal(const CDefenceInvasion_Portal& rhs);
	virtual ~CDefenceInvasion_Portal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo);
	virtual void Collision_Continue(const COLLISION_INFO& tInfo);
	virtual void Collision_Exit(const COLLISION_INFO& tInfo);

private:
	virtual HRESULT Ready_Components() override;


private:
	class CTransform* m_pTransformCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;

public:
	static CDefenceInvasion_Portal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

