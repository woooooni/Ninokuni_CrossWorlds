#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Skill_BlackHole_Bomb final : public CVfx
{
private:
	enum TYPE {
		TYPE_P_SMALL,
		TYPE_P_SMOKE,

		TYPE_END
	};

protected:
	CVfx_Witch_Skill_BlackHole_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Skill_BlackHole_Bomb(const CVfx_Witch_Skill_BlackHole_Bomb& rhs);
	virtual ~CVfx_Witch_Skill_BlackHole_Bomb() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Witch_Skill_BlackHole_Bomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END