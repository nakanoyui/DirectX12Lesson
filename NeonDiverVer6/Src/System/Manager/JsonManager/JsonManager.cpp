#include "JsonManager.h"

void Json::LoadJson(const std::string& className)
{
	std::string constPath = "Asset/Json/" + className + "/constant.json";
	std::ifstream ifs(constPath);
	ifs >> m_constJson;
	ifs.close();
}

void JsonManager::AddJson(const std::string& className)
{
	auto it = m_spJsons.find(className);

	if (it != m_spJsons.end())
	{
		return;
	}

	std::shared_ptr<Json> spJson = std::make_shared<Json>();
	spJson->LoadJson(className);

	m_spJsons[className] = spJson;
}

const std::shared_ptr<Json> JsonManager::GetJson(const std::string_view& className)
{
	auto it = m_spJsons.find(className.data());

	if (it != m_spJsons.end())
	{
		return it->second;
	}

	assert(0 && "JsonÇ™ë∂ç›ÇµÇ‹ÇπÇÒ");
	return nullptr;
}