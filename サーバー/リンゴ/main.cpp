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
#include <vector>
#include <assert.h>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

//------------------------
// �}�N����`
//------------------------
#define MAX_ROOM (5)	// �ő啔����

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_stop;	// �T�[�o�X�g�b�v����p
int g_accept_count;	// �A�N�Z�v�g������
int g_active_surver;	// �A�N�e�B�u�ȃT�[�o�[
int	g_old_active_surver;	// �O��A�N�e�B�u��������T�[�o�[
vector<CTcpListener*> g_listenner;	// �T�[�o�[

//------------------------
// ���C���֐�
//------------------------
void main(void)
{
#ifdef _DEBUG
	// �f�o�b�O�p�T�C�Y�m�F
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG

	g_stop = 0;
	g_accept_count = 1;
	g_active_surver = 0;
	g_old_active_surver = 0;
	g_listenner.push_back(new CTcpListener);

	//------------------------
	// WSA�̏�����
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "�������Ɏ��s���܂���" << endl;
	}

	// �T�[�o��~�̃L�[�{�[�h����p�֐��̃X���b�h����
	thread select_th(StopOrSurver);

	// �X���b�h��؂藣��
	select_th.detach();

	// �ŏ��̃T�[�o�[�𗧂Ă�
	{
		g_listenner[0]->Init();

		// ���������X���b�h
		thread room_create_th(CreateRoom, g_listenner[0], g_accept_count);

		// �X���b�h��؂藣��
		room_create_th.detach();

		// �����������Z
		g_accept_count++;

		// �O�̃T�[�o�[
		g_old_active_surver = g_active_surver;

		// �T�[�o�[������
		g_active_surver++;

		// �T�[�o�[�̐���
		g_listenner.push_back(new CTcpListener);
	}

	while (g_stop != 1)
	{
		// ���������Ăď��������Ă��Ȃ��ĕ��������ő吔�ȉ��őO�̃T�[�o�[�����������I���Ă�����
		if (g_listenner[g_active_surver] != nullptr &&
			g_listenner[g_active_surver]->GetIsInit() == false &&
			g_accept_count <= MAX_ROOM &&
			g_listenner[g_old_active_surver]->GetCreate() == true)
		{
			// ����������
			g_listenner[g_active_surver]->Init();

			// ���������X���b�h
			thread th(CreateRoom, g_listenner[g_active_surver], g_accept_count);

			// �X���b�h��؂藣��
			th.detach();

			// �����������Z
			g_accept_count++;

			// �O�̃T�[�o�[
			g_old_active_surver = g_active_surver;

			// �T�[�o�[������
			g_active_surver++;

			// �T�[�o�[�̐���
			g_listenner.push_back(new CTcpListener);
		}
		// �T�C�Y���擾
		int size = g_listenner.size();
		for (int count_surver = 0; count_surver < size; count_surver++)
		{
			// init���Ă���uninit���Ă�����
			if (g_listenner[count_surver]->GetIsInit() == true &&
				g_listenner[count_surver]->GetIsUninit() == true &&
				g_listenner[count_surver]->GetSocket() == INVALID_SOCKET)
			{
				// ����
				delete g_listenner[count_surver];

				// �z�񂩂����
				g_listenner.erase(g_listenner.begin() + count_surver);

				// �T�[�o�[�̐���
				g_listenner.push_back(new CTcpListener);

				// �O�̃T�[�o�[
				g_old_active_surver = count_surver - 1;

				// �T�[�o�[������
				g_active_surver = size - 1;
			}
		}
	}
	// �T�C�Y���擾
	int size = g_listenner.size();
	// �S�Ă�listener���폜
	for (int count_listener = 0; count_listener < size; count_listener++)
	{
		if (g_listenner[count_listener] != nullptr)
		{
			g_listenner[count_listener]->Uninit();
			delete g_listenner[count_listener];
			g_listenner[count_listener] = nullptr;
		}
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
// ��������
//------------------------
void CreateRoom(CTcpListener *listener, int room_num)
{
	fd_set fds, readfds;	// select�p�ϐ�
	SOCKET maxfd, sock[MAX_PLAYER + 1];	// �Ď��\�P�b�g
	CCommunication *communication[MAX_PLAYER + 1];	// �ʐM�N���X
	CCommunicationData commu_data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^�N���X
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^
	char recv_data[MAX_COMMU_DATA];	// ���V�[�u�p
	char send_data[MAX_COMMU_DATA];	// �Z���h�p
	int recv = 1;	// �ŏ��̃��[�v�ɓ���ׂ̏�����

	// select�p�ϐ��̏�����
	FD_ZERO(&readfds);

	// �����ԍ��̐ݒ�
	listener->SetNumber(room_num);

	// ���Z�b�g
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// �v���C���[�ԍ��̐U�蕪��
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
	}

	// �ʐM�҂�
	for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
	{
		communication[count_player] = listener->Accept();
		data[count_player]->bConnect = true;
	}

	if (communication[0] != nullptr &&
		communication[1] != nullptr &&
		communication[2] != nullptr &&
		communication[3] != nullptr &&
		data[0]->bConnect == true &&
		data[1]->bConnect == true &&
		data[2]->bConnect == true &&
		data[3]->bConnect == true)
	{
		// ���������I����
		listener->SetCreate(true);
	}

	// �������f�[�^�̑��M
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	// �\�P�b�g�̓���
	sock[0] = communication[0]->GetSocket();
	sock[1] = communication[1]->GetSocket();
	sock[2] = communication[2]->GetSocket();
	sock[3] = communication[3]->GetSocket();

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

	// ���V�[�u�łȂ�����Ȃ�������
	while (recv > 0)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		// �\�P�b�g�̊Ď�
		select(maxfd + 1, &fds, NULL, NULL, NULL);

		// �I�����߂�������
		if (g_stop == 1)
		{
			break;
		}

		// �v���C���[1��send����Ă�����
		if (FD_ISSET(sock[0], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[0], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[0].SetCmmuData(*data[0]);

			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[2��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[1], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[1].SetCmmuData(*data[1]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[3��send����Ă�����
		if (FD_ISSET(sock[2], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[2], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[2].SetCmmuData(*data[2]);

			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[4��send����Ă�����
		if (FD_ISSET(sock[3], &fds))
		{
			recv = communication[3]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[3], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[3].SetCmmuData(*data[3]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}

		// �X�N���[������
		system("cls");

		// ���\���ׂ̈̕ϐ�
		CCommunicationData::COMMUNICATION_DATA *screen_info[MAX_PLAYER + 1];
		
		// �����擾
		for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
		{
			screen_info[count_playr] = commu_data[count_playr].GetCmmuData();
		}
		cout << "=======================================================" << endl;
		cout << "���[�� : " << listener->GetNumber() << endl;
		cout << "Player : 1->pos" << screen_info[0]->Player.Pos.x <<" : " << screen_info[0]->Player.Pos.y <<" : " << screen_info[0]->Player.Pos.z << endl;
		cout << "Player : 1->rot" << screen_info[0]->Player.Rot.x <<" : " << screen_info[0]->Player.Rot.y <<" : " << screen_info[0]->Player.Rot.z << endl;
		cout << "Player : 2->pos" << screen_info[1]->Player.Pos.x <<" : " << screen_info[1]->Player.Pos.y <<" : " << screen_info[1]->Player.Pos.z << endl;
		cout << "Player : 2->rot" << screen_info[1]->Player.Rot.x <<" : " << screen_info[1]->Player.Rot.y <<" : " << screen_info[1]->Player.Rot.z << endl;
		cout << "Player : 3->pos" << screen_info[2]->Player.Pos.x <<" : " << screen_info[2]->Player.Pos.y <<" : " << screen_info[2]->Player.Pos.z << endl;
		cout << "Player : 3->rot" << screen_info[2]->Player.Rot.x <<" : " << screen_info[2]->Player.Rot.y <<" : " << screen_info[2]->Player.Rot.z << endl;
		cout << "Player : 4->pos" << screen_info[3]->Player.Pos.x <<" : " << screen_info[3]->Player.Pos.y <<" : " << screen_info[3]->Player.Pos.z << endl;
		cout << "Player : 4->rot" << screen_info[3]->Player.Rot.x <<" : " << screen_info[3]->Player.Rot.y <<" : " << screen_info[3]->Player.Rot.z << endl;
		cout << "=======================================================" << endl;
	}

	// �I������
	for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
	{
		if (communication[count_player] != nullptr)
		{
			communication[count_player]->Uninit();
			delete communication[count_player];
			communication[count_player] = nullptr;
		}
	}
	if (listener != nullptr)
	{
		listener->Uninit();
	}

	// �A�N�Z�v�g�������炷
	g_accept_count--;
}

//------------------------
// �I������
//------------------------
void StopOrSurver(void)
{
	while (g_stop != 1)
	{
		cin >> g_stop;
	}
}

//------------------------
// �L�[���͑҂�
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}