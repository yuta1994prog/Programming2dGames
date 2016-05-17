// Programming 2D Games
// Copyright (c) 2016 by:
// Yuta ISHII
// Hello World v1.0
// winmain.cpp

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// グローバル変数
HINSTANCE hinst;

// 定数
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World!";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

//=====================================================
// Windows アプリケーションの開始点
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
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
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			// Windowsにこのプログラムを終了するように伝える
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIcon = NULL;
	// ウィンドウクラスを登録
	// エラー時，RegisterClassExは0を戻す
	if (RegisterClassEx(&wcx) == 0)
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