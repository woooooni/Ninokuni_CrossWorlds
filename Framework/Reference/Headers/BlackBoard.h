#pragma once

#include "Base.h"

BEGIN(Engine)

template <typename T>
class ENGINE_DLL CBlackBoard final : public CBase
{
private:
	CBlackBoard();
	virtual ~CBlackBoard() = default;

public:
    void SetValue(const wstring & key, const T & value) {
        m_mapStorage[key] = value;
    }

    T GetValue(const wstring & key) const {
        auto iter = m_mapStorage.find(key);
        if (iter != m_mapStorage.end()) {
            return iter->second;
        }
        else {
            MSG_BOX("Fail Find Key");
        }
    }
protected:
    map<wstring, T> m_mapStorage;

public:
    virtual void Free() override;

};

END

