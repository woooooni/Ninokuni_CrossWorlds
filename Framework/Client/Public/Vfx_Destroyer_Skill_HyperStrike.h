#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_HyperStrike final : public CVfx
{
private:
	enum TYPE { // 망치에 트레일 추가 필요
		TYPE_ET1_E_CIRCLELINE,
		TYPE_ET1_P_CIRCLES,
		// --------------------
		TYPE_ET2_D_CRACK,
		TYPE_ET2_P_FIRES,
		TYPE_ET2_P_CIRCLES,
		// --------------------
		TYPE_ET3_D_CIRCLE,
		TYPE_ET3_O_HAMMER,

		TYPE_ET3_D_CRACK,
		TYPE_ET3_E_STONECRACK,
		TYPE_ET3_P_FIRES,
		TYPE_ET3_P_CIRCLES,
		TYPE_ET3_P_STONE,
		TYPE_ET3_E_CIRCLELINE,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_HyperStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_HyperStrike(const CVfx_Destroyer_Skill_HyperStrike& rhs);
	virtual ~CVfx_Destroyer_Skill_HyperStrike() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pEt3_Decal = nullptr;
	class CDestroyer_HyperStrike_Hammer* m_pHammer = nullptr;

private:
	void Shoot_Hammer();

public:
	static CVfx_Destroyer_Skill_HyperStrike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END