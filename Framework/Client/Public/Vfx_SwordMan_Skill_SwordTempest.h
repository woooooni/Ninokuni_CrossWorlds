#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_SwordMan_Skill_SwordTempest final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_SQUARE, 

		TYPE_E_ATTACK_01,

		TYPE_P_SPARKLE_01,

		TYPE_E_ATTACK_02, 
		
		TYPE_P_XSPARKLE_01,

		TYPE_E_ATTACK_03,

		TYPE_P_SPARKLE_02,

		TYPE_E_ATTACK_04, 
		
		TYPE_P_XSPARKLE_02,

		TYPE_E_ATTACK_05,

		TYPE_END
	};

protected:
	CVfx_SwordMan_Skill_SwordTempest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_SwordMan_Skill_SwordTempest(const CVfx_SwordMan_Skill_SwordTempest& rhs);
	virtual ~CVfx_SwordMan_Skill_SwordTempest() = default;

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
	static CVfx_SwordMan_Skill_SwordTempest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END