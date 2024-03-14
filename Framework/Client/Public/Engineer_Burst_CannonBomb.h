#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CEngineer_Burst_CannonBomb final : public CCharacter_Projectile
{
private:
	explicit CEngineer_Burst_CannonBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEngineer_Burst_CannonBomb(const CEngineer_Burst_CannonBomb& rhs);
	virtual ~CEngineer_Burst_CannonBomb() = default;

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
	_float m_fAccEffect = 0.f;
	_bool m_bCollisionEffect = false;

private:
	_bool m_bHit = false;

	_float m_fAccOnOff = 0.f;
	_float m_fOnOffTime = 0.2f;

private:
	class CEffect* m_pSpiralEffect = nullptr;

public:
	static CEngineer_Burst_CannonBomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

