#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage01SpinTail final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_WARNING,

		TYPE_E_TRAIL_01,
		TYPE_E_TRAIL_02,

		TYPE_P_SMOKE_01,
		TYPE_P_SMOKE_02,
		TYPE_P_SMOKE_03,
		TYPE_P_SMOKE_04,
		TYPE_P_SMOKE_05,

		//TYPE_V_E_SPEED_01,
		//TYPE_V_E_SPEED_02,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage01SpinTail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage01SpinTail(const CVfx_Stellia_Skill_Rage01SpinTail& rhs);
	virtual ~CVfx_Stellia_Skill_Rage01SpinTail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pWarning = nullptr;

public:
	static CVfx_Stellia_Skill_Rage01SpinTail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END