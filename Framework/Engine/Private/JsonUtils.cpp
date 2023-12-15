#include "FileUtils.h"
#include "Utils.h"
#include "..\Public\JsonUtils.h"
#include <fstream>
#include <filesystem>

CJsonUtils::CJsonUtils()
{
}

CJsonUtils::~CJsonUtils()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}

Json CJsonUtils::Json_Load(const wstring& strFilePath)
{
	Json data;

	_tchar szExt[MAX_PATH] = L"";
	_wsplitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	
	if (0 != lstrcmp(szExt, L".json"))
	{
		MSG_BOX("파일 확장자는 .json 이어야 합니다.");
		return data;
	}

	auto path = filesystem::path(strFilePath);

	filesystem::create_directories(path.parent_path());

	string strJson = "";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Read);
	strJson = File->Read<string>();


	data = Json::parse(strJson);
	return data;
}

HRESULT CJsonUtils::Json_Save(const wstring& strFilePath, const Json& refJsonFile)
{

	string strJson = refJsonFile.dump();

	_tchar szExt[MAX_PATH] = L"";
	_wsplitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);


	if (0 != lstrcmp(szExt, L".json"))
	{
		MSG_BOX("파일 확장자는 .json 이어야 합니다.");
		return E_FAIL;
	}

	auto path = filesystem::path(strFilePath);

	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Write);
	File->Write<string>(strJson);

	
	return S_OK;
}
