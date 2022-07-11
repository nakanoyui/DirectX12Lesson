#include "ThreadManager.h"

void ThreadManager::WaitForSynchronization()
{
	for (auto& spThread : m_spTreads)
	{
		spThread->join();
	}
}