#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Rage03Charge final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_CIRCLELINE_00,
		TYPE_ET1_E_CIRCLELINE_01,
		TYPE_ET1_P_CIRCLES,

		TYPE_ET2_E_CIRCLE,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Rage03Charge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Rage03Charge(const CVfx_Stellia_Skill_Rage03Charge& rhs);
	virtual ~CVfx_Stellia_Skill_Rage03Charge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_fCircleLine00Effect = nullptr;
	class CEffect* m_fCircleLine01Effect = nullptr;
	class CParticle* m_fCircleParticle = nullptr;

public:
	static CVfx_Stellia_Skill_Rage03Charge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END