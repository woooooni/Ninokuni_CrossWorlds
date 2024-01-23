#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_IgnorePain final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_BARRIER,
		TYPE_ET2_P_SPARCKE,

		TYPE_EV_DISSOLVE,

		TYPE_END
	};

protected:
	CVfx_Destroyer_Skill_IgnorePain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_IgnorePain(const CVfx_Destroyer_Skill_IgnorePain& rhs);
	virtual ~CVfx_Destroyer_Skill_IgnorePain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pEt1_Barrier = nullptr;

public:
	static CVfx_Destroyer_Skill_IgnorePain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END