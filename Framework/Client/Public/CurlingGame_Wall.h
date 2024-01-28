#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_Wall final : public CCurlingGame_Prop
{
private:
	CCurlingGame_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Wall(const CCurlingGame_Wall& rhs);
	virtual ~CCurlingGame_Wall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	Vec3 Get_Normal() const { return m_vNormal; }
	void Set_Normal(const Vec3 vNormal) { memcpy(&m_vNormal, &vNormal, sizeof(Vec3)); }

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

private:
	Vec3 m_vNormal;

public:
	static CCurlingGame_Wall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END

