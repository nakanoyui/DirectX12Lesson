#pragma once

struct AnimationData;
struct GLTFModel;

class ModelData
{
public:

	// ノード：モデルを形成するメッシュを扱うための最小単位
	struct Node
	{
		std::string		m_name;				// ノード名

		std::shared_ptr<Mesh>	m_spMesh;	// メッシュ

		Math::Matrix	m_localTransform;			// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;			// 原点からの行列
		Math::Matrix	m_boneInverseWorldMatrix;	// 原点からの逆行列

		int					m_parent = -1;	// 親インデックス
		std::vector<int>	m_children;		// 子供へのインデックスリスト

		//int		m_nodeIndex = -1;		// 先頭から何番目のノードか？
		int		m_boneIndex = -1;			// ボーンノードの時、先頭から何番目のボーンか？

		bool	m_isSkinMesh = false;
	};

	ModelData();
	~ModelData();

	bool Load(const std::string& filename);

	void CreateNodes(std::shared_ptr<GLTFModel>& spGltfModel);							// ノード作成
	void CreateMaterials(std::shared_ptr<GLTFModel>& spGltfModel, std::string& fileDir);	// マテリアル作成
	void CreateAnimations(std::shared_ptr<GLTFModel>& spGltfModel);						// アニメーション作成

	//アクセサ
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

	// マテリアル配列取得
	const std::vector<Material>& GetMaterials() const { return m_materials; }

	// ノード配列取得
	const std::vector<Node>& GetOriginalNodes() const { return m_originalNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	// それぞれのノードのインデックスリスト取得
	const std::vector<int>& GetRootNodeIndices() const { return m_rootNodeIndices; }
	const std::vector<int>& GetBoneNodeIndices() const { return m_boneNodeIndices; }
	const std::vector<int>& GetMeshNodeIndices() const { return m_meshNodeIndices; }

	bool IsSkinMesh();

private:
	// 解放
	void Release();

	//マテリアル配列
	std::vector<Material> m_materials;

	// アニメーションデータリスト
	std::vector<std::shared_ptr<AnimationData>>	m_spAnimations;

	// 全ノード配列
	std::vector<Node>		m_originalNodes;
	// 全ノード中、RootノードのみのIndex配列
	std::vector<int>		m_rootNodeIndices;
	// 全ノード中、ボーンノードのみのIndex配列
	std::vector<int>		m_boneNodeIndices;
	// 全ノード中、メッシュが存在するノードのみのIndexn配列
	std::vector<int>		m_meshNodeIndices;
};

class ModelWork
{
public:

	// ノード：活動中変化する可能性のあるデータ、検索用の名前
	struct Node
	{
		std::string		m_name;				// ノード名

		Math::Matrix	m_localTransform;	// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;	// 原点からの行列

		void copy(const ModelData::Node& rNode)
		{
			m_name = rNode.m_name;

			m_localTransform = rNode.m_localTransform;
			m_worldTransform = rNode.m_worldTransform;
		}
	};

	// コンストラクタ
	ModelWork() {}

	~ModelWork() {}

	// モデルの読み込み
	bool Load(const std::string& fileName);

	// 全身のボーンの行列を計算
	void CalcNodeMatrices();

	// 有効フラグ
	bool IsEnable() const { return (m_enable && m_spData); }
	void SetEnable(bool flag) { m_enable = flag; }

	// ノード検索：文字列
	const ModelData::Node* FindDataNode(std::string& name) const;
	const Node* FindNode(std::string name) const;
	Node* FindWorkNode(std::string name);

	// アクセサ
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	inline const std::shared_ptr<ModelData> GetData() const { return m_spData; }
	// メッシュ取得
	inline const std::shared_ptr<Mesh> GetMesh(UINT index) const { return index >= m_coppiedNodes.size() ? nullptr : GetDataNodes()[index].m_spMesh; }

	// データノードリスト取得
	const std::vector<ModelData::Node>& GetDataNodes() const { assert(m_spData && "モデルデータが存在しません"); return m_spData->GetOriginalNodes(); }
	// コピーノードリスト取得
	const std::vector<Node>& GetNodes() const { return m_coppiedNodes; }
	std::vector<Node>& WorkNodes() { return m_coppiedNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animName) const { return !m_spData ? nullptr : m_spData->GetAnimation(animName); }
	const std::shared_ptr<AnimationData> GetAnimation(int index) const { return !m_spData ? nullptr : m_spData->GetAnimation(index); }

	// モデル設定：コピーノードの生成
	void SetModel(const std::shared_ptr<ModelData>& rModel);

	const std::vector<int>& GetDrawMeshNodeIndices() const { return m_drawMeshNodeIndices; }
	const std::vector<int>& GetCollisionMeshNodeIndices() const { return m_collisionMeshNodeIndices; }

private:

	// 再帰呼び出し用計算関数
	void recCalcNodeMatrices(int nodeIdx, int parentNodeIdx = -1);

	// 有効
	bool	m_enable = true;

	// モデルデータへの参照
	std::shared_ptr<ModelData>	m_spData = nullptr;

	// 活動中変化する可能性のある全ノードデータのコピー配列
	std::vector<Node>	m_coppiedNodes;

	// 全ノード中、コリジョンメッシュが存在するノードのみのIndexn配列
	std::vector<int>		m_collisionMeshNodeIndices;
	// 全ノード中、描画するノードのみのIndexn配列
	std::vector<int>		m_drawMeshNodeIndices;
};