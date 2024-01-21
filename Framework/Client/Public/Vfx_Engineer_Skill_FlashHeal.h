#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_FlashHeal final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 0

		TYPE_ET1_P_LIGHT, // 8
		TYPE_ET1_P_CROSSSGLITTER,
		TYPE_ET1_P_CIRCLES,
		// -------------------------
		TYPE_ET2_E_AURA_01, // 22
		TYPE_ET2_E_AURA_02,
		TYPE_ET2_E_AURA_03,
		TYPE_ET2_E_AURA_04,
		TYPE_ET2_E_AURA_05,
		TYPE_ET2_E_AURA_06,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_FlashHeal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_FlashHeal(const CVfx_Engineer_Skill_FlashHeal& rhs);
	virtual ~CVfx_Engineer_Skill_FlashHeal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_FlashHeal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END