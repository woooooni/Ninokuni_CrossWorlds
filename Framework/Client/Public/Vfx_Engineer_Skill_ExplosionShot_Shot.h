#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_ExplosionShot_Shot final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_O_BULLET,

		TYPE_ET1_E_CIRCLELINE,
		TYPE_ET1_P_CIRCLES,
		TYPE_ET1_P_FIRE,
		TYPE_ET1_P_SMOKE,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_ExplosionShot_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_ExplosionShot_Shot(const CVfx_Engineer_Skill_ExplosionShot_Shot& rhs);
	virtual ~CVfx_Engineer_Skill_ExplosionShot_Shot() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	void Create_Bullet();

public:
	static CVfx_Engineer_Skill_ExplosionShot_Shot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END