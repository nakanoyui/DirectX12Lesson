#include "ThreadManager.h"

void ThreadManager::WaitForSynchronization()
{
	for (auto& spThread : m_spTreads)
	{
		if (spThread->joinable())
		{
			spThread->join();
		}
	}

	m_spTreads.clear();
}