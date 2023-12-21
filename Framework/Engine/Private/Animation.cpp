#include "..\Public\Animation.h"
#include "Channel.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "Shader.h"
#include "Utils.h"
#include "GameInstance.h"

CAnimation::CAnimation()
{
	Vec3;
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_Channels(rhs.m_Channels)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_strName(rhs.m_strName)
	, m_fOriginSpeed(rhs.m_fOriginSpeed)
	, m_bLoop(rhs.m_bLoop)
	, m_SpeedDescs(rhs.m_SpeedDescs)
	, m_SoundEvents(rhs.m_SoundEvents)
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize_Prototype(aiAnimation* pAIAnimation)
{
	m_strName = CUtils::ToWString(pAIAnimation->mName.C_Str());
	m_fDuration = pAIAnimation->mDuration;
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i]);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(CModel* pModel)
{
	//if (99 != GI->Get_CurrentLevel()) return S_OK;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_ChannelKeyFrames.push_back(0);

		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_Channels[i]->Get_Name());

		if (nullptr == pNode)
			return E_FAIL;

		m_HierarchyNodes.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

void CAnimation::Update_Animation_Data(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	Update_Animation_Speed(fTickPerSecond, tDesc);
	Update_Animation_Event(fTickPerSecond, tDesc);
}

void CAnimation::Clear_AnimationData()
{
	/* 키프레임별 속도 조절 관련 */
	m_iCurSpeedDescIndex = 0;
	m_fCurSpeedDescEndFrame = 0.f;

	m_tLiveSpeedDesc.fCurValue = m_fOriginSpeed;


	/* 사운드 이벤트 */
	for (auto& SoundEvent : m_SoundEvents)
		SoundEvent.second.bExecuted = false;

}

_float CAnimation::Get_LiveSpeed()
{
	if(m_SpeedDescs.empty())
		return m_fOriginSpeed;

	return m_tLiveSpeedDesc.fCurValue;
}

