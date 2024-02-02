#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_SwordMan_Skill_PerfectBlade final : public CVfx
{
protected:
	CVfx_SwordMan_Skill_PerfectBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_SwordMan_Skill_PerfectBlade(const CVfx_SwordMan_Skill_PerfectBlade& rhs);
	virtual ~CVfx_SwordMan_Skill_PerfectBlade() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_int m_iType = -1;
	_float3 m_fMainColor;

public:
	static CVfx_SwordMan_Skill_PerfectBlade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END