//====================================================
//
// �v���C���[�̃w�b�_�[�t�@�C��(tcp_client.h)
// Author : �H�� ����
//
//====================================================
#ifndef _NET_WORK_MANAGER_H_
#define _NET_WORK_MANAGER_H_

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "main.h"
#include "communicationdata.h"
#include "tcp_client.h"
#include <thread>

//-------------------------------
// �O���錾
//-------------------------------
class CTcpClient;

//-------------------------------
// Class�̒�`
//-------------------------------
class CNetWorkManager
{
public:
	CNetWorkManager();	// �f�t�H���g�R���X�g���N�^
	~CNetWorkManager();	// �f�t�H���g�f�X�g���N�^
	void Init(void);	// ����������
	void Uninit(void);	// �I������
	void CreateThread(void) { thread recv_th(Recv); recv_th.detach(); }	// �X���b�h�̐���
	static int Send(char *pSendData, int nSendDataSize) { return m_communication->Send(pSendData, nSendDataSize); }	// ���M(���M�f�[�^�i�[��, ���M�f�[�^�T�C�Y)
	static void Recv(void);	// ��M
	static CCommunicationData *GetPlayerData(void) { return &m_player_data; }	// �v���C���[���̎擾
	static vector<CCommunicationData> GetEnemyData(void) { return m_enemy_data; }	// �G���̎擾
	static CTcpClient *GetCommunication(void) { return m_communication; }				//�ʐM�N���X�擾����
private:
	static CCommunicationData m_player_data;	// �v���C���[�f�[�^
	static vector<CCommunicationData> m_enemy_data;	// �G�f�[�^
	static CTcpClient *m_communication;	// �ʐM�N���X
};

#endif // _TCP_CLIENT_H_