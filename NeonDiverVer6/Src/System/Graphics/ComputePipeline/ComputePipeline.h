#pragma once

class ComputePipeline
{
public:
	/// <summary>
	/// �R���X�g���N�^���V�F�[�_�[�R���p�C��
	/// </summary>
	/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
	/// <param name="filepath">�R���p�C������V�F�[�_�[�t�@�C���̖��O</param>
	ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filepath);
	~ComputePipeline();

	/// <summary>
	/// �����֐�
	/// </summary>
	/// <param name="range">�V�F�[�_�[�ň���RootParam�̔z��</param>
	void Create(const std::vector<RangeType>& range);

	/// <summary>
	/// �V�F�[�_�[���Z�b�g����֐�
	/// </summary>
	void Set();
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ComPtr<ID3DBlob>m_pCSBlob = nullptr;
	ComPtr<ID3D12PipelineState>m_pPipeline = nullptr;
	std::unique_ptr<RootSignature> m_upRootSignature = nullptr;
};