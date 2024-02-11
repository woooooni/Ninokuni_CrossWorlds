#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage01JumpStamp final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_WARNING,

		TYPE_E_RISING_00,
		TYPE_E_RISING_01,

		TYPE_D_DECAL_00,
		TYPE_E_SPRINGUP,
		TYPE_E_SHOCK_00,
		TYPE_E_SHOCK_01,
		TYPE_E_SMOKE_00,

		TYPE_P_SMOKE,
		TYPE_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage01JumpStamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage01JumpStamp(const CVfx_Stellia_Skill_Rage01JumpStamp& rhs);
	virtual ~CVfx_Stellia_Skill_Rage01JumpStamp() = default;

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
	class CEffect* m_pRising01Effect = nullptr;
	class CEffect* m_pRising02Effect = nullptr;
	class CEffect* m_pShock01Effect = nullptr;
	class CEffect* m_pShock02Effect = nullptr;
	class CEffect* m_pSmokeEffect = nullptr;

public:
	static CVfx_Stellia_Skill_Rage01JumpStamp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END