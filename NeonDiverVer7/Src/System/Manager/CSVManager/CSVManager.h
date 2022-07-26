#pragma once

struct StageData
{
	Math::Vector3 m_pos;
	float m_rotate;
	int m_mapKind;
};

class CSVManager
{
public:
	CSVManager(std::shared_ptr<JsonManager> jsonManager) { m_wpJsonManager = jsonManager; };
	~CSVManager() { m_stageDatas.clear(); }

	void Load(const std::string_view filepath);

	const std::vector<StageData>& GetStageData()const { return m_stageDatas; }
private:
	std::vector<StageData> m_stageDatas;

	std::string m_filePath;

	std::weak_ptr<JsonManager> m_wpJsonManager;
};