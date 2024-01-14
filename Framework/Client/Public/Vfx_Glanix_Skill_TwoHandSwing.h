#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_TwoHandSwing final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_TRAIL_01,
		TYPE_P_SMOKE_01_01,
		TYPE_P_SMOKE_01_02,
		TYPE_P_SMOKE_01_03,
		TYPE_P_CIRCLE_01,
		TYPE_E_TRAIL_02,
		TYPE_P_SMOKE_02_01,
		TYPE_P_SMOKE_02_02,
		TYPE_P_SMOKE_02_03,
		TYPE_P_CIRCLE_02,
		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_TwoHandSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_TwoHandSwing(const CVfx_Glanix_Skill_TwoHandSwing& rhs);
	virtual ~CVfx_Glanix_Skill_TwoHandSwing() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Glanix_Skill_TwoHandSwing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END