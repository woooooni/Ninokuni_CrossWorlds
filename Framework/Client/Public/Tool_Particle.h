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
	void Tick_CreateDelete();
	void Tick_Transform();
	void Tick_Prototype();
	void Tick_Binary();

private:
	void Create_Particle();
	void Load_InfoParticle();
	void Store_InfoParticle();
	void Set_OriginalInfoParticle();

	wstring Select_FolderName(_uint iFolderIndex);
	_uint Get_FolderIndex(wstring& strName);

private:
	void Save_Particle(const char* pFileName);
	void Save_Rigidbody(const char* pFileName);
	void Load_Particle(const char* pFileName);

private:
	class CGameObject* m_pParticle = nullptr;
	CParticle::PARTICLE_DESC m_tParticleInfo;
	CParticle::PARTICLE_RIGIDBODY_DESC m_tRigidbodyInfo;

private:
	_float3 m_fPosition;
	_float3 m_fRotation;
	_float3 m_fScale;

	char   m_cSaveAndLoadName[MAX_PATH] = { "" };

	const char* m_cProjection[3] = { "PROJ_PERSPECTIVE", "PROJ_ORTHOGRAPHIC", "PROJ_END"};
	_int m_iProjectionIndex = { 0 };

	const char* m_cFolderName[22] =
	{ "Aura", "Crack", "Decal", "Fire", "Flare",
		"Glow", "Hit", "Ice", "Image", "Mask",
		"MeshTrail", "Noise", "Object", "Ring", "Slash",
	"Smoke", "SubUV", "SwordTrail", "UI", "Water",
		"Wind", "None" };
	_int m_iDiffuseFolderIndex = { 21 };
	_int m_iAlphaFolderIndex = { 21 };

	char   m_cPrototypeName[MAX_PATH] = { "Prototype_" };

private:
	_float3 m_fParticleAxis;

	Vec4 m_vMinVelocity = {};
	Vec4 m_vMaxVelocity = {};

public:
	static CTool_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END