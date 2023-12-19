#pragma once
#include "Tool.h"

#include "Effect.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CTool_Effect final : public CTool
{
private:
	CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Effect() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Create_Effect();
	
	void Store_TransformEffect();

	void Load_InfoEffect();
	void Store_InfoEffect();

	void Save_Effect(const char* pFileName);
	void Load_Effect(const char* pFileName);

	wstring Select_FolderName(_uint iFolderIndex);

private:
	class CGameObject* m_pEffect = nullptr;
	CEffect::EFFECT_DESC m_tEffectInfo;

	_float m_fPosition[3];
	_float m_fRotation[3];
	_float m_fScale[3];



	const char* m_cFolderName[22] = 
	{ "Aura", "Crack", "Decal", "Fire", "Flare", 
		"Glow", "Hit", "Ice", "Image", "Mask", 
		"MeshTrail", "Noise", "Object", "Ring", "Slash",
	"Smoke", "SubUV", "SwordTrail", "UI", "Water", 
		"Wind", "None" };
	_int m_iDiffuseFolderIndex = { 21 };
	_int m_iAlphaFolderIndex   = { 21 };

	char m_cLoadFbxName[MAX_PATH] = { "" };

	char m_cDiffuseTextureName[MAX_PATH] = { "" };
	char m_cAlphaTextureName[MAX_PATH] = { "" };

	char m_cDiffuseTexturePath[MAX_PATH] = { "" };
	char m_cAlphaTexturePath[MAX_PATH] = { "" };

public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END