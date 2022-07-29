#include "JsonManager.h"

void Json::LoadJson(const std::string& className)
{
	m_filePath = className;
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

const std::shared_ptr<Json>& JsonManager::GetJson(const std::string& jsonName) const
{
	std::string filePath = "Asset/Json/" + jsonName + "/constant.json";
	auto it = m_spJsons.find(filePath);

	if (it != m_spJsons.end())
	{
		return it->second;
	}

	assert(0&&"Jsonƒtƒ@ƒCƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	return it->second;
}

void JsonManager::AddJson(const std::string& className)
{
	std::string filePath = "Asset/Json/" + className + "/constant.json";
	auto it = m_spJsons.find(filePath);

	if (it == m_spJsons.end())
	{
		std::shared_ptr<Json> spJson = std::make_shared<Json>();
		spJson->LoadJson(className.data());
		m_spJsons[className.data()] = spJson;
	}
}

void JsonManager::Reference()
{
	m_spJsons.clear();

	for (nlohmann::json::iterator it = m_instanceJson.at("Instance").begin(); it != m_instanceJson.at("Instance").end(); ++it)
	{
		AddJson(it.value().get<std::string>());
	}
}