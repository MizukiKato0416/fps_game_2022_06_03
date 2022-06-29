//==================================================
//
// ���C���w�b�_�[(main.h)
// Author�F�H������
//
//==================================================
#ifndef _MAIN_H_
#define _MAIN_H_
//------------------------
// �C���N���[�h
//------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include <vector>
#include "d3dx9.h"

using namespace std;

//------------------------
// �O���錾
//------------------------
class CCommunication;
class CTcpListener;

//------------------------
// �}�N����`
//------------------------
#define MAX_IP_NUM (16)

//------------------------
// ���C�u�����̃����N
//------------------------
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"d3d9.lib")			//�`��ɕK�v
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����

//------------------------
// �v���g�^�C�v�錾
//------------------------
void KeyWait(void);	// �L�[���͑҂�
void CreateRoom(vector<CCommunication*> communication, int room_num);	// ������������
void AllAccept(CTcpListener* listener, int room_num);	// ������������
void CountUp(void);	// �J�E���g�A�b�v�p
void Init(void);	// ������

#endif // !_MAIN_H_