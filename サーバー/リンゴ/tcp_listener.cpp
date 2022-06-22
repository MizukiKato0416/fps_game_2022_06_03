//====================================================
//
// �v���C���[�̃w�b�_�[�t�@�C��(tcp_client.cpp)
// Author : �H�� ����
//
//====================================================
//-------------------------------
// �C���N���[�h
//-------------------------------
#include "tcp_listener.h"

//-------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------
CTcpListener::CTcpListener()
{
	m_sockServer = INVALID_SOCKET;
}

//-------------------------------
// �f�t�H���g�f�X�g���N�^
//-------------------------------
CTcpListener::~CTcpListener()
{

}

//-------------------------------
// ������
//-------------------------------
bool CTcpListener::Init(void)
{
	FILE *pFile;
	char aFile[64];
	int nPort;
	int nWait;

	pFile = fopen("data/severdata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "PORT_NUM") == 0) // PORT_NUM�̕��������������
			{
				fscanf(pFile, "%s", &aFile[1]);
				fscanf(pFile, "%d", &nPort);
			}
			if (strcmp(&aFile[0], "MAX_WAIT") == 0) // MAX_WAIT�̕��������������
			{
				fscanf(pFile, "%s", &aFile[1]);
				fscanf(pFile, "%d", &nWait);
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPT�̕��������������
			{
				break;
			}
		}
	}
	else
	{
		cout << "�T�[�o�[�f�[�^���ǂݎ��܂���ł����B" << endl;
	}

	fclose(pFile);

	//------------------------
	// �\�P�b�g�쐬
	//------------------------
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sockServer == INVALID_SOCKET)
	{
		cout << "�ڑ��҂��\�P�b�g�����܂���ł����B" << endl;
		return false;
	}

	//------------------------
	// ��t����
	//------------------------
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;	// �ǂ̒ʐM��
	addr.sin_port = htons(nPort);	// �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = INADDR_ANY;	// �N�ł��A�N�Z�X�ł���

	bind(m_sockServer,
		(struct sockaddr*)&addr,
		sizeof(addr));

	listen(m_sockServer, nWait);	// �ő�ҋ@��

	return true;
}

//-------------------------------
// �ڑ���t
//-------------------------------
CCommunication * CTcpListener::Accept(void)
{
	CCommunication *pSendRecv = new CCommunication;

	//------------------------
	// �ڑ��҂�
	//------------------------
	struct sockaddr_in clientAddr;
	int nLength = sizeof(clientAddr);

	SOCKET sock = accept(	m_sockServer,
							(struct sockaddr*)&clientAddr,
							&nLength);
	if (sock == INVALID_SOCKET)
	{
		cout << "�ڑ��ł��܂���ł����B�B" << endl;
	}
	else
	{
		cout << "�ڑ��o���܂����B" << endl;
	}
	if (pSendRecv != NULL)
	{
		pSendRecv->Init(sock);
	}

	return pSendRecv;
}

//-------------------------------
// �I��
//-------------------------------
void CTcpListener::Uninit(void)
{
	if (m_sockServer == INVALID_SOCKET)
	{
		return;
	}

	//------------------------
	// �ڑ��ؒf
	//------------------------
	cout << "�ڑ���ؒf���܂��B"<< endl;
	closesocket(m_sockServer);	// �ڑ���t�p�\�P�b�g
	m_sockServer = INVALID_SOCKET;
}