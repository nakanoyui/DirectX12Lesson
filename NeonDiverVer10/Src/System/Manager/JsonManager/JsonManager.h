#pragma once

class Json
{
public:
	void LoadJson(const std::string& className);

	void BakeJson(const nlohmann::json& serial);

	auto const& GetConstJson()const { return m_constJson; }
	auto& WorkConstJson() { return m_constJson; }

private:
	nlohmann::json m_constJson;

	std::string m_filePath;
};

class JsonManager
{
public:
	JsonManager()
	{
		std::string assetPath = "Asset/Json/asset.json";
		std::ifstream ifs(assetPath);
		ifs >> m_assetJson;
		ifs.close();

		std::string instancePath = "Asset/Json/instance.json";
		ifs.open(instancePath);
		ifs >> m_instanceJson;
		ifs.close();

		std::string generalPath = "Asset/Json/general.json";
		ifs.open(generalPath);
		ifs >> m_generalJson;
		ifs.close();

		for (nlohmann::json::iterator it = m_instanceJson.at("Instance").begin(); it != m_instanceJson.at("Instance").end(); ++it)
		{
			AddJson(it.value().get<std::string>());
		}
	}

	auto const& GetInstanceJson()const { return m_instanceJson; }
	auto const& GetAssetJson()const { return m_assetJson; }
	auto const& GetGeneralJson()const { return m_generalJson; }

	auto& WorkGeneralJson() { return m_generalJson; }

	const std::shared_ptr<Json>& GetJson(const std::string&jsonName)const;
	const std::unordered_map<std::string, std::shared_ptr<Json>>& GetJsons()const { return m_spJsons; }

	void Reference();

private:
	void  AddJson(const std::string& className);

	std::unordered_map<std::string, std::shared_ptr<Json>> m_spJsons;
	nlohmann::json m_instanceJson;
	nlohmann::json m_assetJson;
	nlohmann::json m_generalJson;
};