#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CCharacter_Biplane_Bullet final : public CVehicleFlying_Projectile
{
private:
	explicit CCharacter_Biplane_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCharacter_Biplane_Bullet(const CCharacter_Biplane_Bullet& rhs);
	virtual ~CCharacter_Biplane_Bullet() = default;

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

public:
	static CCharacter_Biplane_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

