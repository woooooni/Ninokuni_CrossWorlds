#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Spawn_Roar final : public CVfx
{
private:
	enum TYPE { // L"SKM_Stellia.ao|Stellia_Spawn"
		TYPE_ET0_D_MAGICCIRCLE,

		// 31 ÂøÁö
		TYPE_ET1_P_SMOKE,
		TYPE_ET1_P_CIRCLES,

		// 47 - 71 Æ÷È¿
		TYPE_ET2_E_ROAR,

		TYPE_END
	};

protected:
	CVfx_Stellia_Spawn_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Spawn_Roar(const CVfx_Stellia_Spawn_Roar& rhs);
	virtual ~CVfx_Stellia_Spawn_Roar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pMagicCircle = nullptr;
	_bool m_bRadialBlur = false;

public:
	static CVfx_Stellia_Spawn_Roar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END