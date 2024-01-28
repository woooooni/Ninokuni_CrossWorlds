#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_HealingTree final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 0
		// -------------------------
		TYPE_ET2_P_LIGHT, // 8

		TYPE_ET3_E_AURA_01, // 22
		TYPE_ET3_P_CROSSSGLITTER_01,
		TYPE_ET3_P_CIRCLES_01,
		// -------------------------

		TYPE_ET4_E_AURA_02,
		// -------------------------
		TYPE_ET5_E_AURA_03,
		// -------------------------
		TYPE_ET6_E_AURA_04,
		// -------------------------
		TYPE_ET7_E_AURA_05,
		// -------------------------
		TYPE_ET8_E_AURA_06,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_HealingTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_HealingTree(const CVfx_Engineer_Skill_HealingTree& rhs);
	virtual ~CVfx_Engineer_Skill_HealingTree() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_HealingTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END