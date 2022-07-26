#pragma once

class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	/// <summary>
	/// テクスチャデータの登録
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	void ResisterTexture(const std::string& filepath);

	/// <summary>
	/// テクスチャデータの取得
	/// </summary>
	/// <param name="textureName">テクスチャ名</param>
	/// <returns>テクスチャデータ</returns>
	std::shared_ptr<Texture> GetTexture(const std::string_view textureName);

private:
	std::map<std::string, std::shared_ptr<Texture>> m_spTextureList;
};