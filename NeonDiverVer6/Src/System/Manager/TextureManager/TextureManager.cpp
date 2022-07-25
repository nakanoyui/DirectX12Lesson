#include "TextureManager.h"

void TextureManeger::ResisterTexture(const std::string& filepath)
{
	std::shared_ptr<Texture> spTexture = std::make_shared<Texture>();

	int last_pos = filepath.find_last_of('/');
	last_pos++; // find_last_of関数だと/を考慮しちゃうから/を飛ばす
	std::string result = filepath.substr(last_pos, filepath.size() - last_pos);

	auto it = m_spTextureList.find(filepath);

	if (it == m_spTextureList.end())
	{
		if (!spTexture->Load(filepath.data(),&GraphicsDevice::Instance()))
		{
			assert(0 && "Textureの登録に失敗しました");
		}
	}
	else
	{
		//	すでにテクスチャがロードされてる
		return;
	}

	m_spTextureList[result] = spTexture;
}

std::shared_ptr<Texture> TextureManeger::GetTexture(const std::string_view textureName)
{
	auto it = m_spTextureList.find(textureName.data());

	if (it != m_spTextureList.end())
	{
		return it->second;
	}

	assert(0 && "Textureが存在しません");
	return nullptr;
}