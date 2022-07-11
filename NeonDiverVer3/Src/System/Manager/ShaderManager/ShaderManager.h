#pragma once

class Shader;

class ShaderManager
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	/// <summary>
	/// �V�F�[�_�[�̓o�^
	/// </summary>
	/// <param name="spShader">�o�^����V�F�[�_�[</param>
	/// <param name="shaderName">�V�F�[�_�[��</param>
	void ResisterShader(const std::shared_ptr<Shader>spShader,const std::string shaderName);

	/// <summary>
	/// �V�F�[�_�[�̎擾
	/// </summary>
	/// <param name="shaderName">�o�^�����V�F�[�_�[��</param>
	/// <returns>�V�F�[�_�[</returns>
	std::shared_ptr<Shader> GetShader(const std::string_view shaderName);

private:
	std::map<std::string, std::shared_ptr<Shader>>	m_spShaders;
};