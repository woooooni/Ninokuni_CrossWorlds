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
class CMirror final : public CGameObject
{
private:
	explicit CMirror(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CMirror(const CMirror& rhs);
	virtual ~CMirror() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Reflect() override;

public:
	virtual HRESULT Ready_Components();

public: // UI
	void Set_Active(_bool bActive) { m_bActive = bActive; }

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;

private: // UI
	_bool m_bActive = { false };
	_vector m_vCamPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_matrix m_vCamMatrix = XMMatrixIdentity();
	_float4x4 m_ViewMatrix;

public:
	static CMirror* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

