#pragma once

class ModelManager
{
public:
	ModelManager() = default;
	~ModelManager() = default;

	/// <summary>
	/// モデルデータの登録
	/// </summary>
	/// <param name="filepath">モデルのファイルパス</param>
	void ResisterModelData(const std::string& filepath);

	/// <summary>
	/// モデルワークの登録
	/// </summary>
	/// <param name="filepath">モデルのファイルパス</param>
	void ResisterModelWork(const std::string& filepath);

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	/// <param name="modelName">モデルのファイルパス</param>
	/// <returns>モデルデータ</returns>
	const std::shared_ptr<ModelData> GetModelData(const std::string_view& modelName);

	/// <summary>
	/// モデルワークの取得
	/// </summary>
	/// <param name="modelName">モデルのファイルパス</param>
	/// <returns>モデルワーク</returns>
	const std::shared_ptr<ModelWork> GetModelWork(const std::string_view& modelName);

private:

	std::map<std::string, std::shared_ptr<ModelData>> m_spModelDataList;

	std::map<std::string, std::shared_ptr<ModelWork>> m_spModelWorkList;
};