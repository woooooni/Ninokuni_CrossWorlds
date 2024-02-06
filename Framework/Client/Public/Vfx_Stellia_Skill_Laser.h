#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_Laser final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_WARNING,

		TYPE_ET2_E_MAINLINE_01,
		TYPE_ET2_E_MAINLINE_02,

		TYPE_ET2_E_CIRCLELINE,
		TYPE_ET2_E_LINEEFFECT,

		TYPE_ET2_P_CIRCLES,

		TYPE_ET3_V_FINISH,

		TYPE_END
	};

protected:
	CVfx_Stellia_Skill_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_Laser(const CVfx_Stellia_Skill_Laser& rhs);
	virtual ~CVfx_Stellia_Skill_Laser() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pMainLine_01 = nullptr;
	class CEffect* m_pMainLine_02 = nullptr;

public:
	static CVfx_Stellia_Skill_Laser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END