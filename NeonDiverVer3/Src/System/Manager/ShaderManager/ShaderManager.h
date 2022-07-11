#pragma once

class Shader;

class ShaderManager
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	/// <summary>
	/// シェーダーの登録
	/// </summary>
	/// <param name="spShader">登録するシェーダー</param>
	/// <param name="shaderName">シェーダー名</param>
	void ResisterShader(const std::shared_ptr<Shader>spShader,const std::string shaderName);

	/// <summary>
	/// シェーダーの取得
	/// </summary>
	/// <param name="shaderName">登録したシェーダー名</param>
	/// <returns>シェーダー</returns>
	std::shared_ptr<Shader> GetShader(const std::string_view shaderName);

private:
	std::map<std::string, std::shared_ptr<Shader>>	m_spShaders;
};