#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_SwingDown final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_START_WARNING,
		// ------------------------------
		TYPE_E_SWING_HANDCIRCLE_RIGHT,

		TYPE_P_SWING_SMOKE_01,
		TYPE_P_SWING_CIRCLES_01,

		TYPE_P_SWING_SMOKE_02,
		TYPE_P_SWING_CIRCLES_02,

		TYPE_P_SWING_SMOKE_03,
		TYPE_P_SWING_CIRCLES_03,

		TYPE_P_SWING_SMOKE_04,
		TYPE_P_SWING_CIRCLES_04,

		TYPE_P_SWING_SMOKE_05,
		TYPE_P_SWING_CIRCLES_05,

		TYPE_P_SWING_SMOKE_06,
		TYPE_P_SWING_CIRCLES_06,

		TYPE_P_SWING_SMOKE_07,
		TYPE_P_SWING_CIRCLES_07,

		TYPE_P_SWING_SMOKE_08,
		TYPE_P_SWING_CIRCLES_08,
		// ------------------------------
		TYPE_D_ATTACK_WARNING,
		TYPE_E_ATTACK_HANDCIRCLE_LEFT,
		TYPE_D_ATTACK_CRACK,
		TYPE_E_ATTACK_SPRINGUP,
		TYPE_P_ATTACK_SMOKE,
		TYPE_P_ATTACK_CIRCLES,
		TYPE_E_ATTACK_CIRCLELINE,
		// ------------------------------
		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_SwingDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_SwingDown(const CVfx_Glanix_Skill_SwingDown& rhs);
	virtual ~CVfx_Glanix_Skill_SwingDown() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Glanix_Skill_SwingDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END