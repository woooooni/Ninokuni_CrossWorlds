#pragma once
#include "Tool.h"

#include "Effect.h"
#include "Decal.h"

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
	void Tick_EffectTool();
	void Tick_DecalTool();
	void Tick_VfxTool();

	void Create_Effect();
	void Create_Decal();
	
	void Store_TransformEffect();
	void Store_TransformDecal();

	void Load_InfoEffect();
	void Load_InfoDecal();

	void Store_InfoEffect();
	void Store_InfoDecal();

	void Set_OriginalInfoEffect();
	void Set_OriginalInfoDecal();
	void Load_OriginalInfoVfx();

	void Save_Effect(const char* pFileName);
	void Save_Decal(const char* pFileName);

	void Load_Effect(const char* pFileName);
	void Load_Decal(const char* pFileName);

	wstring Select_FolderName(_uint iFolderIndex);
	_uint   Get_FolderIndex(wstring& strName);

private:
	class CGameObject* m_pEffect = nullptr;
	class CGameObject* m_pDecal  = nullptr;
	CEffect::EFFECT_DESC m_tEffectInfo;
	CDecal::DECAL_DESC   m_tDecalInfo;

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

	char   m_cPrototypeEffectName[MAX_PATH] = { "Prototype_" };
	char   m_cPrototypeDecalName[MAX_PATH] = { "Prototype_" };
	char   m_cPrototypeVfxName[MAX_PATH] = { "Prototype_" };

	_uint m_iVfxMaxCount = 0;
	_int* m_pFrameTriger = nullptr;
	_float3* m_pPositionOffset = nullptr;
	_float3* m_pScaleOffset = nullptr;
	_float3* m_pRotationOffset = nullptr;

public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END