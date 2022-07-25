#pragma once

#include "KeyInfo/KeyInfo.h"

class KeyManager
{
public:
	KeyManager() = default;
	~KeyManager() = default;

	void Update();

	// キーが押されているかを返す
	//  vKey　　　：キーの種類
	//  bLongPress：true  ・・・長押しでも 反応する ようになる
	//　 　　　　    false・・・長押しで 反応しない ようになる
	//  bFirstMode：true ・・・そのフレーム内でまだキーが押されていない時のみtrueを返すようになる
	//                         false・・・通常通りの挙動
	// 戻り値　　  ：bLongPress = true  : キーが押されていたらtrue,
	//			            キーが押されていなかったらfalse
	//	        bLongPress = false : キーが押された1回目のフレームでのみtrue,
	//			            それ以外ではfalse
	const bool GetKeyState(const int& vKey, const bool& bLongPress = false, const bool& bFirstMode = false);

	// キーを長押ししている時間を返す
	// vKey：キーの種類
	// unit ：時間の単位
	//　　　'f'・・・フレーム数
	//　　　's'・・・秒数
	//戻り値：長押ししている時間
	//　　　   押されていないとき,0を返す
	const int GetLongPressKeyTime(const int& vKey, const char& unit = 'f');

private:
	std::list<std::unique_ptr<KeyInfo>> m_keyInfoList;	// 長押ししているキーの情報
};