#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage03Claw final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_TRAIL_01,
		TYPE_P_CIRCLES_01,

		TYPE_V_E_SPEED,
		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage03Claw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage03Claw(const CVfx_Stellia_Skill_Rage03Claw& rhs);
	virtual ~CVfx_Stellia_Skill_Rage03Claw() = default;

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

public:
	static CVfx_Stellia_Skill_Rage03Claw* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END