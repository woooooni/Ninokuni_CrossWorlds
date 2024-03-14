#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CUI_CostumeTab_Map final : public CGameObject
{
private:
	CUI_CostumeTab_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CostumeTab_Map(const CUI_CostumeTab_Map& rhs);
	virtual ~CUI_CostumeTab_Map() = default;

public:
	void Set_Active(_bool bActive);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Reflect() override;

protected:
	virtual HRESULT Ready_Components() override;
	HRESULT Load_CostumeProps();

private:
	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

private:
	//vector<class CGameObject*> m_CostumeProps;
	_bool m_bActive = { false };
	_vector m_vCamPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_matrix m_vCamMatrix = XMMatrixIdentity();
	_float4x4 m_ViewMatrix;
	Matrix m_ProjMatrix;
	
public:
	static CUI_CostumeTab_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

