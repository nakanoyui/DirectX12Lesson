#include "Model.h"
#include "../GLTFLoader/GLTFLoader.h"

//�R���X�g���N�^�[
ModelData::ModelData()
{
}

//�f�X�g���N�^�[
ModelData::~ModelData()
{
	Release();
}

//���[�h�֐�
bool ModelData::Load(const std::string& filename)
{
	Release();

	std::string fileDir = GetDirFromPath(filename);

	std::shared_ptr<GLTFModel> spGltfModel = LoadGLTFModel(filename);	// ������ɂ��������̂Ŗ��̕ύX
	if (spGltfModel == nullptr) { return false; }

	CreateNodes(spGltfModel);

	CreateMaterials(spGltfModel, fileDir);

	CreateAnimations(spGltfModel);

	return true;
}

// �m�[�h�쐬
void ModelData::CreateNodes(std::shared_ptr<GLTFModel>& spGltfModel)
{
	m_originalNodes.resize(spGltfModel->Nodes.size());

	for (UINT i = 0; i < spGltfModel->Nodes.size(); i++)
	{
		// ���͌��m�[�h
		const GLTFNode& rSrcNode = spGltfModel->Nodes[i];

		// �o�͐�̃m�[�h�Q��
		Node& rDstNode = m_originalNodes[i];

		if (rSrcNode.IsMesh)
		{
			// ���b�V���쐬
			rDstNode.m_spMesh = std::make_shared<Mesh>();

			if (rDstNode.m_spMesh)
			{
				rDstNode.m_spMesh->Create(&GraphicsDevice::Instance(), rSrcNode.Mesh.Vertices, rSrcNode.Mesh.Faces, rSrcNode.Mesh.Subsets, rSrcNode.Mesh.IsSkinMesh);
			}

			// ���b�V���m�[�h���X�g�ɃC���f�b�N�X�o�^
			m_meshNodeIndices.push_back(i);
		}

		// �m�[�h���Z�b�g
		rDstNode.m_name = rSrcNode.Name;

		rDstNode.m_localTransform = rSrcNode.LocalTransform;
		rDstNode.m_worldTransform = rSrcNode.WorldTransform;
		rDstNode.m_boneInverseWorldMatrix = rSrcNode.InverseBindMatrix;

		rDstNode.m_isSkinMesh = rSrcNode.Mesh.IsSkinMesh;

		rDstNode.m_boneIndex = rSrcNode.BoneNodeIndex;

		rDstNode.m_parent = rSrcNode.Parent;
		rDstNode.m_children = rSrcNode.Children;
	}

	for (UINT nodeIdx = 0; nodeIdx < spGltfModel->Nodes.size(); nodeIdx++)
	{
		// ���[�g�m�[�h��Index���X�g
		if (spGltfModel->Nodes[nodeIdx].Parent == -1) { m_rootNodeIndices.push_back(nodeIdx); }

		// �{�[���m�[�h��Index���X�g
		int boneIdx = spGltfModel->Nodes[nodeIdx].BoneNodeIndex;

		if (boneIdx >= 0)
		{
			if (boneIdx >= (int)m_boneNodeIndices.size()) { m_boneNodeIndices.resize(boneIdx + 1); }

			m_boneNodeIndices[boneIdx] = nodeIdx;
		}
	}
}

