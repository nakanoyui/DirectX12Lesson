#pragma once

class AssetManager
{
public:
	void AddShader(const std::shared_ptr<Shader>&spShader, const std::string_view shaderName);
	const std::shared_ptr<Shader>& GetShader(const std::string_view shaderName);

	std::shared_ptr<ModelData>GetModelData(const std::string_view& modelDataPath);

	std::shared_ptr<ModelWork>GetModelWork(const std::string_view& modelWorkPath);

	std::shared_ptr<Texture> GetTexture(const std::string_view& texturePath);

	void AddMesh(const std::shared_ptr<Mesh>& spMesh, const std::string_view& meshName);
	std::shared_ptr<Mesh>GetMesh(const std::string_view& meshName);
private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_spShaders;
	std::unordered_map<std::string, std::shared_ptr<ModelData>> m_spModelDatas;
	std::unordered_map<std::string, std::shared_ptr<ModelWork>> m_spModelWorks;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_spTextures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_spMeshes;
};