#pragma once

struct EmitterData
{
	Math::Vector3 m_startPosition;
	Math::Vector3 m_startScale = Math::Vector3(Constant::One_F);
	float m_startRotate = Constant::Zero_F;

	float MAX_LIFE = 180;
	Math::Vector3 m_velocity = Math::Vector3(Constant::One_F);
	Math::Vector3 m_startForce = Math::Vector3(Constant::One_F);
	Math::Vector3 m_endForce = Math::Vector3(Constant::One_F);
	Math::Vector3 m_randomMin = Math::Vector3(-Constant::One_F, Constant::One_F, -Constant::One_F);
	Math::Vector3 m_randomMax = Math::Vector3(Constant::One_F);

	Math::Vector3 m_startColor = Math::Vector3(Constant::One_F);
	Math::Vector3 m_endColor = Math::Vector3(Constant::One_F);

	bool m_sizeByLifeTime = false;
};

struct ParticleData
{
	int m_index;			// ���g�̃G�~�b�^�[�Ǘ��ԍ�
	int m_state = Constant::Zero_I;		// ���
	float m_life;			// ���C�t

	// ��b�f�[�^
	Math::Vector3 m_pos;
	Math::Vector3 m_scale;
	float m_rotate;
	Math::Vector3 m_velocity;
	Math::Vector3 m_color;
};

class GPUParticleManager
{
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GPUParticleManager(GraphicsDevice* pGraphicsDevice);
	~GPUParticleManager();
	UINT GetMaxParticleValue() { return MaxParticleValue; }
	UINT GetNextParticleOffset() { return m_nextParticleOffset; }
	UINT GetUAVNextOffset() { return m_uavNextOffset; }
	UINT GetSRVNextOffset() { return m_srvNextOffset; }
	UINT GetEmitterNextOffset() { return m_emitterNextOffset++; }
	UINT GetEmitterDataSRVValue() { return m_emitterDataSRVValue; }
	ComPtr<ID3D12Resource> GetParticleBuffer() { return m_pParticleBuffer; }
	ComPtr<ID3D12Resource> GetUpdateParticleBuffer() { return m_pUpdateParticleBuffer; }
	ComPtr<ID3D12Resource> GetEmitterParticleBuffer() { return m_pEmitterParticleBuffer; }
	ComPtr<ID3D12DescriptorHeap> GetHeap() { return m_pHeap; }

	void IncrementParticleOffset(UINT value) { m_nextParticleOffset += value; }
	void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc);
	void CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

	GraphicsDevice* GetDevice() { return m_pGraphicsDevice; }

	void Update();
	void Dispatch(ComputePipeline* pShader);

private:
	const UINT MaxParticleValue;		// �A�v���P�[�V������������p�[�e�B�N�����
	UINT m_nextParticleOffset;			// ���̃G�~�b�^�[��������p�[�e�B�N���o�b�t�@�[�̐擪�A�h���X���w���ϐ�
	const UINT MaxEmitterValue;			// �G�~�b�^�[�̍ő吔
	UINT m_uavNextOffset;				// �R���s���[�g�V�F�[�_�[�ň���UAV�̎��̃A�h���X���w�����߂̕ϐ�
	UINT m_srvNextOffset;				// �`��ň���SRV�̎��̃A�h���X���w�����߂̕ϐ�

	UINT m_emitterDataSRVValue;			// �G�~�b�^�[SRV�̔ԍ�
	UINT m_emitterNextOffset;			// �����̃G�~�b�^�[�ԍ����w���ϐ�

	ComPtr<ID3D12Resource> m_pParticleBuffer = nullptr;
	ComPtr<ID3D12Resource> m_pUpdateParticleBuffer = nullptr;
	ComPtr<ID3D12Resource> m_pEmitterParticleBuffer = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pHeap = nullptr;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
};