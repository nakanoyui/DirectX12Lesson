#include "DrawManager.h"

void DrawManager::Create(const std::string_view& drawListName)
{
	auto it = m_spDrawLists.find(drawListName.data());
	
	if (it != m_spDrawLists.end())
	{
		return;
	}

	auto spDrawList = std::make_shared<DrawList>();
	m_spDrawLists[drawListName.data()] = spDrawList;
}

const std::shared_ptr<DrawList>& DrawManager::GetDrawList(const std::string_view& drawListName)
{
	auto it = m_spDrawLists.find(drawListName.data());

	if (it != m_spDrawLists.end())
	{
		return it->second;
	}

	assert(0 && "DrawList‚ª‘¶Ý‚µ‚Ü‚¹‚ñ");
	return it->second;
}