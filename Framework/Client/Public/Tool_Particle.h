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
	void Load_InfoParticle();
	void Store_InfoParticle();
	void Save_Particle(const char* pFileName);
	void Load_Particle(const char* pFileName);

private:
	class CGameObject* m_pParticle = nullptr;
	class CParticle::PARTICLE_DESC m_tParticleInfo;

	_float m_fPosition[3];
	_float m_fRotation[3];
	_float m_fScale[3];

	_bool m_bParticleType_Pers = false;
	_bool m_bParticleType_Orth = false;

	_float m_fParticleRange[3];
	_float m_fParticleRangeDistance[2];

	_float m_fParticleScaleStart[2];

	_float m_fParticleScaleChangeStartDelay[2];
	_float m_fParticleScaleChangeTime[2];

	_float m_fParticleScaleMin[2];
	_float m_fParticleScaleMax[2];
	_float m_fParticleScaleSpeed[2];

	_float m_fParticleVelocityMin[3];
	_float m_fParticleVelocityMax[3];

	_float m_fParticleVelocityChangeStartDelay[2];
	_float m_fParticleVelocityChangeTime[2];

	_float m_fParticleVelocityMin_02[3];
	_float m_fParticleVelocityMax_02[3];
	_float m_fParticleVelocityMin_03[3];
	_float m_fParticleVelocityMax_03[3];
	_float m_fParticleVelocityMin_04[3];
	_float m_fParticleVelocityMax_04[3];
	_float m_fParticleVelocityMin_05[3];
	_float m_fParticleVelocityMax_05[3];
	_float m_fParticleVelocityMin_06[3];
	_float m_fParticleVelocityMax_06[3];
	_float m_fParticleVelocityMin_07[3];
	_float m_fParticleVelocityMax_07[3];
	_float m_fParticleVelocityMin_08[3];
	_float m_fParticleVelocityMax_08[3];
	_float m_fParticleVelocityMin_09[3];
	_float m_fParticleVelocityMax_09[3];
	_float m_fParticleVelocityMin_10[3];
	_float m_fParticleVelocityMax_10[3];

	_float m_fParticleVelocityTime_01[2];
	_float m_fParticleVelocityTime_02[2];
	_float m_fParticleVelocityTime_03[2];
	_float m_fParticleVelocityTime_04[2];
	_float m_fParticleVelocityTime_05[2];
	_float m_fParticleVelocityTime_06[2];
	_float m_fParticleVelocityTime_07[2];
	_float m_fParticleVelocityTime_08[2];
	_float m_fParticleVelocityTime_09[2];
	_float m_fParticleVelocityTime_10[2];


	_float m_fParticleAxis[3];
	_float m_fParticleRotationSpeed[2];

	_float m_fParticleRotationChangeStartDelay[2];
	_float m_fParticleRotationChangeTime[2];


	_float m_fParticleLifeTime[2];
	_float m_fParticleSpeed[2];

	_float m_fParticleBoxMin[3];
	_float m_fParticleBoxMax[3];

	_float m_fParticleAnimationSpeed[2];

	_float m_fParticleUVIndex[2];
	_float m_fParticleUVMaxCount[2];


	_float m_fFadeChangeStartDelay[2];
	_float m_fStartAlpha[2];
	_float m_fFadeSpeed[2];


	_float m_fColorChangeStartDelay[2];
	_float m_fColorChangeStartM[2];
	_float m_fColorChangeStartF[2];
	_float m_fColorChangeStartE[2];

	_float m_fColorChangeRandomTime[2];

	char   m_cDiffuseTextureName[MAX_PATH] = { "" };
	char   m_cAlphaTextureName[MAX_PATH]   = { "" };

	char   m_cDiffuseTexturePath[MAX_PATH] = { "" };
	char   m_cAlphaTexturePath[MAX_PATH]   = { "" };

	char   m_cSaveAndLoadName[MAX_PATH] = { "" };

	_float m_fBlack_Discard[3];

public:
	static CTool_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END