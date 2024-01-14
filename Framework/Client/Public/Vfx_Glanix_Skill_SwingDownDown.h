#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_SwingDownDown final : public CVfx
{
private:
	enum TYPE {
		// ------------------------------
		// ------------------------------
		// CVfx_Glanix_Skill_SwingDown
	    // ------------------------------
		// ------------------------------
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
		// ------------------------------
		// CVfx_Glanix_Skill_HandDown
		// ------------------------------
		// ------------------------------
		TYPE_D_HANDDOWN_START_WARNING,

		TYPE_P_HANDDOWN_PREACT_BIGSCIRCLE,
		TYPE_E_HANDDOWN_PREACT_CIRCLELINE,
		TYPE_P_HANDDOWN_PREACT_CIRCLES,
		TYPE_E_HANDDOWN_PREACT_HANDCIRCLELEFT,
		TYPE_E_HANDDOWN_PREACT_HANDCIRCLERIGHT,

		TYPE_D_HANDDOWN_ATTACK_CRACK,
		TYPE_E_HANDDOWN_ATTACK_SPRINGUP,
		TTPE_P_HANDDOWN_ATTACK_SMOKE,
		TYPE_P_HANDDOWN_ATTACK_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_SwingDownDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_SwingDownDown(const CVfx_Glanix_Skill_SwingDownDown& rhs);
	virtual ~CVfx_Glanix_Skill_SwingDownDown() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pHand01Effect = nullptr;
	class CEffect* m_pHand02Effect = nullptr;

	class CDecal* m_pWarningDecal = nullptr;
	class CEffect* m_SpringUpEffect = nullptr;

public:
	static CVfx_Glanix_Skill_SwingDownDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END