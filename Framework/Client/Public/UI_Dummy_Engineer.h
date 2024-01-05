#pragma once
#include "Character.h"

BEGIN(Client)
class CUI_Dummy_Engineer final : public CCharacter
{
private:
	CUI_Dummy_Engineer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dummy_Engineer(const CUI_Dummy_Engineer& rhs);
	virtual ~CUI_Dummy_Engineer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

private:
	_float2 Transpose_ProjectionPosition();
	
public:
	static CUI_Dummy_Engineer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

