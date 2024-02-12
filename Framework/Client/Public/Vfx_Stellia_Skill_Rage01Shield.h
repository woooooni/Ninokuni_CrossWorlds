#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage01Shield final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_SHIELD,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage01Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage01Shield(const CVfx_Stellia_Skill_Rage01Shield& rhs);
	virtual ~CVfx_Stellia_Skill_Rage01Shield() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pShield = nullptr;

public:
	static CVfx_Stellia_Skill_Rage01Shield* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END