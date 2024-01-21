#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Model.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
END

BEGIN(Client)

class CStaticObject abstract : public CGameObject
{
protected:
	explicit CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;
	virtual HRESULT Render_Minimap() override;

	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	CShader* Get_ShaderCom(CModel::TYPE eType)
	{
		if (eType == CModel::TYPE::TYPE_ANIM)
			return m_pAnimShaderCom;
		else
			return m_pNonAnimShaderCom;
	}

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName);


protected:
	void YRotation(_float fSpeed, _float fTimeDelta);
	void XRotation(_float fSpeed, _float fTimeDelta);
	void ZRotation(_float fSpeed, _float fTimeDelta);

protected:
	CShader* m_pNonAnimShaderCom = nullptr;
	CShader* m_pAnimShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;

	wstring m_strMapObjName = L"";

	_int m_iMoveTick = 0;
	_int m_iRandomCase = 0;

public:
	virtual void Free() override;
};

END

