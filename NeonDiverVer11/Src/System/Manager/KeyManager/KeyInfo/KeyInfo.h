#pragma once

class KeyInfo
{
public:
	// �R���X�g���N�^
	KeyInfo(const int& _vKey) :
		m_vKey(_vKey),
		m_pressTime(0),
		m_bFirst(true)
	{};

	// ���������Ԃ����Z
	inline void AddPressTime() {
		m_pressTime++;
	};

	// m_bFirst��false�ɂ���
	inline void NoFirst() {
		m_bFirst = false;
	}

	//============================================================
	//�Q�b�^�[ 
	//============================================================
		// �L�[�̎��
	inline int GetVKey() const {
		return m_vKey;
	}

	// �L�[��������Ă��鎞��
	inline unsigned GetPressTime() const {
		return m_pressTime;
	}

	// �������ꂽ�t���[���̎��̂�true�ƂȂ�
	inline bool GetbFirst() const {
		return m_bFirst;
	}

private:
	//============================================================
	//�����o�ϐ�
	//============================================================
	const int	m_vKey;			// �L�[�̎��
	unsigned	m_pressTime;	// �L�[��������Ă��鎞��
	bool		m_bFirst;		// �������ꂽ�t���[���̎��̂�true�ƂȂ�
};