#pragma once
#include "Part.h"

BEGIN(Client)

class CHair final : public CPart
{
private:
	CHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CHair(CHair& rhs);
	virtual ~CHair() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	virtual HRESULT Ready_Components() override;


public:
	static CHair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGameObject* Clone(void* pArg) override;
	void Free() override;




};

END