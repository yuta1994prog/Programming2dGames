// Programming 2D Games
// Copyright (c) 2016 by:
// Yuta ISHII
// 「DirectX Fullscreen」 v1.0
// winmain.cpp

#define _CRTDBG_MAP_ALLOC	// メモリリークを検出するため
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>			// メモリリークを検出するため
#include <crtdbg.h>			// メモリリークを検出するため
#include "graphics.h"

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &,HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// グローバル変数
HINSTANCE hinst;
// Graphicsポインタ
Graphics *graphics;

//=====================================================
// Windows アプリケーションの開始点
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// デバッグビルドの場合，メモリリークをチェック
#if defined(DEBUG)|(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	MSG msg;
	HWND hwnd = NULL;
	// ウィンドウ作成
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;
	try {
		// Graphicsオブジェクトを作成
		graphics = new Graphics;
		// Graphicsを初期化，GameErrorをスロー
		graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
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
			else
				graphics->showBackbuffer();
		}
		SAFE_DELETE(graphics);	// 終了前にメモリ解放
		return msg.wParam;
	}
	catch (const GameError &err)
	{
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
	}
	SAFE_DELETE(graphics);	// 終了前にメモリ開放
	return 0;
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
		// 文字の入力を受け取る
		case WM_CHAR:
			switch (wParam)
			{
			case ESC_KEY:
				PostQuitMessage(0);
				return 0;
			}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//=====================================================
// ウィンドウ作成
// 戻り値:エラーの場合:false
//=====================================================
bool CreateMainWindow(HWND &hwnd,HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
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
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;
	// ウィンドウを作成
	hwnd = CreateWindow(CLASS_NAME, GAME_TITLE, style, CW_USEDEFAULT, CW_USEDEFAULT, GAME_WIDTH, GAME_HEIGHT, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	// ウィンドウの作成でエラーが発生した場合
	if (!hwnd)
		return false;
	if (!FULLSCREEN)	//ウィンドウ表示の場合
	{
		// クライアント領域がGAME_WIDTH * GAME_HEIGHTとなるようにウィンドウサイズを調整
		RECT clientRect;
		// ウィンドウのクライアント領域のサイズを取得
		GetClientRect(hwnd, &clientRect);
		MoveWindow(hwnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
	}
	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);
	// ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);
	return true;
}

////=====================================================
//// 現在のアプリケーションの別のインスタンスがないかチェック
//// 戻り値:別のインスタンスが見つかった:true
////        見つからなかった:false
////=====================================================
//
//bool AnotherInstance() 
//{
//	HANDLE outMutex;
//	// 固有の文字列を使ってミューテックスの作成を試みる
//	outMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161_XYZZY");
//	if (GetLastError()==ERROR_ALREADY_EXISTS)
//	{
//		return true;
//	}
//	return false;
//}