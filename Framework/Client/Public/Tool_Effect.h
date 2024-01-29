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
	enum TYPE { TYPE_EFFECT, TYPE_DECAL, TYPE_VFX, TYPE_END };

private:
	CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Effect() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	_bool Tick_EffectTool();
	_bool Tick_DecalTool();
	_bool Tick_VfxTool();

	void Tick_CreateDelete(TYPE eType);
	void Tick_Transform(TYPE eType);
	void Tick_Prototype(TYPE eType);
	void Tick_Binary(TYPE eType);

private:
	void Create_Object(TYPE eType);
	void Set_Transform(TYPE eType);

	void Load_ObjectInfo(TYPE eType);
	void Store_ObjectInfo(TYPE eType);

	void Set_OriginalInfo(TYPE eType);
	void Load_OriginalInfoVfx();

	wstring Select_FolderName(_uint iFolderIndex);
	_uint Get_FolderIndex(wstring& strName);

private:
	void Save_Effect(const char* pFileName);
	void Load_Effect(const char* pFileName);

	void Save_Decal(const char* pFileName);
	void Load_Decal(const char* pFileName);

private:
	class CGameObject* m_pEffect = nullptr;
	class CGameObject* m_pDecal  = nullptr;
	CEffect::EFFECT_DESC m_tEffectInfo;
	CDecal::DECAL_DESC   m_tDecalInfo;

private:
	_float3 m_fPosition;
	_float3 m_fRotation;
	_float3 m_fScale;

	char m_cBinaryName[MAX_PATH];

	const char* m_cEffectType[3] = { "EFFECT_TEXTURE", "EFFECT_MESH", "EFFECT_END" };
	_int m_iEffectTypeIndex = { 2 };

	char m_cModelName[MAX_PATH] = { "Prototype_Component_Model_" };
	const char* m_cFolderName[22] = 
	{ "Aura", "Crack", "Decal", "Fire", "Flare", 
		"Glow", "Hit", "Ice", "Image", "Mask", 
		"MeshTrail", "Noise", "Object", "Ring", "Slash",
	"Smoke", "SubUV", "SwordTrail", "UI", "Water", 
		"Wind", "None" };
	_int m_iDiffuseFolderIndex = { 21 };
	_int m_iAlphaFolderIndex   = { 21 };
	_int m_iDistortionFolderIndex = { 21 };

	char   m_cPrototypeName[MAX_PATH] = { "" };

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