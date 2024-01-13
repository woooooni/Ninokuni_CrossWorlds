#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_FootDown final : public CVfx
{
private:
	enum TYPE {
		TYPE_D_CRACK,
		TYPE_P_SMOKE,
		TYPE_P_CIRCLE,
		TYPE_E_CIRCLE_LINE,
		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_FootDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_FootDown(const CVfx_Glanix_Skill_FootDown& rhs);
	virtual ~CVfx_Glanix_Skill_FootDown() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Glanix_Skill_FootDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END