// �}�e���A���쐬
void ModelData::CreateMaterials(std::shared_ptr<GLTFModel>& spGltfModel, std::string& fileDir)
{
	//�}�e���A���z����󂯎���T�C�Y�̃��������m��
	m_materials.resize(spGltfModel->Materials.size());

	for (UINT i = 0; i < m_materials.size(); ++i)
	{
		//src = source�̗�
		//dst = destination�̗�
		const GLTFMaterial& rSrcMaterial = spGltfModel->Materials[i];
		Material& rDstMaterial = m_materials[i];

		//���O
		rDstMaterial.Name = rSrcMaterial.Name;

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		//��{�F
		rDstMaterial.BaseColor = rSrcMaterial.BaseColor;

		// ��{�F�e�N�X�`��
		bool isLoadColorTex = false;
		rDstMaterial.BaseColorTex = std::make_shared<Texture>();

		if (!rSrcMaterial.BaseColorTexture.empty())
		{
			isLoadColorTex = rDstMaterial.BaseColorTex->Load(fileDir + rSrcMaterial.BaseColorTexture,&GraphicsDevice::Instance());
		}

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// �@���}�b�v
		bool isLoadNomalTex = false;
		rDstMaterial.NormalTex = std::make_shared<Texture>();

		if (!rSrcMaterial.NormalTexture.empty())
		{
			isLoadNomalTex = rDstMaterial.NormalTex->Load(fileDir + rSrcMaterial.NormalTexture, &GraphicsDevice::Instance());
		}

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// �������E�e��
		rDstMaterial.Metallic = rSrcMaterial.Metallic;
		rDstMaterial.Roughness = rSrcMaterial.Roughness;

		// �������E�e���}�b�v
		bool isLoadRoughMetalTex = false;
		rDstMaterial.MetallicRoughnessTex = std::make_shared<Texture>();

		if (!rSrcMaterial.MetallicRoughnessTexture.empty())
		{
			isLoadRoughMetalTex = rDstMaterial.MetallicRoughnessTex->Load(fileDir + rSrcMaterial.MetallicRoughnessTexture, &GraphicsDevice::Instance());
		}

		// ���Ȕ����E�G�~�b�V�u
		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		rDstMaterial.Emissive = rSrcMaterial.Emissive;

		// ���Ȕ����E�G�~�b�V�u
		if (!rSrcMaterial.EmissiveTexture.empty())
		{
			rDstMaterial.EmissiveTex = std::make_shared<Texture>();

			if (!rDstMaterial.EmissiveTex->Load(fileDir + rSrcMaterial.EmissiveTexture, &GraphicsDevice::Instance()))
			{
				rDstMaterial.EmissiveTex = nullptr;
			}
		}

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// BaseColorTexture���ǂݍ��߂Ȃ������ꍇ�́A����ɔ��摜���g�p
		if (!isLoadColorTex)
		{
			rDstMaterial.BaseColorTex->SetTexBuffer(GraphicsDevice::Instance().GetWhiteTex(), &GraphicsDevice::Instance());
		}

		// NormalTexture���ǂݍ��߂Ȃ������ꍇ�́A����Ƀf�t�H���g�@���摜���g�p
		if (!isLoadNomalTex)
		{
			rDstMaterial.NormalTex->SetTexBuffer(GraphicsDevice::Instance().GetWhiteTex(), &GraphicsDevice::Instance());
		}

		// RoughMetalTexture���ǂݍ��߂Ȃ������ꍇ�́A����ɔ��摜���g�p
		if (!isLoadRoughMetalTex)
		{
			rDstMaterial.MetallicRoughnessTex->SetTexBuffer(GraphicsDevice::Instance().GetWhiteTex(), &GraphicsDevice::Instance());
		}
	}
}

// �A�j���[�V�����쐬
void ModelData::CreateAnimations(std::shared_ptr<GLTFModel>& spGltfModel)
{
	// �A�j���[�V�����f�[�^
	m_spAnimations.resize(spGltfModel->Animations.size());

	for (UINT i = 0; i < m_spAnimations.size(); ++i)
	{
		const GLTFAnimationData& rSrcAnimation = *spGltfModel->Animations[i];

		m_spAnimations[i] = std::make_shared<AnimationData>();
		AnimationData& rDstAnimation = *(m_spAnimations[i]);

		rDstAnimation.m_name = rSrcAnimation.m_name;

		rDstAnimation.m_maxLength = rSrcAnimation.m_maxLength;

		rDstAnimation.m_nodes.resize(rSrcAnimation.m_nodes.size());

		for (UINT j = 0; j < rDstAnimation.m_nodes.size(); ++j)
		{
			rDstAnimation.m_nodes[j].m_nodeOffset = rSrcAnimation.m_nodes[j]->m_nodeOffset;
			rDstAnimation.m_nodes[j].m_translations = rSrcAnimation.m_nodes[j]->m_translations;
			rDstAnimation.m_nodes[j].m_rotations = rSrcAnimation.m_nodes[j]->m_rotations;
			rDstAnimation.m_nodes[j].m_scales = rSrcAnimation.m_nodes[j]->m_scales;
		}
	}
}

