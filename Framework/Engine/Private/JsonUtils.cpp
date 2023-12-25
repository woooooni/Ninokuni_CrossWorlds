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
	
	/* 여기서부터 추가된 코드 */
	ifstream file(strFilePath, ios::binary);
	if (!file.is_open()) {
		MSG_BOX("파일을 열 수 없습니다.");
		return data;
	}

	getline(file, strJson, static_cast<char>(EOF));

	// Bom이 있다면 제거 후 로드.
	if (strJson.size() >= 3 && strJson[0] == '\xEF' && strJson[1] == '\xBB' && strJson[2] == '\xBF') {
		strJson.erase(0, 3);

		data = data.parse(strJson);
	}
	else
	{
		shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
		File->Open(strFilePath, FileMode::Read);

		strJson = File->Read<string>();

		data = data.parse(strJson);
	}

	return data;
}

HRESULT CJsonUtils::Json_Save(const wstring& strFilePath, const Json& refJsonFile)
{
	string strJson = refJsonFile.dump(1);

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

	// BOM은 일부 텍스트 에디터에서 파일의 인코딩을 인식하는데 사용.
	// 대부분의 텍스터 에디터 및 프로그램은 UTF-8을 잘 인식하나 드물게 인식을 못하는 경우도 있어서 BOM을 추가.
	 
	// UTF-8 BOM을 나타내는 3바이트의 값.
	// \xEF: UTF - 8에서 BOM의 시작 부분을 나타내는 바이트.
	// \xBB : BOM의 중간 부분을 나타내는 바이트.
	// \xBF : BOM의 마지막 부분을 나타내는 바이트.
	// 때때로 BOM이 포함되면 예기치 않은 결과를 초래할 수 있으므로 사용 시 주의
	string bom = "\xEF\xBB\xBF"; 
	File->Write(bom.data(), bom.size());
	
	File->Write(strJson.data(), strJson.size());

	// File->Write<string>(strJson);

	
	return S_OK;
}

