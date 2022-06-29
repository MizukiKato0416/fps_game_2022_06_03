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

// lx, ly, lz : ���C�̎n�_
// vx, vy, vz : ���C�̕����x�N�g��
// px, py, pz : ���̒��S�_�̍��W
// r : ���̔��a
// q1x, q1y, q1z: �ՓˊJ�n�_�i�߂�l�j
// q2x, q2y, q2z: �ՓˏI���_�i�߂�l�j

bool calcRaySphere(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float px, float py, float pz,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// lx, ly, lz : ���C�̎n�_
// vx, vy, vz : ���C�̕����x�N�g��
// p1x, p1y, p1z: �~������1�_
// p2x, p2y, p2z: �~�����̂���1�_
// r : �~���̔��a
// q1x, q1y, q1z: �ђʊJ�n�_�i�߂�l�j
// q2x, q2y, q2z: �ђʏI���_�i�߂�l�j

bool calcRayInfCilinder(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
// lx, ly, lz : ���C�̎n�_
// vx, vy, vz : ���C�̕����x�N�g��
// p1x, p1y, p1z: �J�v�Z�����̒[�_P1
// p2x, p2y, p2z: �J�v�Z�����̒[�_P2
// r : �J�v�Z���̔��a
// q1x, q1y, q1z: �ђʊJ�n�_�i�߂�l�j
// q2x, q2y, q2z: �ђʏI���_�i�߂�l�j
bool calcRayCapsule(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// ��P1P2P3�̓��ς��Z�o
// x1, y1, z1: �_P1
// x2, y2, z2: �_P2
// x3, y3, z3: �_P3
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);


#endif // !_MAIN_H_