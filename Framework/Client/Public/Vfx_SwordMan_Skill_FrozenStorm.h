#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_SwordMan_Skill_FrozenStorm final : public CVfx
{
private:
	enum TYPE { 
		TYPE_D_CIRCLE, TYPE_P_SMOKE, TYPE_P_XSPARKLE, TYPE_P_SPARKLE_01,
		TYPE_E_TRAILBASIC, TYPE_P_SNOW, TYPE_E_TRAILLINE, TYPE_P_SPARKLE_02, TYPE_END };

protected:
	CVfx_SwordMan_Skill_FrozenStorm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_SwordMan_Skill_FrozenStorm(const CVfx_SwordMan_Skill_FrozenStorm& rhs);
	virtual ~CVfx_SwordMan_Skill_FrozenStorm() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_SwordMan_Skill_FrozenStorm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END