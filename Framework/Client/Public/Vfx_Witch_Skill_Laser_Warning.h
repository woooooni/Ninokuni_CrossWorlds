#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Skill_Laser_Warning final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_WARNING,
		TYPE_E_LASERLINE,

		TYPE_END
	};

protected:
	CVfx_Witch_Skill_Laser_Warning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Skill_Laser_Warning(const CVfx_Witch_Skill_Laser_Warning& rhs);
	virtual ~CVfx_Witch_Skill_Laser_Warning() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pWarningDecal = nullptr;

	_bool m_bIsCreate = false;

public:
	static CVfx_Witch_Skill_Laser_Warning* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END