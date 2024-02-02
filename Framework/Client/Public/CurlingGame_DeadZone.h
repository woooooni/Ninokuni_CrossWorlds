#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_DeadZone final : public CCurlingGame_Prop
{
private:
	CCurlingGame_DeadZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_DeadZone(const CCurlingGame_DeadZone& rhs);
	virtual ~CCurlingGame_DeadZone() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

public:
	static CCurlingGame_DeadZone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END

