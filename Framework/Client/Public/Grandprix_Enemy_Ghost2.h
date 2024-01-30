#pragma once
#include "Grandprix_Enemy.h"

BEGIN(Client)
class CGrandprix_Enemy_Ghost2 final : public CGrandprix_Enemy
{
private:
	CGrandprix_Enemy_Ghost2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGrandprix_Enemy_Ghost2(const CGrandprix_Enemy_Ghost2& rhs);
	virtual ~CGrandprix_Enemy_Ghost2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;
	HRESULT Ready_Sockets();

public:
	static CGrandprix_Enemy_Ghost2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

