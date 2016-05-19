// Programming 2D Games
// Copyright (c) 2016 by:
// Yuta ISHII
// 「Character Input」 v1.0
// winmain.cpp

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// グローバル変数
HINSTANCE hinst;
HDC hdc;			// デバイスコンテキストへのハンドル
TCHAR ch = ' ';		// 入力された文字
RECT rect;			// Rectangle構造体
PAINTSTRUCT ps;		// WM_PAINTで使用される

// 定数
const char CLASS_NAME[] = "Keyboard";
const char APP_TITLE[] = "Character Input";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 300;

//=====================================================
// Windows アプリケーションの開始点
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	// ウィンドウ作成
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
	// メインのメッセージループ
	int done = 0;
	while (!done)
	{
		// PeekMessageはWindowsメッセージがあるかをテストする非ブロックメソッド
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 終了メッセージを検知
			if (msg.message == WM_QUIT)
				done = 1;
			// メッセージをデコードしてWinProcに渡す
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

//=====================================================
// ウィンドウイベントコールバック関数
//=====================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			// Windowsにこのプログラムを終了するように伝える
			PostQuitMessage(0);
			return 0;
		// 文字の入力を受け取る
		case WM_CHAR:
			switch (wParam)
			{
				case 0x08:	// BackSpace
				case 0x09:	// Tab
				case 0x0A:	// ラインフィード
				case 0x0D:	// キャリッジリターン
				case 0x1B:	// Esc
					MessageBeep((UINT)-1);
					return 0;
				default:					// 表示可能な文字
					ch = (TCHAR)wParam;		// 文字を取得
					// WM_PAINTを強制的に発生させる
					InvalidateRect(hwnd, NULL, TRUE);
					return 0;
			}
		case WM_PAINT: // ウィンドウを再描写する必要がある場合，デバイスコンテキストへのハンドルを取得
			hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rect);		// ウィンドウの矩形を所得
			// 文字を表示
			TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
			EndPaint(hwnd, &ps);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

//=====================================================
// ウィンドウ作成
// 戻り値:エラーの場合:false
//=====================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	// ウィンドウクラスの構造体をメインウィンドウを記述するパラメータで設定する
	wcx.cbSize = sizeof(wcx);				// 構造体のサイズ
	wcx.style = CS_HREDRAW | CS_VREDRAW;	// ウィンドウサイズ変更時に再描写
	wcx.lpfnWndProc = WinProc;				// ウィンドウプロシージャを指す
	wcx.cbClsExtra = 0;						// 拡張クラスメモリなし
	wcx.cbWndExtra = 0;						// 拡張ウィンドウメモリなし
	wcx.hInstance = hInstance;				// インスタンスへのハンドル
	wcx.hIcon = NULL;						// 大きいアイコン
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// 事前定義されている矢印カーソル
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// 黒色の背景
	wcx.lpszMenuName = NULL;				// メニューリソースの名前
	wcx.lpszClassName = CLASS_NAME;			// ウィンドウクラスの名前
	wcx.hIconSm = NULL;						// 小さいアイコン
	// ウィンドウクラスを登録
	// エラー時，RegisterClassExは0を戻す
	if (RegisterClassEx(&wcx) == 0)	// エラーの場合
		return false;
	// ウィンドウを作成
	hwnd = CreateWindow(CLASS_NAME, APP_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	// ウィンドウの作成でエラーが発生した場合
	if (!hwnd)
		return false;
	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);
	// ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);
	return true;
}

//=====================================================
// 現在のアプリケーションの別のインスタンスがないかチェック
// 戻り値:別のインスタンスが見つかった:true
//        見つからなかった:false
//=====================================================

bool AnotherInstance() 
{
	HANDLE outMutex;
	// 固有の文字列を使ってミューテックスの作成を試みる
	outMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161_XYZZY");
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		return true;
	}
	return false;
}