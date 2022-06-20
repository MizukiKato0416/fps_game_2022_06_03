//====================================================
//
// �R�~���j�P�[�V�����f�[�^�̃w�b�_�[�t�@�C��(communicationdata.h)
// Author : �H�� ����
//
//====================================================
#ifndef _COMUNICASION_DATA_H_
#define _COMUNICASION_DATA_H_

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "main.h"
#include "d3dx9.h"
#include <string>

//-------------------------------
// �}�N����`
//-------------------------------
#define MAX_BULLET (30)
#define MAX_PLAYER (3)
#define MAX_MOTION_DATA (64)
#define MAX_DATA (1)
#define MAX_COMMU_DATA (2048)

//-------------------------------
// Class�̒�`
//-------------------------------
class CCommunicationData
{
public:
	//---------------------------
	// �\����
	//---------------------------
	typedef struct
	{
		int nNumber;	// �v���C���[�̃C���f�b�N�X
		float fMotionSpeed;	// ���[�V�����̃X�s�[�h
		char aMotion[MAX_DATA][MAX_MOTION_DATA];	// ���[�V�����̎��
		D3DXVECTOR3 Pos;	// �v���C���[�̈ʒu
		D3DXVECTOR3 Rot;	// �p�x
	} PLAYUER_DATA;

	typedef struct
	{
		D3DXVECTOR3 BigenPos;	// �e�̈ʒu
		D3DXVECTOR3 EndPos;	// �e�̈ʒu
		int nCollEnemy;	// ���Ă�����̔ԍ�
		bool bEnemyHit;	// ���������ǂ���
		bool bObjectHit;	// ���������ǂ���
	} BULLET_DATA;

	typedef struct
	{
		PLAYUER_DATA Player;	// �v���C���[�f�[�^
		BULLET_DATA Bullet[MAX_BULLET];	// �e�f�[�^
		bool bConnect;	// �ڑ��m�F
	} COMMUNICATION_DATA;

	CCommunicationData();	// �f�t�H���g�R���X�g���N�^
	~CCommunicationData();	// �f�t�H���g�f�X�g���N�^
	COMMUNICATION_DATA *GetCmmuData(void) { return &m_CommuData; }	// �f�[�^�̃Z�b�^�[
	void SetCmmuData(COMMUNICATION_DATA Data) { m_CommuData = Data; }	// �f�[�^�̃Q�b�_�[
private:
	COMMUNICATION_DATA m_CommuData;	// �f�[�^
};

#endif // _TCP_CLIENT_H_