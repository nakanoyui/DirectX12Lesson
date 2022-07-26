#pragma once

class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	/// <summary>
	/// �e�N�X�`���f�[�^�̓o�^
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X</param>
	void ResisterTexture(const std::string& filepath);

	/// <summary>
	/// �e�N�X�`���f�[�^�̎擾
	/// </summary>
	/// <param name="textureName">�e�N�X�`����</param>
	/// <returns>�e�N�X�`���f�[�^</returns>
	std::shared_ptr<Texture> GetTexture(const std::string_view textureName);

private:
	std::map<std::string, std::shared_ptr<Texture>> m_spTextureList;
};