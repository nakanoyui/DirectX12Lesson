#pragma once

#include "KeyInfo/KeyInfo.h"

class KeyManager
{
public:
	KeyManager() = default;
	~KeyManager() = default;

	void Update();

	// �L�[��������Ă��邩��Ԃ�
	//  vKey�@�@�@�F�L�[�̎��
	//  bLongPress�Ftrue  �E�E�E�������ł� �������� �悤�ɂȂ�
	//�@ �@�@�@�@    false�E�E�E�������� �������Ȃ� �悤�ɂȂ�
	//  bFirstMode�Ftrue �E�E�E���̃t���[�����ł܂��L�[��������Ă��Ȃ����̂�true��Ԃ��悤�ɂȂ�
	//                         false�E�E�E�ʏ�ʂ�̋���
	// �߂�l�@�@  �FbLongPress = true  : �L�[��������Ă�����true,
	//			            �L�[��������Ă��Ȃ�������false
	//	        bLongPress = false : �L�[�������ꂽ1��ڂ̃t���[���ł̂�true,
	//			            ����ȊO�ł�false
	const bool GetKeyState(const int& vKey, const bool& bLongPress = false, const bool& bFirstMode = false);

	// �L�[�𒷉������Ă��鎞�Ԃ�Ԃ�
	// vKey�F�L�[�̎��
	// unit �F���Ԃ̒P��
	//�@�@�@'f'�E�E�E�t���[����
	//�@�@�@'s'�E�E�E�b��
	//�߂�l�F���������Ă��鎞��
	//�@�@�@   ������Ă��Ȃ��Ƃ�,0��Ԃ�
	const int GetLongPressKeyTime(const int& vKey, const char& unit = 'f');

private:
	std::list<std::unique_ptr<KeyInfo>> m_keyInfoList;	// ���������Ă���L�[�̏��
};