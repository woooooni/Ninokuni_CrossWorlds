#pragma once
#include "Engine_Defines.h"

class ENGINE_DLL CJsonUtils
{
public:
	CJsonUtils();
	~CJsonUtils();

public:
	static Json Json_Load(const wstring& strFilePath);
	static HRESULT Json_Save(const wstring& strFilePath, const Json& refJsonFile);

private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

