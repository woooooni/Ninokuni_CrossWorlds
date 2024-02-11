#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CEnemy_Biplane_Feather final : public CVehicleFlying_Projectile
{
private:
	explicit CEnemy_Biplane_Feather(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEnemy_Biplane_Feather(const CEnemy_Biplane_Feather& rhs);
	virtual ~CEnemy_Biplane_Feather() = default;

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

public:
	virtual void Enter_Scene() override;
	virtual void Return_Pool() override;

private:
	_float m_fAccEffect = 0.f;
	_float m_fGenEffectTime = 0.5f;

private:
	class CParticle* m_pParticle = nullptr;
	class CTransform* m_pTargetTransform = nullptr;

public:
	static CEnemy_Biplane_Feather* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

