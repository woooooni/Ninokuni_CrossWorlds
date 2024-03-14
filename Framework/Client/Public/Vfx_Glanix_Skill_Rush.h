#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_Rush final : public CVfx
{
private:
	enum TYPE {
		// ---------------------------
		TYPE_ET1_1_D_RECTWARNING,
		// ---------------------------
		TYPE_ET2_1_D_CRACK_R,
		TYPE_ET2_1_P_SMOKE_R,
		TYPE_ET2_1_P_CIRCLES_R,

		TYPE_ET2_2_D_CRACK_L,
		TYPE_ET2_2_P_SMOKE_L,
		TYPE_ET2_2_P_CIRCLES_L,

		TYPE_ET2_3_D_CRACK_R,
		TYPE_ET2_3_P_SMOKE_R,
		TYPE_ET2_3_P_CIRCLES_R,
		// ---------------------------
		TYPE_ET3_1_P_SMOKE,
		TYPE_ET3_1_P_CIRCLES,
		
		TYPE_ET3_2_P_SMOKE,
		TYPE_ET3_2_P_CIRCLES,

		TYPE_ET3_3_P_SMOKE,
		TYPE_ET3_3_P_CIRCLES,

		TYPE_ET3_4_P_SMOKE,
		TYPE_ET3_4_P_CIRCLES,

		TYPE_ET3_5_P_SMOKE,
		TYPE_ET3_5_P_CIRCLES,

		TYPE_ET3_6_P_SMOKE,
		TYPE_ET3_6_P_CIRCLES,

		TYPE_ET3_7_P_SMOKE,
		TYPE_ET3_7_P_CIRCLES,

		TYPE_ET3_8_P_SMOKE,
		TYPE_ET3_8_P_CIRCLES,

		TYPE_ET3_9_P_SMOKE,
		TYPE_ET3_9_P_CIRCLES,

		TYPE_ET3_10_P_SMOKE,
		TYPE_ET3_10_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_Rush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_Rush(const CVfx_Glanix_Skill_Rush& rhs);
	virtual ~CVfx_Glanix_Skill_Rush() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	void Set_Et3_SmokeParticle(TYPE eType);
	void Set_Et3_CirclesParticle(TYPE eType);

private:
	_bool m_bEvent_1 = false;
	_bool m_bEvent_2 = false;
	_bool m_bEvent_3 = false;

public:
	static CVfx_Glanix_Skill_Rush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END