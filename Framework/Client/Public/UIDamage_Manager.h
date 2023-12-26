#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUIDamage_Manager : public CBase
{
	DECLARE_SINGLETON(CUIDamage_Manager)

private:
	CUIDamage_Manager();
	virtual ~CUIDamage_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Ready_DamageNumberPrototypes();

public:
	HRESULT Create_SkillDamageNumber(class CTransform* pTransformCom, _int iDamage, ELEMENTAL_TYPE eType = ELEMENTAL_END);
	HRESULT Create_CommonDamageNumber(class CTransform* pTransformCom, _int iDamage);
	HRESULT Create_Critical(class CTransform* pTransformCom);
	HRESULT Create_Miss(class CTransform* pTransformCom);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};
END