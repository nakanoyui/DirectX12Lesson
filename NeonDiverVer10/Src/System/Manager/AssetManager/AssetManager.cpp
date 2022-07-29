#include "AssetManager.h"

void AssetManager::AddShader(const std::shared_ptr<Shader>& spShader, const std::string_view shaderName)
{
	auto it = m_spShaders.find(shaderName.data());

	if (it != m_spShaders.end())
	{
		return;
	}

	m_spShaders[shaderName.data()] = spShader;
}

const std::shared_ptr<Shader>& AssetManager::GetShader(const std::string_view shaderName)
{
	auto it = m_spShaders.find(shaderName.data());

	if (it != m_spShaders.end())
	{
		return it->second;
	}

	assert(0 && "ShaderÇ™ë∂ç›ÇµÇ‹ÇπÇÒ");
	return it->second;
}

std::shared_ptr<ModelData> AssetManager::GetModelData(const std::string_view& modelDataPath)
{
	auto it = m_spModelDatas.find(modelDataPath.data());

	if (it != m_spModelDatas.end())
	{
		return (*it).second;
	}
	else
	{
		std::shared_ptr<ModelData>spModelData = std::make_shared<ModelData>();
		spModelData->Load(modelDataPath.data());
		m_spModelDatas[modelDataPath.data()] = spModelData;
		return spModelData;
	}
}

std::shared_ptr<ModelWork> AssetManager::GetModelWork(const std::string_view& modelWorkPath)
{
	auto it = m_spModelWorks.find(modelWorkPath.data());

	if (it != m_spModelWorks.end())
	{
		return (*it).second;
	}
	else
	{
		std::shared_ptr<ModelWork>spModelWork = std::make_shared<ModelWork>();
		spModelWork->Load(modelWorkPath.data());
		m_spModelWorks[modelWorkPath.data()] = spModelWork;
		return spModelWork;
	}
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string_view& texturePath)
{
	auto it = m_spTextures.find(texturePath.data());

	if (it != m_spTextures.end())
	{
		return (*it).second;
	}
	else
	{
		std::shared_ptr<Texture>spTexture = std::make_shared<Texture>();
		spTexture->Load(texturePath.data(), &GraphicsDevice::Instance(), true);
		m_spTextures[texturePath.data()] = spTexture;
		return spTexture;
	}
}

void AssetManager::AddMesh(const std::shared_ptr<Mesh>& spMesh, const std::string_view& meshName)
{
	auto it = m_spMeshes.find(meshName.data());

	if (it != m_spMeshes.end())
	{
		return;
	}

	m_spMeshes[meshName.data()] = spMesh;
}

std::shared_ptr<Mesh> AssetManager::GetMesh(const std::string_view& meshName)
{
	auto it = m_spMeshes.find(meshName.data());

	if (it != m_spMeshes.end())
	{
		return it->second;
	}

	assert(0 && "ShaderÇ™ë∂ç›ÇµÇ‹ÇπÇÒ");
	return nullptr;
}