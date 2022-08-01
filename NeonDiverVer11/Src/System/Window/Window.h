#pragma once

class Window
{
public:

	//===================================
	// 取得・設定
	//===================================

	// ウィンドウハンドル取得
	HWND GetWndHandle() const { return m_hWnd; }
	// ウィンドウが存在する？
	bool IsCreated() const { return m_hWnd ? true : false; }
	// マウスホイールの変化量を取得
	int GetMouseWheelVal() const { return m_mouseWheelVal; }
	// インスタンスハンドル取得
	HINSTANCE GetInstanceHandle() const { return GetModuleHandle(0); }

	// クライアントサイズの設定
	void SetClientSize(int w, int h);

	//===================================
	// 初期化・解放
	//===================================

	bool Create(int clientWidth, int clientHeight, const std::string& titleName, const std::string& windowClassName);

	// 解放
	void Release();

	//
	~Window() {
		Release();
	}

	//===================================
	// 処理
	//===================================

	// ウィンドウメッセージを処理する
	//  戻り値：終了メッセージが来たらfalseが返る
	bool ProcessMessage();

private:

	// ウィンドウハンドル
	HWND	m_hWnd = nullptr;

	// マウスホイール値
	int		m_mouseWheelVal = 0;

	// ウィンドウ関数
	static LRESULT CALLBACK callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};