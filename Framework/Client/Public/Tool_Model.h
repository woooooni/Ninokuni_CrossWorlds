#pragma once

#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Tick_Model(_float fTimeDelta);
	void Tick_Animation(_float fTimeDelta);
	void Tick_Event(_float fTimeDelta);
	void Tick_Socket(_float fTimeDelta);
	void Tick_Costume(_float fTimeDelta);

private:
	void Tick_Dummys(_float fTimeDelta);

private:
	HRESULT Render_DebugDraw();

private:
	/* In Initialize */
	HRESULT Ready_DebugDraw();
	HRESULT Ready_Dummy();
	HRESULT Ready_Weapons();
	HRESULT Ready_SoundKey();

	/* In Impory Animation */
	HRESULT Ready_SocketTransforms();

private:
	const _bool Is_Exception();

	HRESULT Clear_ToolAnimationData();
	HRESULT Claer_EventData();

	Vec3 Calculate_SocketPosition();
	Matrix Calculate_SocketWorldMatrix();

private:
	wstring m_strFilePath = L"";
	wstring m_strFileName = L"";

	wstring m_strPartFilePath = L"";
	wstring m_strPartFileName = L"";

	class CDummy* m_pDummy = nullptr;
	class CDummy* m_pPartDummy = nullptr;

#pragma region Animation

	_bool m_bAllAnimLoop = TRUE;

#pragma endregion


#pragma region Socket

	/* Bone */
	_int m_iCurBoneIndex = 0;

	/* 무기*/
	vector<class CWeapon*> m_Weapons;
	_int m_iCurWeaponIndex = -1;

	/* 소켓 */
	vector<ANIM_TRANSFORM_CACHES>	m_AnimTransformsCaches;/* 모든 뼈 계산 됨 */

	_int m_iSocketIndex = -1; /* 툴 리스트 박스에서의 인덱스 == 모델의 트랜스폼 벡터에서의 인덱스 */

	/* Render Index */
	_int m_iRenderSocketIndex = 0;

	vector<Vec3> m_vAutoSocket;
	_bool		m_bAuto = FALSE;
	_int		m_iAutoAnimIndex = 0;

#pragma endregion

#pragma region Event 

	_float m_fCurEventFrame = 0.f;



	_int m_iSoundEventIndex = -1;

	const char** m_arrSoundKeys = nullptr;
	_uint m_iSoundKeySize = 0;
	

#pragma endregion

#pragma region Custom Parts 
	CHARACTER_TYPE m_eCharacyerType = CHARACTER_TYPE::SWORD_MAN;
	PART_TYPE m_ePartType = PART_TYPE::HEAD;

	_bool m_bCostumeMode = FALSE;
#pragma endregion


#pragma region Debug Draw 

	Vec4 m_vColor = { 0.f, 0.f, 1.f, 1.f };
	BasicEffect* m_pEffect = nullptr;
	BoundingSphere* m_pSphere = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;

#pragma endregion

public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END