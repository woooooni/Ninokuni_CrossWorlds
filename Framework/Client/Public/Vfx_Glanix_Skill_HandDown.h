#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Glanix_Skill_HandDown final : public CVfx
{
private:
	enum TYPE {
		TYPE_START,
		TYPE_END
	};

protected:
	CVfx_Glanix_Skill_HandDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Glanix_Skill_HandDown(const CVfx_Glanix_Skill_HandDown& rhs);
	virtual ~CVfx_Glanix_Skill_HandDown() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Glanix_Skill_HandDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END