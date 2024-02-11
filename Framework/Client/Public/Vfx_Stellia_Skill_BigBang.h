#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_BigBang final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_SMOKE_IN_01,
		TYPE_ET1_P_CIRCLES_IN,
		TYPE_ET1_D_DECAL,

		TYPE_ET2_E_SMOKE_OUT,
		TYPE_ET2_E_SMOKE_UP,
		TYPE_ET2_P_CIRCLES_OUTUP,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_BigBang(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_BigBang(const CVfx_Stellia_Skill_BigBang& rhs);
	virtual ~CVfx_Stellia_Skill_BigBang() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect*   m_pSmokeIn_01 = nullptr;
	class CParticle* m_pCircleInParticle = nullptr;
	class CDecal*    m_pDecal = nullptr;

public:
	static CVfx_Stellia_Skill_BigBang* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END