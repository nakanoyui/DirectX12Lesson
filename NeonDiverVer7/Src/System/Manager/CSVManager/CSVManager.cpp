#include "CSVManager.h"

void CSVManager::Load(const std::string_view filepath)
{
	m_filePath = filepath.data();

	std::ifstream ifs(m_filePath);

	if (!ifs)
	{
		assert(0 && "CSVDataが見つかりません");
	}

	int z = 0, x = 0;
	float gridSize = m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("GridSize");
	Math::Vector3 offset = Math::Vector3(
		m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("Offset").at("X"),
		m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("Offset").at("Y"),
		m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("Offset").at("Z"));

	offset.x *= m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("X").get<float>();
	offset.z *= m_wpJsonManager.lock()->GetJson("StageData")->GetConstJson().at("Z").get<float>();

	while (true)
	{
		std::string lineData;
		if (!getline(ifs, lineData)) { break; }

		// .で分けて単語ごとにデータを格納
		std::vector<StageData> lineDatas;
		std::istringstream stream(lineData);
		std::string element;

		while (getline(stream, element, ','))
		{
			StageData mapData;
			mapData.m_pos = { (float)(gridSize * x),Constant::Zero_F,-(float)(gridSize * z) };
			mapData.m_pos += offset;
			mapData.m_mapKind = stoi(element);

			m_stageDatas.push_back(mapData);

			x++;
		}
		z++;
		x = 0;
	}
}