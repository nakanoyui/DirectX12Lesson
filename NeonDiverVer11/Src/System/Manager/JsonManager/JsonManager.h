#pragma once

class Json
{
public:
	void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

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
		m_instanceJson.LoadJson("Asset/Json/instance.json");

		m_assetJson.LoadJson("Asset/Json/asset.json");

		m_generalJson.LoadJson("Asset/Json/general.json");

		for (nlohmann::json::iterator it = m_instanceJson.WorkConstJson().at("Instance").begin();
			it != m_instanceJson.WorkConstJson().at("Instance").end(); ++it)
		{
			AddJson(it.value().get<std::string>());
		}
	}

	auto const& GetInstanceJson()const { return m_instanceJson; }
	auto const& GetAssetJson()const { return m_assetJson; }
	auto const& GetGeneralJson()const { return m_generalJson; }

	auto& WorkInstanceJson() { return m_instanceJson; }
	auto& WorkGeneralJson() { return m_generalJson; }

	const std::shared_ptr<Json>& GetJson(const std::string& jsonName)const;
	const std::unordered_map<std::string, std::shared_ptr<Json>>& GetJsons()const { return m_spJsons; }

	void Reference();

	void BakeJson(const std::string& className);
private:
	void AddJson(const std::string& className);

	std::unordered_map<std::string, std::shared_ptr<Json>> m_spJsons;
	Json m_instanceJson;
	Json m_assetJson;
	Json m_generalJson;
};