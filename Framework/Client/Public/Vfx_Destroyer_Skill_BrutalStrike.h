#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_BrutalStrike final : public CVfx
{
private: // 망치에 트레일 추가 필요
	enum TYPE {
		TYPE_ET1_D_RECT, // 0
		// --------------------
		TYPE_ET2_D_SMOKED, // 12

		TYPE_ET2_P_FIRE,
		TYPE_ET2_E_SMOKELINE,
		TYPE_ET2_P_STONE,
		TYPE_ET2_P_CIRCLES,

		TYPE_ET2_P_HITUP_01,
		TYPE_ET2_P_HITUP_02,
		TYPE_ET2_P_HITUP_03,
		TYPE_ET2_P_HITUP_04,
		TYPE_ET2_P_HITUP_05,
		// --------------------
		// --------------------
		TYPE_ET3_D_SMOKED, // 27

		TYPE_ET3_P_FIRE,
		TYPE_ET3_E_SMOKELINE,
		TYPE_ET3_P_STONE,
		TYPE_ET3_P_CIRCLES,

		TYPE_ET3_P_HITUP_01,
		TYPE_ET3_P_HITUP_02,
		TYPE_ET3_P_HITUP_03,
		TYPE_ET3_P_HITUP_04,
		TYPE_ET3_P_HITUP_05,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_BrutalStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_BrutalStrike(const CVfx_Destroyer_Skill_BrutalStrike& rhs);
	virtual ~CVfx_Destroyer_Skill_BrutalStrike() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pEt1_D_RECT = nullptr;

public:
	static CVfx_Destroyer_Skill_BrutalStrike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END