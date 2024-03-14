#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CTexture;
END


BEGIN(Client)

class CMoon : public CGameObject
{
public:
	enum TEX_TYPE { RED_TEX, DIFFUSE_TEX, DISSOLVE_TEX, TEX_END };
private:
	explicit CMoon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CMoon(const CMoon& rhs);
	virtual ~CMoon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components();

public:
	const _bool& Get_AppearMoon() const { return m_bAppearMoon; }
	void Set_AppearMoon(_bool appaar) { m_bAppearMoon = appaar; }

public:
	void Set_RedColorStart(_bool start) { m_bIsRedColor = start; }
	const _bool& Get_RedColorStart() const { return m_bIsRedColor; }



private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = { nullptr, nullptr };

private:
	_bool m_bAppearMoon = false;
	_bool m_bIsRedColor = false;
	_float m_fTime = 0.0f;
	_float m_fWeight = 0.0f;
	_float m_fSpeed = 1.5f;

	Vec4 m_fChangeColor = Vec4(0.89f, 0.169f, 0.0f, 1.0f);
public:
	static CMoon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

