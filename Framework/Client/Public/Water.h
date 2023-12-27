#pragma once

#include "Animals.h"

BEGIN(Client)

class CWater final : public CDynamicObject
{
private:
	explicit CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName);

public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
		const wstring& strFilePath, const wstring& strFileName, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;
};

END

