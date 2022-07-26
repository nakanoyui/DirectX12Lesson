#include "JsonManager.h"

void Json::LoadJson(const std::string& className)
{
	m_filePath = "Asset/Json/" + className + "/constant.json";
	std::ifstream ifs(m_filePath);
	ifs >> m_constJson;
	ifs.close();
}

void Json::BakeJson(const nlohmann::json& serial)
{
	std::ofstream ofs(m_filePath);

	if (ofs)
	{
		nlohmann::json json = serial;
		std::string strJson = json.dump(2);
		ofs.write(strJson.c_str(), strJson.size());
	}
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