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

//-------------------------------
// �}�N����`
//-------------------------------
#define MAX_BULLET (30)
#define MAX_PLAYER (3)
#define MAX_COMMU_DATA (512)

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
		string Motion;	// ���[�V�����̎��
		D3DXVECTOR3 Pos;	// �v���C���[�̈ʒu
		D3DXVECTOR3 Rot;	// �p�x
	} PLAYUER_DATA;

	typedef struct
	{
		D3DXVECTOR3 Pos;	// �e�̈ʒu
		D3DXVECTOR3 Rot;	// �e�̊p�x
		int nCollPlayer;	// ���Ă�����̔ԍ�
		bool bHit;			// ���������ǂ���
	} BULLET_DATA;

	typedef struct
	{
		PLAYUER_DATA Player;	// �v���C���[�f�[�^
		BULLET_DATA Bullet[MAX_BULLET];	// �e�f�[�^
		bool bConnect = false;	// �ڑ��m�F
	} COMMUNICATION_DATA;

	CCommunicationData();	// �f�t�H���g�R���X�g���N�^
	~CCommunicationData();	// �f�t�H���g�f�X�g���N�^
	COMMUNICATION_DATA *GetCmmuData(void) { return &m_CommuData; }	// �f�[�^�̃Z�b�^�[
	void SetCmmuData(COMMUNICATION_DATA Data) { m_CommuData = Data; }	// �f�[�^�̃Q�b�_�[
private:
	COMMUNICATION_DATA m_CommuData;	// �f�[�^
};

#endif // _TCP_CLIENT_H_