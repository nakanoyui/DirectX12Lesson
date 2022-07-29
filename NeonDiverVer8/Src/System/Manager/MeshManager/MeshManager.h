#pragma once

struct MeshInfo
{
	std::string m_name;
	std::shared_ptr<Mesh> m_spMesh = nullptr;
};

class MeshManager
{
public:
	MeshManager(GraphicsDevice*pGraphicsDevice);
	~MeshManager() = default;
	std::shared_ptr<Mesh>Add(const std::string& name);
	std::shared_ptr<Mesh>GetMesh(const std::string&name);
private:
	GraphicsDevice* m_pGraphicsDevice = nullptr;
	std::map<std::string, std::shared_ptr<Mesh>> m_spMashes;
	std::vector<MeshInfo> m_meshInfos;
};