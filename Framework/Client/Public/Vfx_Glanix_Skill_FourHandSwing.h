#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_FourHandSwing final : public CVfx
{
private:
	enum TYPE {
		// 1
		TYPE_D_DECAL_00,

		// 2
		TYPE_P_SPARKLE_01,
		TYPE_P_SMOKE_01,

		// 3
		TYPE_P_SPARKLE_02,
		TYPE_P_SMOKE_02,

		// 4
		TYPE_P_SPARKLE_03,
		TYPE_P_SMOKE_03,

		// 5
		TYPE_P_LIGHTSPARKLE_04,

		// 6
		TYPE_P_LIGHTSPARKLE_05,

		// 7
		TYPE_E_CIRCLE_LHAND_06,
		TYPE_E_CIRCLE_RHAND_06,

		// 8
		TYPE_E_TRAIL_07,
		TYPE_P_CIRCLE_07,

		// 9
		TYPE_E_TRAIL_08,
		TYPE_P_CIRCLE_08,

		// 10
		TYPE_E_TRAIL_09,
		TYPE_P_CIRCLE_09,

		// 11
		TYPE_E_TRAIL_10,
		TYPE_P_CIRCLE_10,
		
		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_FourHandSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_FourHandSwing(const CVfx_Glanix_Skill_FourHandSwing& rhs);
	virtual ~CVfx_Glanix_Skill_FourHandSwing() = default;

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

public:
	static CVfx_Glanix_Skill_FourHandSwing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END