void CAnimation::Add_SpeedDesc(ANIM_SPEED_DESC desc)
{
	m_SpeedDescs.push_back(desc);

	/* Sorting */

	std::sort(m_SpeedDescs.begin(), m_SpeedDescs.end(),
		[](const ANIM_SPEED_DESC& desc1, const ANIM_SPEED_DESC& desc2)
		{
			if (desc1.fStartFrame < desc2.fStartFrame)
				return true;
			else if (desc1.fStartFrame > desc2.fStartFrame)
				return false;
			return desc1.fEndFrame < desc2.fEndFrame;
		});

	Sort_SpeedDesces();
}
void CAnimation::Delete_SpeedDesc(const _uint& iIndex)
{
	if (m_SpeedDescs.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<ANIM_SPEED_DESC>::iterator iter = m_SpeedDescs.begin(); iter != m_SpeedDescs.end();)
	{
		if (iIndex == iCount)
		{
			m_SpeedDescs.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_SpeedDesces();
}

void CAnimation::Change_SpeedDesc(const _uint& iIndex, const Vec4& vDesc)
{
	if (m_SpeedDescs.size() <= iIndex)
		return;

	m_SpeedDescs[iIndex].fStartFrame = vDesc.x;
	m_SpeedDescs[iIndex].fEndFrame = vDesc.y;
	m_SpeedDescs[iIndex].fStartSpeed = vDesc.z;
	m_SpeedDescs[iIndex].fEndSpeed = vDesc.w;
}

void CAnimation::Sort_SpeedDesces()
{
	std::sort(m_SpeedDescs.begin(), m_SpeedDescs.end(),
		[](const ANIM_SPEED_DESC& desc1, const ANIM_SPEED_DESC& desc2)
		{
			if (desc1.fStartFrame < desc2.fStartFrame)
				return true;
			else if (desc1.fStartFrame > desc2.fStartFrame)
				return false;
			return desc1.fEndFrame < desc2.fEndFrame;
		});

	m_iCurSpeedDescIndex = 0;
}

void CAnimation::Change_EventKeyFrame(const _uint& iIndex, const _float fFrame, const ANIM_EVENT_TYPE& eEventType)
{
	switch (eEventType)
	{
	case Engine::SOUND:
	{
		if (m_SoundEvents.size() <= iIndex)
			return;

		m_SoundEvents[iIndex].first = fFrame;
	}
		break;
	case Engine::EFFECT:
		break;
	case Engine::CAMERA:
		break;
	case Engine::COLLIDER:
		break;
	case Engine::ANIM_EVENT_TYPE_END:
		break;
	default:
		break;
	}
}

void CAnimation::Add_SoundEvent(const _float& fFrame, const ANIM_EVENT_SOUND_DESC& desc)
{
	m_SoundEvents.push_back(pair(fFrame, desc));

	Sort_SoundEvents();
}

void CAnimation::Del_SoundEvent(const _uint iIndex)
{
	if (m_SoundEvents.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<pair<_float, ANIM_EVENT_SOUND_DESC>>::iterator iter = m_SoundEvents.begin(); iter != m_SoundEvents.end();)
	{
		if (iIndex == iCount)
		{
			m_SoundEvents.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_SoundEvents();
}

void CAnimation::Del_All_SoundEvent()
{
	m_SoundEvents.clear();
	m_SoundEvents.shrink_to_fit();
}

void CAnimation::Change_SoundEvent(const _uint iIndex, const ANIM_EVENT_SOUND_DESC& desc)
{
	if (m_SoundEvents.size() <= iIndex)
		return;

	m_SoundEvents[iIndex].second.pSoundKey = desc.pSoundKey;
	m_SoundEvents[iIndex].second.iChannelID = desc.iChannelID;
	m_SoundEvents[iIndex].second.fVolume = desc.fVolume;
	m_SoundEvents[iIndex].second.bStop = desc.bStop;
}

void CAnimation::Sort_SoundEvents()
{
	if (m_SoundEvents.empty())
		return;

	std::sort(m_SoundEvents.begin(), m_SoundEvents.end(),
		[](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first;
		});
}

CChannel* CAnimation::Get_Channel(const wstring& strChannelName)
{
	for (auto& iter : m_Channels)
	{
		if (strChannelName == iter->Get_Name())
			return iter;
	}

	return nullptr;
}

HRESULT CAnimation::Calculate_Animation(const _uint& iFrame)
{
	for (auto& pChannel : m_Channels)
	{
		for (auto& iCurrentKeyFrame : m_ChannelKeyFrames)
			iCurrentKeyFrame = iFrame;
	}

	_uint iChannelIndex = 0;
	for (auto& pChannel : m_Channels)
	{
		m_ChannelKeyFrames[iChannelIndex]
			= pChannel->Update_Transformation_NoneLerp(m_ChannelKeyFrames[iChannelIndex], m_HierarchyNodes[iChannelIndex]);

		++iChannelIndex;
	}

	return S_OK;
}

HRESULT CAnimation::Clear_Channels()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

	// 툴레벨이 아니라면 어차피 하이어러키 내부 데이터는 없음 
	
	//for (auto& pHierarchyNode : m_HierarchyNodes)
	//	Safe_Release(pHierarchyNode);

	//m_HierarchyNodes.clear();

	return S_OK;
}

void CAnimation::Update_Animation_Speed(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	/* Checke Speed */
	if (m_SpeedDescs.empty())
		return;

	if (m_tLiveSpeedDesc.bActive)
		m_tLiveSpeedDesc.Update(fTickPerSecond);

	for (_uint i = m_iCurSpeedDescIndex; i < m_SpeedDescs.size(); i++)
	{
		if (m_SpeedDescs[i].fStartFrame < tDesc.cur.iCurFrame && !m_tLiveSpeedDesc.bActive)
		{
			if (abs(m_tLiveSpeedDesc.fCurValue - m_SpeedDescs[i].fEndSpeed) < 0.05f)
				continue;

			if (m_SpeedDescs[m_iCurSpeedDescIndex].fStartFrame <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndFrame <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed <= 0.f)
				continue;

			if (m_SpeedDescs[m_iCurSpeedDescIndex].fStartFrame >=
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndFrame)
				continue;

			m_iCurSpeedDescIndex = i;

			m_tLiveSpeedDesc.Start(
				m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed,
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed,
				Calculate_LerpTime(m_SpeedDescs[m_iCurSpeedDescIndex], tDesc, fTickPerSecond),
				LERP_MODE::SMOOTH_STEP);
		}
	}
}

void CAnimation::Update_Animation_Event(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	/* 사운드 이벤트 */
	for (auto& SoundEvent : m_SoundEvents)
	{
		if (SoundEvent.first <= tDesc.cur.iCurFrame && !SoundEvent.second.bExecuted)
		{
			SoundEvent.second.bExecuted = true;
			 
			GI->Play_Sound(SoundEvent.second.pSoundKey, CHANNELID(SoundEvent.second.iChannelID), SoundEvent.second.fVolume, SoundEvent.second.bStop);

		}
	}
}

const _float CAnimation::Calculate_LerpTime(const ANIM_SPEED_DESC tSpeedDesc, const TWEEN_DESC tTweenDesc, const _float fTickPerSecond)
{
	_float fTimeAcc = 0.f;
	_float fFrameAcc = 0.f;

	_float fCurFrame = tSpeedDesc.fStartFrame;
	_float fEndFrame = tSpeedDesc.fEndFrame;

	/* 예외 처리 조건 필수 ! */

	/* 시작 프레임에서 종료 프레임까지 디폴트 시간으로 소요되는 총 시간 */
	while (1)
	{
		fTimeAcc += fTickPerSecond;
		fFrameAcc += fTickPerSecond;

		const _float fTimePerFrame = 1 / (m_fTickPerSecond * m_tLiveSpeedDesc.fCurValue);

		if (fTimePerFrame <= fFrameAcc)
		{
			fCurFrame++;
			fFrameAcc = 0.f;
			if (fCurFrame == tSpeedDesc.fEndFrame)
			{
				break;
			}
		}

		/* 혹시 모를 예외 처리*/
		if (100 < fTimeAcc)
			return 0.f;
	}


	fFrameAcc = 0.f;

	fCurFrame = tSpeedDesc.fStartFrame;
	fEndFrame = tSpeedDesc.fEndFrame;

	LERP_FLOAT_DESC tLerpDesc;
	tLerpDesc.Start(
		m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed,
		m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed,
		fTimeAcc, LERP_MODE::SMOOTH_STEP);

	fTimeAcc = 0.f;

	/* 시작 프레임에서 종료 프레임까지 럴프 시간 소요되는 총 시간 */
	while (1)
	{
		fTimeAcc += fTickPerSecond;
		fFrameAcc += fTickPerSecond;

		const _float fTimePerFrame = 1 / (m_fTickPerSecond * tLerpDesc.fCurValue);

		if (fTimePerFrame <= fFrameAcc)
		{
			fCurFrame++;
			fFrameAcc = 0.f;
			if (fCurFrame == tSpeedDesc.fEndFrame)
			{
				return fTimeAcc;
			}
		}

		if (tLerpDesc.bActive)
		{
			tLerpDesc.Update(fTickPerSecond);
			if(!tLerpDesc.bActive)
				return fTimeAcc;
		}

		/* 혹시 모를 예외 처리*/
		if (100 < fTimeAcc)
			return 0.f;
	}
	
	
	return 0.f;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Prototype(pAIAnimation)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create_Bin()
{
	CAnimation* pInstance = new CAnimation();
	return pInstance;
}

CAnimation* CAnimation::Clone(CModel* pModel)
{
	CAnimation* pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Initialize(pModel)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();


	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

	m_HierarchyNodes.clear();
}
