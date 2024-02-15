#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_GrandPrix_BlackHole final : public CVfx
{
private:
	enum TYPE {
		TYPE_E_SPHERE_CREATE,
		TYPE_E_SPHERE_DRAIN,
		TYPE_E_SPHERE_DRAIN02,
		TYPE_E_SPHERE_OUTLINE,

		TYPE_E_SPHERE_DELETE,
		TYPE_E_SPHERE_DELETE2,
		TYPE_E_SPHERE_DELETE3,
		TYPE_E_SPHERE_DELETE4,
		TYPE_END
	};

protected:
	CVfx_GrandPrix_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_GrandPrix_BlackHole(const CVfx_GrandPrix_BlackHole& rhs);
	virtual ~CVfx_GrandPrix_BlackHole() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CEffect* m_pBlackHole = nullptr;
	class CEffect* m_pDrain = nullptr;
	class CEffect* m_pBlackHoleSmaller = nullptr;

	_bool m_bIsCreate = false;
	_bool m_bIsSmaller = false;

public:
	static CVfx_GrandPrix_BlackHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END