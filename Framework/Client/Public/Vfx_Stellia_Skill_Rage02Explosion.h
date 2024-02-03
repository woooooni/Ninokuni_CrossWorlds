#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage02Explosion final : public CVfx
{
private:
	enum TYPE {
		TYPE_P_BOMB_X,
		TYPE_P_BOMB_Y,
		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage02Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage02Explosion(const CVfx_Stellia_Skill_Rage02Explosion& rhs);
	virtual ~CVfx_Stellia_Skill_Rage02Explosion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Stellia_Skill_Rage02Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END