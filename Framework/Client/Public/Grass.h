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
class CRigidBody;
class CVIBuffer_LensFlare;
END

BEGIN(Client)

class CGrass final : public CGameObject
{
private:
	enum GRASS_TEX { WIND_MAP, SHADE_MAP, GRASS_TEXEND };
private:
	struct CB_GRASS
	{
		_float fBladeWidthMin = 0.02f;
		_float fBladeWidthMax = 0.05f;
		_float fBladeHeightMin = 0.1f;
		_float fBladeHeightMax = 0.2f;

		_float fBladeBendDistance = 0.38f;
		_float fBladeBendCurve = 2.0f;
		_float fBendDelta = 0.2f;
		_float fTessellationGrassDistance = 0.1f;


		// Grass
		_float fGrassThreshold = 0.5f;
		_float fGrassFalloff = 0.05f;
		// Wind
		_float fWindFrequency = 0.01f;
		_float fFad = 0.0f;


		// Grass && Wind
		//Vec4 vGrassMap_ST;
		Vec4 vWindMap_ST;
		Vec4 vWindVelocity = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	};
private:
	explicit CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	explicit CGrass(const CGrass& rhs);
	virtual ~CGrass() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
private:
	virtual HRESULT Ready_Components() override;
	HRESULT Bind_ShaderResource();

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_LensFlare* m_pVIBufferCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pTextureCom[GRASS_TEX::GRASS_TEXEND] = {};

private:
	CB_GRASS m_CBGrass = {};
	_float m_fTime = 0.0f;
public:
	static CGrass* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END