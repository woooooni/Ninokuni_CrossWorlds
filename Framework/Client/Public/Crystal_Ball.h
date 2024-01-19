#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CCrystal_Ball final : public CCharacter_Projectile
{
private:
	explicit CCrystal_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCrystal_Ball(const CCrystal_Ball& rhs);
	virtual ~CCrystal_Ball() = default;

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
	_float m_fGenEffectTime = 0.5f;

public:
	static CCrystal_Ball* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

