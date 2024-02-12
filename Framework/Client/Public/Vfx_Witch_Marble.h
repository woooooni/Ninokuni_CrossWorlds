#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Marble final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_E_MARBLE,
		TYPE_ET1_E_MARBLE_OUTLINE,

		TYPE_ET1_P_BACK,

		TYPE_END
	};

protected:
	CVfx_Witch_Marble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Marble(const CVfx_Witch_Marble& rhs);
	virtual ~CVfx_Witch_Marble() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_bool m_bCreate = false;

	CEffect* m_pMarble = nullptr;

public:
	static CVfx_Witch_Marble* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END