// �A�j���[�V�����f�[�^�擾�F�����񌟍�
const std::shared_ptr<AnimationData> ModelData::GetAnimation(const std::string& animName) const
{
	for (auto&& anim : m_spAnimations)
	{
		if (anim->m_name == animName)
		{
			return anim;
		}
	}

	return nullptr;
}

// �A�j���[�V�����f�[�^�擾�F�ԍ��w��
const std::shared_ptr<AnimationData> ModelData::GetAnimation(UINT index) const
{
	return index >= m_spAnimations.size() ? nullptr : m_spAnimations[index];
}

// ���
void ModelData::Release()
{
	m_materials.clear();
	m_originalNodes.clear();

	m_rootNodeIndices.clear();
	m_boneNodeIndices.clear();
	m_meshNodeIndices.clear();
}

bool ModelData::IsSkinMesh()
{
	for (auto& node : m_originalNodes)
	{
		if (node.m_isSkinMesh) { return true; }
	}

	return false;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 
// ModelWork
// 
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////

bool ModelWork::Load(const std::string& fileName)
{
	// ���f���̓ǂݍ���
	std::shared_ptr<ModelData> data = std::make_shared<ModelData>();

	if (!data->Load(fileName)) { return false; }

	// ���f���̃Z�b�g
	SetModel(data);

	return true;
}

const ModelData::Node* ModelWork::FindDataNode(std::string& name) const
{
	if (m_spData == nullptr) { return nullptr; }

	return m_spData->FindNode(name);
}

// �m�[�h�����F������
const ModelWork::Node* ModelWork::FindNode(std::string name) const
{
	for (auto&& node : m_coppiedNodes)
	{
		if (node.m_name == name)
		{
			return &node;
		}
	}

	return nullptr;
}

// �σm�[�h�����F������
ModelWork::Node* ModelWork::FindWorkNode(std::string name)
{
	for (auto&& node : m_coppiedNodes)
	{
		if (node.m_name == name)
		{
			return &node;
		}
	}

	return nullptr;
}

// ���f���ݒ�F�R�s�[�m�[�h�̐���
void ModelWork::SetModel(const std::shared_ptr<ModelData>& rModel)
{
	m_spData = rModel;

	UINT nodeSize = static_cast<UINT>(rModel->GetOriginalNodes().size());

	m_coppiedNodes.resize(nodeSize);

	// �m�[�h�̃R�s�[�𐶐�
	for (UINT i = 0; i < nodeSize; ++i)
	{
		m_coppiedNodes[i].copy(rModel->GetOriginalNodes()[i]);
	}

	// ���b�V�������m�[�h�Ɋւ���U�蕪������
	for (int index : rModel->GetMeshNodeIndices())
	{
		// �����蔻��p�m�[�h����
		if (rModel->GetOriginalNodes()[index].m_name.find("COL") == std::string::npos)
		{
			m_drawMeshNodeIndices.push_back(index);
		}
		else
		{
			m_collisionMeshNodeIndices.push_back(index);
		}
	}

	if (!m_collisionMeshNodeIndices.size())
	{
		m_collisionMeshNodeIndices = rModel->GetMeshNodeIndices();
	}
}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ���[�g�m�[�h����e�m�[�h�̍s����v�Z���Ă���
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
void ModelWork::CalcNodeMatrices()
{
	if (!m_spData) { assert(0 && "���f���̂Ȃ��m�[�h�s��v�Z"); return; }

	// �S�{�[���s�����������
	for (auto&& nodeIdx : m_spData->GetRootNodeIndices())
	{
		recCalcNodeMatrices(nodeIdx);
	}
}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// �m�[�h�s��v�Z�p�̍ċN�p�֐�
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
void ModelWork::recCalcNodeMatrices(int nodeIdx, int parentNodeIdx)
{
	auto& data = m_spData->GetOriginalNodes()[nodeIdx];
	auto& work = m_coppiedNodes[nodeIdx];

	// �e�Ƃ̍s�������
	if (parentNodeIdx >= 0)
	{
		auto& parent = m_coppiedNodes[data.m_parent];

		work.m_worldTransform = work.m_localTransform * parent.m_worldTransform;
	}
	// �e�����Ȃ��ꍇ�͐e�͎������g�Ƃ���
	else
	{
		work.m_worldTransform = work.m_localTransform;
	}

	for (auto childNodeIdx : data.m_children)
	{
		recCalcNodeMatrices(childNodeIdx, nodeIdx);
	}
}