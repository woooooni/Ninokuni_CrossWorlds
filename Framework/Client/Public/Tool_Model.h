#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Tick_Model(_float fTimeDelta);
	void Tick_Animation(_float fTimeDelta);
	void Tick_Socket(_float fTimeDelta);
	void Tick_Event(_float fTimeDelta);
	void Tick_Costume(_float fTimeDelta);

private:
	void Reset_Transform();
	const _bool Is_Exception();

private:
	wstring m_strFilePath = L"";
	wstring m_strFileName = L"";
	class CDummy* m_pDummy = nullptr;

public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END