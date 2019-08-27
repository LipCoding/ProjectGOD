#include "QuestResourcesManager.h"
#include "Core/PathManager.h"
#include "Core.h"

QuestResourcesManager::QuestResourcesManager()
{
}


QuestResourcesManager::~QuestResourcesManager()
{
}

bool QuestResourcesManager::initialize()
{
	wstring temp_path = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);
	temp_path += L"ItemIcon/*.*";
	string itemIcon_folder_path = strconv(temp_path);
	vector<string> filesInForder;
	WIN32_FIND_DATAA fd;
	HANDLE handle = ::FindFirstFileA(itemIcon_folder_path.c_str(), &fd);

	if (INVALID_HANDLE_VALUE == handle)
		return false;

	do
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			filesInForder.push_back(fd.cFileName);
		}
	} while (::FindNextFileA(handle, &fd));

	::FindClose(handle);

	for (const auto& file_name : filesInForder)
	{
		CImage* new_image = new CImage;
		wstring item_path = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);
		item_path += L"ItemIcon\\" + strconv(file_name);
		//string item_path = strconv(temp_item_path);
		//item_path += file_name;
		
		HRESULT result = new_image->Load(item_path.c_str());
		reward_items_texture.push_back(make_pair(file_name, new_image));
	}



	return true;
}

CImage* QuestResourcesManager::findTexture(const string& texture_key)
{
	auto finder = find_if(reward_items_texture.begin(), reward_items_texture.end(),
		[texture_key](const TEXTURE& texture)
		{
			return texture.first == texture_key;
		}
	);

	if (finder != reward_items_texture.end())
		return finder->second;

	return nullptr;
}