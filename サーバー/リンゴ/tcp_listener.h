//====================================================
//
// �v���C���[�̃w�b�_�[�t�@�C��(tcp_server.h)
// Author : �H�� ����
//
//====================================================
#ifndef _TCP_LISTENER_H_
#define _TCP_LISTENER_H_

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "main.h"
#include "communication.h"

//-------------------------------
// Class�̒�`
//-------------------------------
class CTcpListener
{
public:
	CTcpListener();	// �f�t�H���g�R���X�g���N�^
	~CTcpListener();	// �f�t�H���g�f�X�g���N�^
	bool Init(void);	// ������
	CCommunication *Accept(void);	// �ڑ��҂�
	void Uninit(void);	// �I��
	bool GetIsInit(void) { return m_is_init; }	// ���������Ă��邩�̔���̎擾
	bool GetIsUninit(void) { return m_is_uninit; }	// �I�����Ă��邩�̔���̎擾
	int GetNumber(void) { return m_my_number; }	// �����̕����ԍ��̎擾
	void SetNumber(int number) { m_my_number = number; }	// �����̕����ԍ��̐ݒ�
private:
	SOCKET m_sockServer;	// �\�P�b�g
	int m_my_number;	//�@�����̕����ԍ�
	bool m_is_init;	// ���������Ă��邩
	bool m_is_uninit;	// �I�����Ă��邩
};

#endif // _TCP_CLIENT_H_