#pragma once

struct AnimationData;
struct GLTFModel;

class ModelData
{
public:

	// �m�[�h�F���f�����`�����郁�b�V�����������߂̍ŏ��P��
	struct Node
	{
		std::string		m_name;				// �m�[�h��

		std::shared_ptr<Mesh>	m_spMesh;	// ���b�V��

		Math::Matrix	m_localTransform;			// �����̐e�{�[������̍s��
		Math::Matrix	m_worldTransform;			// ���_����̍s��
		Math::Matrix	m_boneInverseWorldMatrix;	// ���_����̋t�s��

		int					m_parent = -1;	// �e�C���f�b�N�X
		std::vector<int>	m_children;		// �q���ւ̃C���f�b�N�X���X�g

		//int		m_nodeIndex = -1;		// �擪���牽�Ԗڂ̃m�[�h���H
		int		m_boneIndex = -1;			// �{�[���m�[�h�̎��A�擪���牽�Ԗڂ̃{�[�����H

		bool	m_isSkinMesh = false;
	};

	ModelData();
	~ModelData();

	bool Load(const std::string& filename);

	void CreateNodes(std::shared_ptr<GLTFModel>& spGltfModel);							// �m�[�h�쐬
	void CreateMaterials(std::shared_ptr<GLTFModel>& spGltfModel, std::string& fileDir);	// �}�e���A���쐬
	void CreateAnimations(std::shared_ptr<GLTFModel>& spGltfModel);						// �A�j���[�V�����쐬

	//�A�N�Z�T
	const std::shared_ptr<Mesh> GetMesh(UINT index) const { return index < m_originalNodes.size() ? m_originalNodes[index].m_spMesh : nullptr; }

	Node* FindNode(std::string name)
	{
		for (auto&& node : m_originalNodes)
		{
			if (node.m_name == name)
			{
				return &node;
			}
		}

		return nullptr;
	}

	// �}�e���A���z��擾
	const std::vector<Material>& GetMaterials() const { return m_materials; }

	// �m�[�h�z��擾
	const std::vector<Node>& GetOriginalNodes() const { return m_originalNodes; }

	// �A�j���[�V�����f�[�^�擾
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	// ���ꂼ��̃m�[�h�̃C���f�b�N�X���X�g�擾
	const std::vector<int>& GetRootNodeIndices() const { return m_rootNodeIndices; }
	const std::vector<int>& GetBoneNodeIndices() const { return m_boneNodeIndices; }
	const std::vector<int>& GetMeshNodeIndices() const { return m_meshNodeIndices; }

	bool IsSkinMesh();

private:
	// ���
	void Release();

	//�}�e���A���z��
	std::vector<Material> m_materials;

	// �A�j���[�V�����f�[�^���X�g
	std::vector<std::shared_ptr<AnimationData>>	m_spAnimations;

	// �S�m�[�h�z��
	std::vector<Node>		m_originalNodes;
	// �S�m�[�h���ARoot�m�[�h�݂̂�Index�z��
	std::vector<int>		m_rootNodeIndices;
	// �S�m�[�h���A�{�[���m�[�h�݂̂�Index�z��
	std::vector<int>		m_boneNodeIndices;
	// �S�m�[�h���A���b�V�������݂���m�[�h�݂̂�Indexn�z��
	std::vector<int>		m_meshNodeIndices;
};

class ModelWork
{
public:

	// �m�[�h�F�������ω�����\���̂���f�[�^�A�����p�̖��O
	struct Node
	{
		std::string		m_name;				// �m�[�h��

		Math::Matrix	m_localTransform;	// �����̐e�{�[������̍s��
		Math::Matrix	m_worldTransform;	// ���_����̍s��

		void copy(const ModelData::Node& rNode)
		{
			m_name = rNode.m_name;

			m_localTransform = rNode.m_localTransform;
			m_worldTransform = rNode.m_worldTransform;
		}
	};

	// �R���X�g���N�^
	ModelWork() {}

	~ModelWork() {}

	// ���f���̓ǂݍ���
	bool Load(const std::string& fileName);

	// �S�g�̃{�[���̍s����v�Z
	void CalcNodeMatrices();

	// �L���t���O
	bool IsEnable() const { return (m_enable && m_spData); }
	void SetEnable(bool flag) { m_enable = flag; }

	// �m�[�h�����F������
	const ModelData::Node* FindDataNode(std::string& name) const;
	const Node* FindNode(std::string name) const;
	Node* FindWorkNode(std::string name);

	// �A�N�Z�T
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	inline const std::shared_ptr<ModelData> GetData() const { return m_spData; }
	// ���b�V���擾
	inline const std::shared_ptr<Mesh> GetMesh(UINT index) const { return index >= m_coppiedNodes.size() ? nullptr : GetDataNodes()[index].m_spMesh; }

	// �f�[�^�m�[�h���X�g�擾
	const std::vector<ModelData::Node>& GetDataNodes() const { assert(m_spData && "���f���f�[�^�����݂��܂���"); return m_spData->GetOriginalNodes(); }
	// �R�s�[�m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return m_coppiedNodes; }
	std::vector<Node>& WorkNodes() { return m_coppiedNodes; }

	// �A�j���[�V�����f�[�^�擾
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animName) const { return !m_spData ? nullptr : m_spData->GetAnimation(animName); }
	const std::shared_ptr<AnimationData> GetAnimation(int index) const { return !m_spData ? nullptr : m_spData->GetAnimation(index); }

	// ���f���ݒ�F�R�s�[�m�[�h�̐���
	void SetModel(const std::shared_ptr<ModelData>& rModel);

	const std::vector<int>& GetDrawMeshNodeIndices() const { return m_drawMeshNodeIndices; }
	const std::vector<int>& GetCollisionMeshNodeIndices() const { return m_collisionMeshNodeIndices; }

private:

	// �ċA�Ăяo���p�v�Z�֐�
	void recCalcNodeMatrices(int nodeIdx, int parentNodeIdx = -1);

	// �L��
	bool	m_enable = true;

	// ���f���f�[�^�ւ̎Q��
	std::shared_ptr<ModelData>	m_spData = nullptr;

	// �������ω�����\���̂���S�m�[�h�f�[�^�̃R�s�[�z��
	std::vector<Node>	m_coppiedNodes;

	// �S�m�[�h���A�R���W�������b�V�������݂���m�[�h�݂̂�Indexn�z��
	std::vector<int>		m_collisionMeshNodeIndices;
	// �S�m�[�h���A�`�悷��m�[�h�݂̂�Indexn�z��
	std::vector<int>		m_drawMeshNodeIndices;
};