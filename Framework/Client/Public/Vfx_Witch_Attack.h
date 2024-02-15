#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Witch_Attack final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_ATTACK_HANDREADY,
		TYPE_E_ATTACK_BODYREADY,
		TYPE_E_ATTACK_HANDDELETE,
		TYPE_E_ATTACK_BODYDELETE,

		TYPE_END
	};

protected:
	CVfx_Witch_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Witch_Attack(const CVfx_Witch_Attack& rhs);
	virtual ~CVfx_Witch_Attack() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pHandCreate = nullptr;
	class CEffect* m_pBodyCreate = nullptr;
	class CEffect* m_pHandDelete = nullptr;
	class CEffect* m_pBodyDelete = nullptr;

public:
	static CVfx_Witch_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END