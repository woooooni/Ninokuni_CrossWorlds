#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_ElementalBlast final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_RECT,
		// -----------------------
		TYPE_ET2_E_CIRCLELINE,
		TYPE_ET2_P_CIRCLES,
		// -----------------------
		TYPE_ET3_P_FIRE_MAIN,
		TYPE_ET3_P_CIRCLES,

		TYPE_ET3_P_SMOKE,      //

		TYPE_EVENT_FIRE,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_ElementalBlast(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_ElementalBlast(const CVfx_Engineer_Skill_ElementalBlast& rhs);
	virtual ~CVfx_Engineer_Skill_ElementalBlast() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CParticle* m_pEt3_FireBig = nullptr;
	class CParticle* m_pEt3_Circles = nullptr;

public:
	static CVfx_Engineer_Skill_ElementalBlast* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END