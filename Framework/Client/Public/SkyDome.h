#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
END


BEGIN(Client)
class CSkyDome final : public CGameObject 
{
protected:
	explicit CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual HRESULT Ready_Components();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

private:
	Vec4 m_vApexColor = Vec4(0.0f, 31.0f / 255.0f, 114.0f / 255.0f, 1.0f);
	Vec4 m_vCenterColor = Vec4(183.0f / 255.0f, 255.0f/ 255.0f , 261.0f / 255.0f,1.0f);


public:
	static CSkyDome* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

