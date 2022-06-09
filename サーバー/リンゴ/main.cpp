//==================================================
//
// ���C��CPP(main.cpp)
// Author�F�H������
//
//==================================================
//------------------------
// �C���N���[�h
//------------------------
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_stop = 1;

//------------------------
// ���C���֐�
//------------------------
void main(void)
{
#ifdef _DEBUG
	int size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG
	//------------------------
	// ������
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "�������Ɏ��s���܂���" << endl;
	}

	CTcpListener *pListenner = new CTcpListener;

	if (pListenner != NULL && pListenner->Init() == true)
	{
		thread th(StopOrSurver);

		th.detach();
		while (pListenner != NULL)
		{
			thread th(Accept, pListenner);

			th.detach();
			if (g_stop == 0)
			{
				break;
			}
		}
	}
	if (pListenner != NULL)
	{
		pListenner->Uninit();
		delete pListenner;
		pListenner = NULL;
	}

	//------------------------
	// �I��
	//------------------------
	cout << "�I�����܂��B"<< endl;
	cout << "�����L�[�������Ă��������B"<< endl;
	KeyWait();
	WSACleanup();
}

//------------------------
// �ڑ��҂�
//------------------------
void Accept(CTcpListener *listener)
{
	CCommunication *communication;

	communication = listener->Accept();

	thread th(CreateRoom, listener, communication);

	th.detach();
}

//------------------------
// ��������
//------------------------
void CreateRoom(CTcpListener *listener, CCommunication *player_01)
{
	fd_set fds, readfds;
	SOCKET maxfd, sock[MAX_PLAYER + 1];
	CCommunication *communication[MAX_PLAYER];
	CCommunicationData commu_data[MAX_PLAYER + 1];
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];
	char recv_data[MAX_COMMUDATA];
	char send_data[MAX_COMMUDATA];
	int recv;

	FD_ZERO(&readfds);

	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCommuData();
		data[count_playr]->Player.nNumber = count_playr + 1;
	}
	data[0] = commu_data[0].GetCommuData();
	for (int count_player = 0; count_player < MAX_PLAYER; count_player++)
	{
		communication[count_player] = listener->Accept();
		data[count_player + 1]->bConnect = true;
	}

	// �\�P�b�g�̓���
	sock[0] = player_01->GetSocket();
	sock[1] = communication[0]->GetSocket();
	sock[2] = communication[1]->GetSocket();
	sock[3] = communication[2]->GetSocket();

	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		// �Ď��\�P�b�g�̓o�^
		FD_SET(sock[nCnt], &readfds);
	}
	// �ő�\�P�b�g�̔���
	if (sock[0] > sock[1] &&
		sock[0] > sock[2] &&
		sock[0] > sock[3])
	{
		maxfd = sock[0];
	}
	else if (sock[1] > sock[0] &&
			 sock[1] > sock[2] &&
			 sock[1] > sock[3])
	{
		maxfd = sock[1];
	}
	else if (sock[2] > sock[0] &&
			 sock[2] > sock[1] &&
			 sock[2] > sock[3])
	{
		maxfd = sock[2];
	}
	else if (sock[3] > sock[0] &&
			 sock[3] > sock[1] &&
			 sock[3] > sock[2])
	{
		maxfd = sock[3];
	}

	while (recv != -1)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		// �\�P�b�g�̊Ď�
		select(maxfd + 1, &fds, NULL, NULL, NULL);

		// �v���C���[1��send����Ă�����
		if (FD_ISSET(sock[0], &fds))
		{
			recv = player_01->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[2��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[3��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[4��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
	}
	player_01->Uninit();
	for (int count_player = 0; count_player < MAX_PLAYER; count_player++)
	{
		communication[count_player]->Uninit();
	}
}

//------------------------
// �I������
//------------------------
void StopOrSurver(void)
{
	cin >> g_stop;
}

//------------------------
// �L�[���͑҂�
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}