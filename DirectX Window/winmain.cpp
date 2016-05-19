// Programming 2D Games
// Copyright (c) 2016 by:
// Yuta ISHII
// �uDirectX Window�v v1.0
// winmain.cpp

#define _CRTDBG_MAP_ALLOC	// ���������[�N�����o���邽��
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>			// ���������[�N�����o���邽��
#include <crtdbg.h>			// ���������[�N�����o���邽��
#include "graphics.h"

// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// �O���[�o���ϐ�
HINSTANCE hinst;
HDC hdc;			// �f�o�C�X�R���e�L�X�g�ւ̃n���h��
TCHAR ch = ' ';		// ���͂��ꂽ����
RECT rect;			// Rectangle�\����
PAINTSTRUCT ps;		// WM_PAINT�Ŏg�p�����

// �萔
const char CLASS_NAME[] = "Keyboard";
const char APP_TITLE[] = "DirectX Window";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 300;

Graphics *graphics;

//=====================================================
// Windows �A�v���P�[�V�����̊J�n�_
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �f�o�b�O�r���h�̏ꍇ�C���������[�N���`�F�b�N
#if defined(DEBUG)|(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	MSG msg;
	// �E�B���h�E�쐬
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;
	try {
		// Graphics�I�u�W�F�N�g���쐬
		graphics = new Graphics;
		// Graphics���������CGameError���X���[
		graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
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
			else
				graphics->showBackbuffer();
		}
		SAFE_DELETE(graphics);	// �I���O�Ƀ��������
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
	SAFE_DELETE(graphics);	// �I���O�Ƀ������J��
	return 0;
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
		// �����̓��͂��󂯎��
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
// �E�B���h�E�쐬
// �߂�l:�G���[�̏ꍇ:false
//=====================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	// �E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肷��
	wcx.cbSize = sizeof(wcx);				// �\���̂̃T�C�Y
	wcx.style = CS_HREDRAW | CS_VREDRAW;	// �E�B���h�E�T�C�Y�ύX���ɍĕ`��
	wcx.lpfnWndProc = WinProc;				// �E�B���h�E�v���V�[�W�����w��
	wcx.cbClsExtra = 0;						// �g���N���X�������Ȃ�
	wcx.cbWndExtra = 0;						// �g���E�B���h�E�������Ȃ�
	wcx.hInstance = hInstance;				// �C���X�^���X�ւ̃n���h��
	wcx.hIcon = NULL;						// �傫���A�C�R��
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// ���O��`����Ă�����J�[�\��
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// ���F�̔w�i
	wcx.lpszMenuName = NULL;				// ���j���[���\�[�X�̖��O
	wcx.lpszClassName = CLASS_NAME;			// �E�B���h�E�N���X�̖��O
	wcx.hIconSm = NULL;						// �������A�C�R��
	// �E�B���h�E�N���X��o�^
	// �G���[���CRegisterClassEx��0��߂�
	if (RegisterClassEx(&wcx) == 0)	// �G���[�̏ꍇ
		return false;
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;
	// �E�B���h�E���쐬
	hwnd = CreateWindow(CLASS_NAME, APP_TITLE, style, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	// �E�B���h�E�̍쐬�ŃG���[�����������ꍇ
	if (!hwnd)
		return false;
	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);
	// �E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);
	return true;
}

//=====================================================
// ���݂̃A�v���P�[�V�����̕ʂ̃C���X�^���X���Ȃ����`�F�b�N
// �߂�l:�ʂ̃C���X�^���X����������:true
//        ������Ȃ�����:false
//=====================================================

bool AnotherInstance() 
{
	HANDLE outMutex;
	// �ŗL�̕�������g���ă~���[�e�b�N�X�̍쐬�����݂�
	outMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161_XYZZY");
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		return true;
	}
	return false;
}