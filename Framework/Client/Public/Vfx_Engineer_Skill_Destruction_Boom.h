#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_Destruction_Boom final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET2_D_CRACK,

		TYPE_ET2_P_FIRE_BIG,
		TYPE_ET2_P_FIRE_SMALL,
		// -------------------		  
		TYPE_ET2_E_CIRCLELINE,
		TYPE_ET2_P_CIRCLES,
		TYPE_ET2_P_STONE,
		TYPE_ET2_P_SMOKE,

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_Destruction_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_Destruction_Boom(const CVfx_Engineer_Skill_Destruction_Boom& rhs);
	virtual ~CVfx_Engineer_Skill_Destruction_Boom() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_Destruction_Boom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END