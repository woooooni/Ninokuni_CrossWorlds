#pragma once

#include "StaticObject.h"

BEGIN(Client)

class CDog_FootPrints : public CStaticObject
{
	CDog_FootPrints(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	CDog_FootPrints(const CDog_FootPrints& rhs);
	virtual ~CDog_FootPrints() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components();

private:
	class CDecal* m_pFootPrints = nullptr;
	_float m_fShowDist = 0.f;

	_bool m_bIsShow = false;

public:
	static CDog_FootPrints* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;

};

END
