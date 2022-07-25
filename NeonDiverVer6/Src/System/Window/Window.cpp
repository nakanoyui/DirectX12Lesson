#include "Window.h"

void Window::SetClientSize(int w, int h)
{
	RECT rcWnd, rcCli;

	GetWindowRect(m_hWnd, &rcWnd); // ウィンドウのRECT取得
	GetClientRect(m_hWnd, &rcCli); // クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow(m_hWnd,
		rcWnd.left,// X座標
		rcWnd.top,// Y座標
		w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
		h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
		TRUE);
}

bool Window::Create(int clientWidth, int clientHeight, const std::string& titleName, const std::string& windowClassName)
{
	// インスタンスハンドル取得
	HINSTANCE hInst = GetInstanceHandle();

	//===================================================================
	// メインウィンドウ作成
	//===================================================================

	std::wstring wndClsName = sjis_to_wide(windowClassName);

	//ウィンドウクラスの定義
	WNDCLASSEX wc;											// ウィンドウクラスの定義用
	wc.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
	wc.style = 0;											// スタイル
	wc.lpfnWndProc = &Window::callWindowProc;				// ウインドウ関数
	wc.cbClsExtra = 0;										// エキストラクラス情報 
	wc.cbWndExtra = 0;										// エキストラウィンドウ情報
	wc.hInstance = hInst;									// インスタンスハンドル
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			// ラージアイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		// スモールアイコン 
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			// マウスカーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色 
	wc.lpszMenuName = nullptr;								// メインメニュー名
	wc.lpszClassName = wndClsName.c_str();					// ウィンドウクラス名

	//ウィンドウクラスの登録
	if (!RegisterClassEx(&wc)) 
	{
		return false;
	}

	//ウィンドウの作成
	m_hWnd = CreateWindow(
		wndClsName.c_str(),									// ウィンドウクラス名
		sjis_to_wide(titleName).c_str(),					// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,				// ウィンドウタイプを標準タイプに	
		0,													// ウィンドウの位置（Ｘ座標）
		0,													// ウィンドウの位置（Ｙ座標）						
		clientWidth,										// ウィンドウの幅
		clientHeight,										// ウィンドウの高さ			
		nullptr,											// 親ウィンドウのハンドル
		nullptr,											// メニューのハンドル
		hInst,												// インスタンスハンドル 
		this);												// 追加情報

	if (m_hWnd == nullptr) 
	{
		return false;
	}

	// クライアントのサイズを設定
	SetClientSize(clientWidth, clientHeight);

	//===================================================================
	//ウィンドウの表示
	//===================================================================
	ShowWindow(m_hWnd, SW_SHOW);

	//ウィンドウの更新
	UpdateWindow(m_hWnd);

	return true;
}

void Window::Release()
{
	if (m_hWnd) 
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

bool Window::ProcessMessage()
{
	m_mouseWheelVal = 0;

	// メッセージ取得
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT) {
			return false;
		}

		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

LRESULT Window::callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// ウィンドウプロパティから、GameWindowクラスのインスタンスを取得
	Window* pThis = (Window*)GetProp(hWnd, L"GameWindowInstance");

	// nullptrの場合は、デフォルト処理を実行
	if (pThis == nullptr)
	{
		switch (message) {
		case WM_CREATE:
		{
			// CreateWindow()で渡したパラメータを取得
			CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;
			Window* window = (Window*)createStruct->lpCreateParams;

			// ウィンドウプロパティにこのクラスのインスタンスアドレスを埋め込んでおく
			// 次回から、pThis->WindowProcの方へ処理が流れていく
			SetProp(hWnd, L"GameWindowInstance", window);

		}
		return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	// インスタンス側のWindow関数を実行する
	return pThis->WindowProc(hWnd, message, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	//===================================
	//メッセージによって処理を選択
	//===================================
	switch (message) {
		// ホイールスクロール時
	case WM_MOUSEWHEEL:
	{
		m_mouseWheelVal = (short)HIWORD(wParam);
	}
	break;
	// Xボタンが押された
	case WM_CLOSE:
		// 破棄
		Release();
		break;
		// ウィンドウ破棄直前
	case WM_DESTROY:
		RemoveProp(hWnd, L"GameWindowInstance");
		PostQuitMessage(0);
		break;
	default:
		// メッセージのデフォルト処理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
