#pragma once

class DepthStencilTexture :public Texture, public DepthStencil
{
public:
	DepthStencilTexture() = default;
	~DepthStencilTexture() = default;

	/// <summary>
	/// �[�x�e�N�X�`�������֐�
	/// </summary>
	/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
	/// <param name="size">�𑜓x</param>
	void Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);

	/// <summary>
	/// �V�F�[�_�ɃZ�b�g����֐�
	/// </summary>
	/// <param name="descIndex">���W�X�^�ԍ�</param>
	void Set(int descIndex) override;
};