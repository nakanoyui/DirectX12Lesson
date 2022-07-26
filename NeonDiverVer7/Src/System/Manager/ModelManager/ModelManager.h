#pragma once

class ModelManager
{
public:
	ModelManager() = default;
	~ModelManager() = default;

	/// <summary>
	/// ���f���f�[�^�̓o�^
	/// </summary>
	/// <param name="filepath">���f���̃t�@�C���p�X</param>
	void ResisterModelData(const std::string& filepath);

	/// <summary>
	/// ���f�����[�N�̓o�^
	/// </summary>
	/// <param name="filepath">���f���̃t�@�C���p�X</param>
	void ResisterModelWork(const std::string& filepath);

	/// <summary>
	/// ���f���f�[�^�̎擾
	/// </summary>
	/// <param name="modelName">���f���̃t�@�C���p�X</param>
	/// <returns>���f���f�[�^</returns>
	const std::shared_ptr<ModelData> GetModelData(const std::string_view& modelName);

	/// <summary>
	/// ���f�����[�N�̎擾
	/// </summary>
	/// <param name="modelName">���f���̃t�@�C���p�X</param>
	/// <returns>���f�����[�N</returns>
	const std::shared_ptr<ModelWork> GetModelWork(const std::string_view& modelName);

private:

	std::map<std::string, std::shared_ptr<ModelData>> m_spModelDataList;

	std::map<std::string, std::shared_ptr<ModelWork>> m_spModelWorkList;
};