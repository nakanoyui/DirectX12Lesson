#pragma once

class KeyInfo
{
public:
	// コンストラクタ
	KeyInfo(const int& _vKey) :
		m_vKey(_vKey),
		m_pressTime(0),
		m_bFirst(true)
	{};

	// 長押し時間を加算
	inline void AddPressTime() {
		m_pressTime++;
	};

	// m_bFirstをfalseにする
	inline void NoFirst() {
		m_bFirst = false;
	}

	//============================================================
	//ゲッター 
	//============================================================
		// キーの種類
	inline int GetVKey() const {
		return m_vKey;
	}

	// キーが押されている時間
	inline unsigned GetPressTime() const {
		return m_pressTime;
	}

	// 生成されたフレームの時のみtrueとなる
	inline bool GetbFirst() const {
		return m_bFirst;
	}

private:
	//============================================================
	//メンバ変数
	//============================================================
	const int	m_vKey;			// キーの種類
	unsigned	m_pressTime;	// キーが押されている時間
	bool		m_bFirst;		// 生成されたフレームの時のみtrueとなる
};