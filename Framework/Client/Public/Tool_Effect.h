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
	_uint   Get_FolderIndex(wstring& strName);

private:
	class CGameObject* m_pEffect = nullptr;
	CEffect::EFFECT_DESC m_tEffectInfo;

	_float m_fPosition[3];
	_float m_fRotation[3];
	_float m_fScale[3];

	char m_cBinaryName[MAX_PATH];

	const char* m_cEffectType[3] = { "EFFECT_TEXTURE", "EFFECT_MESH", "EFFECT_END" };
	_int m_iEffectTypeIndex = { 2 };

	_float m_fEffectRange[3];
	_float m_fEffectRangeDistance[2];

	char m_cModelName[MAX_PATH] = { "Prototype_Component_Model_" };
	const char* m_cFolderName[22] = 
	{ "Aura", "Crack", "Decal", "Fire", "Flare", 
		"Glow", "Hit", "Ice", "Image", "Mask", 
		"MeshTrail", "Noise", "Object", "Ring", "Slash",
	"Smoke", "SubUV", "SwordTrail", "UI", "Water", 
		"Wind", "None" };
	_int m_iDiffuseFolderIndex = { 21 };
	_int m_iAlphaFolderIndex   = { 21 };

	_float m_ffEffectUVIndex[2];
	_float m_ffEffectUVMaxCount[2];

	_float m_fAlphaStart[2];
	_float m_fAlphaSpeed[2];
	_float m_fAlphaChangeStartDelay[2];

public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END