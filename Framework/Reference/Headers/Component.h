#pragma once

#include "Base.h"


BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);


public:
	class CGameObject* Get_Owner() { return m_pOwner; }
	void Set_Owner(class CGameObject* pOwner);

public:
	void Set_PrototypeTag(const wstring& strPrototypeTag) { m_strPrototypeTag = strPrototypeTag; }
	const wstring& Get_PrototypeTag() { return m_strPrototypeTag; }

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; }
#endif // DEBUG


protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class	CGameObject*	m_pOwner = { nullptr };
	_bool					m_isCloned = { false };
	wstring					m_strPrototypeTag = L"";
	

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END