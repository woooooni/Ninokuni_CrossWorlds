#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CSwordMan_AuraBlade final : public CCharacter_Projectile
{
	
private:
	explicit CSwordMan_AuraBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSwordMan_AuraBlade(const CSwordMan_AuraBlade& rhs);
	virtual ~CSwordMan_AuraBlade() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;


private:
	_float m_fMoveSpeed = 20.f;


public:
	static CSwordMan_AuraBlade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

