#pragma once
#include "Tool.h"

#include "Particle.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CTool_Particle final : public CTool
{
private:
	CTool_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Particle() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Create_Particle();
	void Save_Particle(const char* pFileName);
	void Load_Particle(const char* pFileName);

private:
	class CGameObject* m_pParticle = nullptr;
	class CParticle::PARTICLE_DESC m_tParticleInfo;

	_float m_fPosition[3];
	_float m_fRotation[3];
	_float m_fScale[3];

	_float m_fParticleScale[2];
	_float m_fParticleRange[3];
	_float m_fParticleLifeTime[2];
	_float m_fParticleSpeed[2];

	_float m_fParticleBoxMin[3];
	_float m_fParticleBoxMax[3];

	_float m_fParticleVelocityMin[3];
	_float m_fParticleVelocityMax[3];

	_float m_fParticleAnimationSpeed[2];

	_float m_fParticleUVIndex[2];
	_float m_fParticleUVMaxCount[2];

	char   m_cDiffuseTextureName[MAX_PATH] = { "" };
	char   m_cAlphaTextureName[MAX_PATH]   = { "" };

	char   m_cDiffuseTexturePath[MAX_PATH] = { "" };
	char   m_cAlphaTexturePath[MAX_PATH] = { "" };

	char   m_cSaveAndLoadName[MAX_PATH] = { "" };

public:
	static CTool_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END