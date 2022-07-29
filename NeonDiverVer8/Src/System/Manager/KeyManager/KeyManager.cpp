#include "KeyManager.h"

// �X�V
void KeyManager::Update()
{
	// �ۑ����Ă���L�[����S�đ{��
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		// �ۑ����Ă���L�[������������Ă���ꍇ�A
		if (GetAsyncKeyState((*it)->GetVKey()) & 0x8000)
		{
			(*it)->AddPressTime();	// �o�ߎ��Ԃ𑝉�
			(*it)->NoFirst();		// �������Ŕ������Ȃ��ꍇfalse��Ԃ��悤��
			it++;
		}
		// ����������Ă��Ȃ��ꍇ�͂��̃L�[�����X�g����폜
		else
		{
			(*it).reset();
			it = m_keyInfoList.erase(it);
		}
	}
}

//�L�[��������Ă��邩��Ԃ�
const bool KeyManager::GetKeyState(const int& vKey, const bool& bLongPress, const bool& bFirstMode)
{
	// �ۑ����Ă���L�[����S�đ{��
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		if ((*it)->GetVKey() == vKey)	// ���������ꍇ
		{
			if (!bFirstMode)
			{
				// �������ł���������Ȃ�true��Ԃ�
				if (bLongPress) return true;

				// �������Ŕ������Ȃ��ꍇ
				// �������ꂽ���Ɠ��t���[���̏ꍇtrue�A����ȊO�ł�false��Ԃ�
				else return (*it)->GetbFirst();
			}
			else
			{
				// �������ꂽ���Ɠ��t���[���̏ꍇfalse��Ԃ�
				if ((*it)->GetbFirst()) return false;

				// �������ł���������Ȃ�true��Ԃ�
				if (bLongPress) return true;

				// �������Ŕ������Ȃ��ꍇ�Afalse��Ԃ�
				else return false;
			}
		}

		it++;
	}

	// ������Ȃ�������
	if (GetAsyncKeyState(vKey) & 0x8000)	// �����̃L�[��������Ă�����
	{
		m_keyInfoList.push_back(std::unique_ptr<KeyInfo>(new KeyInfo(vKey)));	// �L�[���𐶐�
		return true;															// true��Ԃ�
	}

	return false;
}

//�L�[�𒷉������Ă��鎞�Ԃ�Ԃ�
const int KeyManager::GetLongPressKeyTime(const int& vKey, const char& unit)
{
	// �ۑ����Ă���L�[����S�đ{��
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		if ((*it)->GetVKey() == vKey)	// ���������ꍇ
		{
			//unit�ɍ��킹�Ēl��Ԃ�
			//�t���[����
			if (unit == 'f') return (*it)->GetPressTime();

			//���݂�FPS�l�Ŋ����Ă����悻�̕b�����Z�o����
			if (unit == 's') return int((*it)->GetPressTime() / (float)Application::Instance().GetGameProcess()->m_fpsController.m_nowfps);
		}

		it++;
	}

	// ������Ȃ�������0��Ԃ�
	return 0;
}