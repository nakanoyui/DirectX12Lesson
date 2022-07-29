#pragma once

class Json
{
public:
	void LoadJson(const std::string& className);

	void BakeJson(const nlohmann::json&serial);

	auto const& GetConstJson()const { return m_constJson; }

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
	}

	void AddJson(const std::string& className);

	const std::shared_ptr<Json> GetJson(const std::string_view& className);

	auto const GetAssetJson()const { return m_assetJson; }

private:
	std::map<std::string, std::shared_ptr<Json>> m_spJsons;
	nlohmann::json m_assetJson;
};