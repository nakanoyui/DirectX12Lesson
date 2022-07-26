#include "MeshManager.h"

MeshManager::MeshManager(GraphicsDevice* pGraphicsDevice)
	:m_pGraphicsDevice(pGraphicsDevice)
{
}

std::shared_ptr<Mesh> MeshManager::Add(const std::string& name)
{
	std::shared_ptr<Mesh> spMesh = std::make_shared<Mesh>();
	m_spMashes[name] = spMesh;
	m_meshInfos.push_back({name,spMesh});
	return spMesh;
}

std::shared_ptr<Mesh> MeshManager::GetMesh(const std::string& name)
{
	std::shared_ptr<Mesh> spMesh = m_spMashes[name];
	if (!spMesh)spMesh = m_spMashes["Cube"];
	return spMesh;
}
