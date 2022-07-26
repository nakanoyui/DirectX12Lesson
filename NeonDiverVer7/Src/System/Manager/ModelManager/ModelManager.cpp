#include "ModelManager.h"

void ModelManager::ResisterModelData(const std::string& filepath)
{
	std::shared_ptr<ModelData> spModelData = std::make_shared<ModelData>();

	size_t last_pos = filepath.find_last_of('/');
	last_pos++; // find_last_ofŠÖ”‚¾‚Æ/‚ğl—¶‚µ‚¿‚á‚¤‚©‚ç/‚ğ”ò‚Î‚·
	std::string result = filepath.substr(last_pos, filepath.size() - last_pos);

	auto it = m_spModelDataList.find(filepath);

	if (it == m_spModelDataList.end())
	{
		if (!spModelData->Load(filepath.data()))
		{
			assert(0 && "ModelData‚Ì“o˜^‚É¸”s‚µ‚Ü‚µ‚½");
		}
	}
	else
	{
		//	‚·‚Å‚Éƒ‚ƒfƒ‹‚ªƒ[ƒh‚³‚ê‚Ä‚é
		return;
	}

	m_spModelDataList[result] = spModelData;
}

void ModelManager::ResisterModelWork(const std::string& filepath)
{
	std::shared_ptr<ModelWork> spModelWork = std::make_shared<ModelWork>();

	size_t last_pos = filepath.find_last_of('/');
	last_pos++; // find_last_ofŠÖ”‚¾‚Æ/‚ğl—¶‚µ‚¿‚á‚¤‚©‚ç/‚ğ”ò‚Î‚·
	std::string result = filepath.substr(last_pos, filepath.size() - last_pos);

	auto it = m_spModelWorkList.find(filepath);

	if (it == m_spModelWorkList.end())
	{
		if (!spModelWork->Load(filepath.data()))
		{
			assert(0 && "ModelWork‚Ì“o˜^‚É¸”s‚µ‚Ü‚µ‚½");
		}
	}
	else
	{
		//	‚·‚Å‚Éƒ‚ƒfƒ‹‚ªƒ[ƒh‚³‚ê‚Ä‚é
		return;
	}

	m_spModelWorkList[result] = spModelWork;
}

const std::shared_ptr<ModelData> ModelManager::GetModelData(const std::string_view& modelName)
{
	auto it = m_spModelDataList.find(modelName.data());

	if (it != m_spModelDataList.end())
	{
		return it->second;
	}

	assert(0 && "ModelData‚ª‘¶İ‚µ‚Ü‚¹‚ñ");
	return nullptr;
}

const std::shared_ptr<ModelWork> ModelManager::GetModelWork(const std::string_view& modelName)
{
	auto it = m_spModelWorkList.find(modelName.data());

	if (it != m_spModelWorkList.end())
	{
		return it->second;
	}

	assert(0 && "ModelWork‚ª‘¶İ‚µ‚Ü‚¹‚ñ");
	return nullptr;
}