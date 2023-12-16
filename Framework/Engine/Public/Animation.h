#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
public:
	
	enum class ANIM_EVENT_TYPE { SOUND, EFFECT, CAMERA, COLLIDER, TYPE_END };

	typedef struct tagAnimEventDesc
	{
		ANIM_EVENT_TYPE eType;

	}ANIM_EVENT_DESC;

	typedef struct tagAnimSpeedDesc
	{
		_float fPoint1 = 0.f;
		_float fPoint2 = 0.f;

		_float fValue1 = 0.f;
		_float fValue2 = 0.f;

		tagAnimSpeedDesc(const _float& _fPoint1, const _float& _fPoint2, const _float& _fValue1, const _float& _fValue2)
			: fPoint1(_fPoint1), fPoint2(_fPoint2), fValue1(_fValue1), fValue2(_fValue2) {}

	}ANIM_SPEED_DESC;


private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype(aiAnimation* pAIAnimation); 
	HRESULT Initialize(class CModel* pModel); 

public:

#pragma region Prop
	_float Get_Duration() { return m_fDuration; }

	void Set_AnimationName(const wstring& strName) { m_strName = strName; }
	const wstring& Get_AnimationName() { return m_strName; }

	void Set_Loop(_bool bLoop) { m_bLoop = bLoop; }
	_bool Is_Loop() { return m_bLoop; }

	_uint Get_MaxFrameCount() const { return _uint(m_fDuration) + 1; }
#pragma endregion

	
#pragma region Speed
	void Set_TickPerSecond(_float fTickPerSecond) { m_fTickPerSecond = fTickPerSecond; }
	_float Get_TickPerSecond() { return m_fTickPerSecond; }

	void Set_AnimationSpeed(_float fSpeed) { m_fSpeed = fSpeed; }
	_float Get_AnimationSpeed() { return m_fSpeed; }

	void Add_SpeedDesc(ANIM_SPEED_DESC desc) { m_SpeedDescs.push_back(desc); }
	vector<ANIM_SPEED_DESC> Get_SpeedDescs() const { return m_SpeedDescs; }

#pragma endregion


#pragma region Events



#pragma endregion


#pragma region FrameWork
public:
	class CChannel* Get_Channel(const wstring & strChannelName);
	const vector<class CChannel*>& Get_Channels() { return m_Channels; }

public:
	HRESULT Calculate_Animation(const _uint& iFrame);
	HRESULT Clear_Channels();
#pragma endregion

private:
	wstring	m_strName;

	_bool m_bLoop = false;
	_float m_fDuration = 0.f; 

private:
	_float m_fTickPerSecond = 0.f; 
	_float m_fSpeed = 1.f;


private:

	vector<ANIM_SPEED_DESC> m_SpeedDescs;

private: 
	_uint m_iNumChannels = 0;
	vector<class CChannel*>	m_Channels;
	vector<_uint> m_ChannelKeyFrames;
	vector<class CHierarchyNode*> m_HierarchyNodes;

public:
	static CAnimation* Create(aiAnimation* pAIAnimation);
	static CAnimation* Create_Bin();
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;

public:
	friend class CModel_Manager;
};

END