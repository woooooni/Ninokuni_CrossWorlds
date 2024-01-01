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
class CVIBuffer_Rect;
END

BEGIN(Client)

class CAurora final : public CGameObject
{
private:
	struct AURORA_DESC
	{
		_float fWaveScale = 1.6f;
		_float fWaveAmplitude = 1.539336;
		_float fWaveSpeed = 1.0f;
		_float fPadding = 0.0f;

		Vec3 vScalar = Vec3(2000.0f, 130.0f, 100.0f);
		_float fTime = 0.0f;
		Vec3 vPosition = Vec3(0.0f, 206.942184f, 190.32748f);
		_float fPadding2 = 0.0f;
	};

private:
	enum LEVEL_AURORA { WINTER_AURORA, WITCH_AURORA, AURORA_END };

private:
	explicit CAurora(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CAurora(const CAurora& rhs);
	virtual ~CAurora() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom[LEVEL_AURORA::AURORA_END] = {};

private:
	AURORA_DESC m_AuroraDesc = {};

public:
	static CAurora* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

