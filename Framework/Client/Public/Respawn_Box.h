#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CRespawn_Box final : public CGameObject
{
public:
	typedef struct tagRespawnBox
	{
		Vec4 vStartPosition = { 0.f, 0.f, 0.f, 1.f };
		Vec3 vExtents = { 0.f, 0.f, 0.f };

		Vec4 vRespawnPosition = { 0.f, 0.f, 0.f, 1.f };
	} RESPAWN_DESC;
	

private:
	CRespawn_Box(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRespawn_Box(const CRespawn_Box& rhs);
	virtual ~CRespawn_Box() = default;

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
	HRESULT Ready_Collider();

private:
	Vec3 m_vExtents = { 100.f, 100.f, 100.f };
	Vec4 m_vRespawnPosition = { 0.f, 0.f, 0.f, 1.f };
private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

public:
	static CRespawn_Box* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

