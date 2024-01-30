#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CBiplane_Thunder_Cloud final : public CVehicleFlying_Projectile
{
private:
	CBiplane_Thunder_Cloud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBiplane_Thunder_Cloud(const CBiplane_Thunder_Cloud& rhs);
	virtual ~CBiplane_Thunder_Cloud() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

private:
	void Tick_Target(_float fTimeDelta);
	void Find_Target(_float fTimeDelta);

private:
	class CGameObject* m_pTarget = nullptr;

private:
	_float m_fAccRotation = 0.f;

private:
	_float m_fAccEffect = 0.f;
	_float m_fGenEffectTime = 0.5f;

public:
	static CBiplane_Thunder_Cloud* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

