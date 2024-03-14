#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_ChaseJumpStamp final : public CVfx
{
private:
	enum TYPE {
		// JumpStamp -----------
		TYPE_D_WARNING, // 0

		TYPE_E_RISING_00, // 35 -> 29
		TYPE_E_RISING_01,

		TYPE_D_DECAL_00, // 90 -> 60
		TYPE_E_SPRINGUP,
		TYPE_E_SHOCK_00,
		TYPE_E_SHOCK_01,
		TYPE_E_SMOKE_00,

		TYPE_P_SMOKE, // 91 -> 61
		TYPE_P_CIRCLES,
		// ---------------------

		// ClawRight -----------
		TYPE_E_TRAIL_01,   // 22 -> 75
		TYPE_P_CIRCLES_01, // 23

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_ChaseJumpStamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_ChaseJumpStamp(const CVfx_Stellia_Skill_ChaseJumpStamp& rhs);
	virtual ~CVfx_Stellia_Skill_ChaseJumpStamp() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pWarningDecal = nullptr;
	class CEffect* m_pSpringUpEffect = nullptr;

public:
	static CVfx_Stellia_Skill_ChaseJumpStamp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END