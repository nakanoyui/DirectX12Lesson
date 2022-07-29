#include "KeyManager.h"

// 更新
void KeyManager::Update()
{
	// 保存しているキー情報を全て捜査
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		// 保存しているキーが今も押されている場合、
		if (GetAsyncKeyState((*it)->GetVKey()) & 0x8000)
		{
			(*it)->AddPressTime();	// 経過時間を増加
			(*it)->NoFirst();		// 長押しで反応しない場合falseを返すように
			it++;
		}
		// もう押されていない場合はこのキーをリストから削除
		else
		{
			(*it).reset();
			it = m_keyInfoList.erase(it);
		}
	}
}

//キーが押されているかを返す
const bool KeyManager::GetKeyState(const int& vKey, const bool& bLongPress, const bool& bFirstMode)
{
	// 保存しているキー情報を全て捜査
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		if ((*it)->GetVKey() == vKey)	// 見つかった場合
		{
			if (!bFirstMode)
			{
				// 長押しでも反応するならtrueを返す
				if (bLongPress) return true;

				// 長押しで反応しない場合
				// 生成された時と同フレームの場合true、それ以外ではfalseを返す
				else return (*it)->GetbFirst();
			}
			else
			{
				// 生成された時と同フレームの場合falseを返す
				if ((*it)->GetbFirst()) return false;

				// 長押しでも反応するならtrueを返す
				if (bLongPress) return true;

				// 長押しで反応しない場合、falseを返す
				else return false;
			}
		}

		it++;
	}

	// 見つからなかったら
	if (GetAsyncKeyState(vKey) & 0x8000)	// 引数のキーが押されていたら
	{
		m_keyInfoList.push_back(std::unique_ptr<KeyInfo>(new KeyInfo(vKey)));	// キー情報を生成
		return true;															// trueを返す
	}

	return false;
}

//キーを長押ししている時間を返す
const int KeyManager::GetLongPressKeyTime(const int& vKey, const char& unit)
{
	// 保存しているキー情報を全て捜査
	auto it = m_keyInfoList.begin();
	while (it != m_keyInfoList.end())
	{
		if ((*it)->GetVKey() == vKey)	// 見つかった場合
		{
			//unitに合わせて値を返す
			//フレーム数
			if (unit == 'f') return (*it)->GetPressTime();

			//現在のFPS値で割っておおよその秒数を算出する
			if (unit == 's') return int((*it)->GetPressTime() / (float)Application::Instance().GetGameProcess()->m_fpsController.m_nowfps);
		}

		it++;
	}

	// 見つからなかったら0を返す
	return 0;
}