#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_Destruction final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_RECT, // 0

		TYPE_ET1_A_CANNON_LEFT, // 42
		TYPE_ET1_A_CANNON_RIGHT,
		TYPE_ET1_P_SMOKE,
		TYPE_ET1_E_IRON,
		// ---------------------------
		TYPE_ET2_P_ENTRANCE_LEFT, // 52
		TYPE_ET2_P_ENTRANCE_RIGHT,

		TYPE_ET2_E_TRAILLINES_LEFT,
		TYPE_ET2_E_TRAILLINES_RIGHT,

		TYPE_ET2_P_SMOKES,
		TYPE_ET2_P_FIRE,
		TYPE_ET2_P_CIRCLES,
		// ---------------------------
		TYPE_ET3_E_IRON, // 90

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_Destruction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_Destruction(const CVfx_Engineer_Skill_Destruction& rhs);
	virtual ~CVfx_Engineer_Skill_Destruction() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_Destruction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END