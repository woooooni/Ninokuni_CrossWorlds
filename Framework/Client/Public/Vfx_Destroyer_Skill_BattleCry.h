#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_BattleCry final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_CIRCLE, // 0
		// --------------------
		TYPE_ET2_P_SPARCKE, // 11
		TYPE_ET2_E_CIRCLELINE,
		// --------------------
		TYPE_ET3_E_DOME, // 13

		TYPE_ET3_D_FIRECRACK,
		TYPE_ET3_E_SPRINGUP,
		TYPE_ET3_E_CIRCLELINE,
		TYPE_ET3_P_STONE,
		TYPE_ET3_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_BattleCry(const CVfx_Destroyer_Skill_BattleCry& rhs);
	virtual ~CVfx_Destroyer_Skill_BattleCry() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Destroyer_Skill_BattleCry* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END