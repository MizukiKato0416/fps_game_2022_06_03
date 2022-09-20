//=======================================================================
//���C��(�V�X�e���n)����
//Aouthor: �����@����
//=======================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//�C���N���[�h�t�@�C��
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"						//�`��ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ή������p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�����ɕK�v
#include <time.h>
#include <XInput.h>
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <utility>

using namespace std;

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")			//�`��ɕK�v
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")		//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"dinput8.lib")		//���͏����ɕK�v
#pragma comment(lib,"xinput.lib")		//pad���͏����ɕK�v
#pragma comment(lib, "winmm.lib")		//fps�\���ɕK�v
#pragma comment(lib, "XAudio2.lib")		//3D�I�[�f�B�I

//========================================================
//�}�N����`
//========================================================
#define SCREEN_WIDTH (1280)		//�E�B���h�E�̕�
#define SCREEN_HEIGHT (720)		//�E�B���h�E�̍���
#define FPS (60)

//========================================================
//��ʁi���[�h�j�̎��
//========================================================

//========================================================
//�v���g�^�C�v�錾
//========================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//�s����g�����x�N�g���̕ϊ�
D3DXVECTOR3 VTransform(D3DXVECTOR3 InV, D3DXMATRIX InM);
bool calcRaySphere(float lx, float ly, float lz, float vx, float vy, float vz, float px, float py, float pz, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
bool calcRayInfCilinder(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// ���C�Ɖ~���̊ђʓ_���Z�o
bool calcRayCapsule(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);	// ��P1P2P3�̓��ς��Z�o

#ifdef _DEBUG
int GetFPS(void);
#endif

#endif // !_MAIN_H_