#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Whale_Jump final : public CVfx
{
private:
	enum TYPE {
		TYPE_JUMP,
		TYPE_DOWN_TAIL,
		TYPE_DOWN_HEAD,

		TYPE_END
	};

protected:
	CVfx_Whale_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Whale_Jump(const CVfx_Whale_Jump& rhs);
	virtual ~CVfx_Whale_Jump() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_float m_fAccEffect = 0.f;

public:
	static CVfx_Whale_Jump* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END