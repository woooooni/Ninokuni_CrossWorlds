#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_IceThrow final : public CVfx
{
private:
	enum TYPE {
		// ---------------------------------
		TYPE_ET1_1_D_RECTWARNING,

		// ---------------------------------
		TYPE_ET2_1_P_SPARKLE,
		TYPE_ET2_1_P_SMOKE,

		TYPE_ET2_2_P_SPARKLE,
		TYPE_ET2_2_P_SMOKE,
		// ---------------------------------
		TYPE_ET3_1_D_CRACK,
		TYPE_ET3_1_P_SMOKE,
		TYPE_ET3_1_P_CIRCLES,

		TYPE_ET3_2_E_SPRINGUP,
		TYPE_ET3_2_P_CIRCLES,

		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_IceThrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_IceThrow(const CVfx_Glanix_Skill_IceThrow& rhs);
	virtual ~CVfx_Glanix_Skill_IceThrow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_bool m_bEvent_1 = false;
	_bool m_bEvent_2 = false;
	_bool m_bEvent_3 = false;

	class CEffect* m_pEv3_SpringUp = nullptr;

public:
	static CVfx_Glanix_Skill_IceThrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END