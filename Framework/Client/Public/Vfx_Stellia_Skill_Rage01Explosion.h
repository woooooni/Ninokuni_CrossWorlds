#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage01Explosion final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_PULL,
		TYPE_D_DECAL_00,
		TYPE_P_CIRCLES,
		TYPE_D_DECAL_01,

		TYPE_E_SHOCK_01,
		TYPE_E_SPRINGUP,
		TYPE_P_SMOKE,
		TYPE_E_WIND_00,
		TYPE_E_WIND_01,
		TYPE_P_EXPOLSION_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage01Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage01Explosion(const CVfx_Stellia_Skill_Rage01Explosion& rhs);
	virtual ~CVfx_Stellia_Skill_Rage01Explosion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pSmokeIn_01 = nullptr;

	class CEffect* m_pDecal01Effect = nullptr;
	class CEffect* m_pDecla02Effect = nullptr;

	class CParticle* m_pCircles  = nullptr;
	class CEffect*   m_pSpringUp = nullptr;

public:
	static CVfx_Stellia_Skill_Rage01Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END