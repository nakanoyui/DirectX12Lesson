#include "ShaderManager.h"

#include "System/Graphics/Shader/Shader.h"

void ShaderManager::ResisterShader(const std::shared_ptr<Shader>spShader,const std::string shaderName)
{
	auto it = m_spShaders.find(shaderName);

	if (it != m_spShaders.end())
	{
		return;
	}

	m_spShaders[shaderName] = spShader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string_view shaderName)
{
	auto it = m_spShaders.find(shaderName.data());

	if (it != m_spShaders.end())
	{
		return it->second;
	}

	assert(0 && "Shader‚ª‘¶İ‚µ‚Ü‚¹‚ñ");
	return nullptr;
}