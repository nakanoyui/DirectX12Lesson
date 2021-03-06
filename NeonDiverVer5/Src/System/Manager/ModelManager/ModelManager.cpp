#include "ModelManager.h"

void ModelManager::ResisterModelData(const std::string& filepath)
{
	std::shared_ptr<ModelData> spModelData = std::make_shared<ModelData>();

	size_t last_pos = filepath.find_last_of('/');
	last_pos++; // find_last_of関数だと/を考慮しちゃうから/を飛ばす
	std::string result = filepath.substr(last_pos, filepath.size() - last_pos);

	auto it = m_spModelDataList.find(filepath);

	if (it == m_spModelDataList.end())
	{
		if (!spModelData->Load(filepath.data()))
		{
			assert(0 && "ModelDataの登録に失敗しました");
		}
	}
	else
	{
		//	すでにモデルがロードされてる
		return;
	}

	m_spModelDataList[result] = spModelData;
}

void ModelManager::ResisterModelWork(const std::string& filepath)
{
	std::shared_ptr<ModelWork> spModelWork = std::make_shared<ModelWork>();

	size_t last_pos = filepath.find_last_of('/');
	last_pos++; // find_last_of関数だと/を考慮しちゃうから/を飛ばす
	std::string result = filepath.substr(last_pos, filepath.size() - last_pos);

	auto it = m_spModelWorkList.find(filepath);

	if (it == m_spModelWorkList.end())
	{
		if (!spModelWork->Load(filepath.data()))
		{
			assert(0 && "ModelWorkの登録に失敗しました");
		}
	}
	else
	{
		//	すでにモデルがロードされてる
		return;
	}

	m_spModelWorkList[result] = spModelWork;
}

std::shared_ptr<ModelData> ModelManager::GetModelData(const std::string_view& modelName)
{
	auto it = m_spModelDataList.find(modelName.data());

	if (it != m_spModelDataList.end())
	{
		return it->second;
	}

	assert(0 && "ModelDataが存在しません");
	return nullptr;
}

std::shared_ptr<ModelWork> ModelManager::GetModelWork(const std::string_view& modelName)
{
	auto it = m_spModelWorkList.find(modelName.data());

	if (it != m_spModelWorkList.end())
	{
		return it->second;
	}

	assert(0 && "ModelWorkが存在しません");
	return nullptr;
}