#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Skill_Rage02Sphere_Ready final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_SPHERE,
		TYPE_P_SMALL,
		TYPE_D_WARNING,
		TYPE_END
	};

protected:
	CVfx_Witch_Skill_Rage02Sphere_Ready(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Skill_Rage02Sphere_Ready(const CVfx_Witch_Skill_Rage02Sphere_Ready& rhs);
	virtual ~CVfx_Witch_Skill_Rage02Sphere_Ready() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pSphere = nullptr;
	class CParticle* m_pParticle = nullptr;
	class CDecal* m_pWarningDecal = nullptr;

	_bool m_bIsCreate = false;
	_bool m_bIsCreateParticle = false;

	Vec4 m_vPos = {};

	LERP_FLOAT_DESC m_tLerpDesc = {};
	_float m_fDuration = 0.5f;
	_float m_fMaxY = 1.8f;
	_float m_fDestY = 1.7f;

	_float m_fAccTime = 0.f;
	_float m_fCreateParticleTime = 1.05f;


public:
	static CVfx_Witch_Skill_Rage02Sphere_Ready* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END