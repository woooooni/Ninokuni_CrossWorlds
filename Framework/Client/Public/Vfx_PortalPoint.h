#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_PortalPoint final : public CVfx
{
protected:
	CVfx_PortalPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_PortalPoint(const CVfx_PortalPoint& rhs);
	virtual ~CVfx_PortalPoint() = default;

public:
	void Set_LineEffectScale(Vec3 vScale) { m_vLineEffectScale = vScale; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	Vec3 m_vLineEffectScale = {};

private:
	class CEffect* pEffectObject = nullptr;
	class CParticle* pParticleObject = nullptr;

public:
	static CVfx_PortalPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END