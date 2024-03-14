#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CWater : public CDynamicObject
{
public:
	struct VS_GerstnerWave
	{
		Vec2 A = Vec2(6.243707f, 2.381200f);
		Vec2 F = Vec2(0.2f, 0.1f);

		Vec2 S = Vec2(0.42f, 0.86f);
		Vec2 vPadding;
		Vec4 D = Vec4(0.2f, 0.4f, 0.8f, 1.8f);
	};
	struct PS_GerstnerWave
	{
		_float fresnelBias = 0.265310f;
		_float fresnelPower = 7.754313f;
		_float waveAmount = 0.62173;
		_float reflectAmount = 0.98109f;

		Vec3 vLightDir; // DirectionLight
		_float bumpScale = 0.42356f;

		Vec3 vLightColor; // DirectionLight
		_float pad = 0.0f;
		Vec4 vShallowWaterColor = Vec4(0.215686f, 0.743581f, 1.0f, 1.0f);
		Vec4 vDeepWaterColor = Vec4(0.050980f, 0.481481f, 0.304660f, 1.0f);
	};
protected:
	explicit CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName);

	virtual HRESULT Bind_ShaderResources();

public:
	VS_GerstnerWave& Get_VSGerstnerWave() { return m_VSGerstnerWave; }
	PS_GerstnerWave& Get_PSGerstnerWave() { return m_PSGerstnerWave; }
	_float& Get_Damper() { return m_fDamper; }

	void Set_VSGerstnerWave(VS_GerstnerWave& vs) { ::memcpy(&m_VSGerstnerWave, &vs, sizeof(VS_GerstnerWave)); }
	void Set_PSGerstnerWave(PS_GerstnerWave& ps) { ::memcpy(&m_PSGerstnerWave, &ps, sizeof(PS_GerstnerWave)); }
	void Set_Damper(_float damp) { m_fDamper = damp; }


public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

protected:
	VS_GerstnerWave m_VSGerstnerWave = {};
	PS_GerstnerWave m_PSGerstnerWave = {};
	_float m_fDamper = 0.005f;

private:
	CShader* m_pWaterShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pDiffuseTextureCom = nullptr;
	CTexture* m_pNoiseTextureCom[2] = { nullptr, nullptr };
protected:
	// Water State
	_float m_fReflectRefractSacle = 0.1f;
	_float m_fWaterTranslationSpeed = 0.015f;

	_float m_fTime = 0.0f;
	_float m_fBloomTile = 12.0f;

	Vec2 m_vDirection;

	_bool m_bFresnel = false;
public:
	virtual void Free() override;
};

END

