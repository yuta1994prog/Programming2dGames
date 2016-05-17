// Programming 2D Games
// Copyright (c) 2016 by:
// Yuta ISHII
// Hello World v1.0
// winmain.cpp

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// �O���[�o���ϐ�
HINSTANCE hinst;

// �萔
const char CLASS_NAME[] = "WinMain";
const char APP_TITLE[] = "Hello World!";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

//=====================================================
// Windows �A�v���P�[�V�����̊J�n�_
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;
	// ���C���̃��b�Z�[�W���[�v
	int done = 0;
	while (!done)
	{
		// PeekMessage��Windows���b�Z�[�W�����邩���e�X�g�����u���b�N���\�b�h
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// �I�����b�Z�[�W�����m
			if (msg.message == WM_QUIT)
				done = 1;
			// ���b�Z�[�W���f�R�[�h����WinProc�ɓn��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

//=====================================================
// �E�B���h�E�C�x���g�R�[���o�b�N�֐�
//=====================================================
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			// Windows�ɂ��̃v���O�������I������悤�ɓ`����
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//=====================================================
// �E�B���h�E�쐬
// �߂�l:�G���[�̏ꍇ:false
//=====================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	// �E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肷��
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
	// �E�B���h�E�N���X��o�^
	// �G���[���CRegisterClassEx��0��߂�
	if (RegisterClassEx(&wcx) == 0)
		return false;
	// �E�B���h�E���쐬
	hwnd = CreateWindow(CLASS_NAME, APP_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	// �E�B���h�E�̍쐬�ŃG���[�����������ꍇ
	if (!hwnd)
		return false;
	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);
	// �E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);
	return true;
}